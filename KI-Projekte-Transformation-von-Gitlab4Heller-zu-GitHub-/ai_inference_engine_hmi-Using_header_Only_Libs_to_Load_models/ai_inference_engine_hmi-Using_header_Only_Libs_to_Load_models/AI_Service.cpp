#include "AI_Service.h"
AI_Service::AI_Service()
{
	//QString path = "S:/hmisl/oem/sinumerik/hmi/appl/Config.json";
	QString path = "../../../../oem/sinumerik/hmi/appl/ai_engine/Config.json";
	this->settings = new Configs(new Json_Handler(path));
	this->control_unit_handler = new Control_Unit_handler();
	this->setPriority(QThread::HighestPriority);
	
}
AI_Service::~AI_Service()
{
	this->control_unit_handler->~Control_Unit_handler();
	this->preprocessor->~Model();
	this->model->~Model();
	this->quit();
	this->wait();
	logs(Log::F, QString("AI_Service::~AI_Service(): AI_Service is successfully destructed!"));
}

void AI_Service::init()
{

	this->settings->read_Configs();
	this->control_unit_handler->set_timeout(this->settings->get_timeout());
	this->model = new Model(this->settings->get_model_input_shape(), this->settings->get_model_output_shape(), new Fdeep_handler(this->settings->get_model_path()));
	this->model->load();
	QVector<int>* preprocessor_data_dim= new QVector<int>((int)1);
	preprocessor_data_dim->replace(0,4);
	this->preprocessor = new Model(preprocessor_data_dim, preprocessor_data_dim, new Fdeep_handler(this->settings->get_preprocessor_path()));
	this->preprocessor->load();
	logs(Log::F, QString("AI_Service::init(): AI_Service is successfully initialized!"));
}

QList<QString> AI_Service::get_reading_addresses()
{
	QList<QString> reading_adresses;
	foreach(QString input_signal, *this->settings->get_input_signals()) {
		reading_adresses.append(this->settings->get_addresses().value(input_signal));
	}
	return reading_adresses;
}

QVector<float> AI_Service::fill_buffer(QVector<float>& buffer, QVector<float> datapoint) {

	int  datapoint_size = datapoint.size();
	for (int i = 0; i < buffer.size(); i++) {
		int index = i % datapoint_size;
		buffer[i] = datapoint[index];
	}
	return buffer;
}

QVector<float> AI_Service::shift_insert_to_buffer(QVector<float>& buffer, QVector<float> datapoint) {
	int point_length= datapoint.length();
	//Remove last datapoint
	for (int i = 0;i < point_length;i++){
		buffer.push_front(datapoint[point_length-1-i]);
		buffer.removeLast();
	}
	return buffer;

}

void AI_Service::run() {
	try {
		this->is_running = true;
		QList<QString> input_addresses = this->get_reading_addresses();
		bool first = true;
		bool must_predict_next = true;
		int buffer_dim = 1;
		QString trace_text = "";
		QString data_trace_text = "";
		for (int i = 0; i < this->model->get_input_shape()->count(); i++)
		{
			////Problem
			buffer_dim = buffer_dim * this->model->get_input_shape()->value(i);
		}
		//define the buffer
		QVector<float> buffer(buffer_dim, 1.0f); // Initialize with some sample data.
		QDateTime current_time = QDateTime::currentDateTime();
		QDateTime prev_time = current_time;
		QVector <QString>* input_signal_names = this->settings->get_input_signals();
		while (this->is_running) {
			current_time = QDateTime::currentDateTime();
			trace_text = "Current Time Stamp= " + current_time.toString("dd.MM.yyyy hh:mm:ss.zzz") + "\n";
			if (first) {
				first = false;
				/////
				//read datapoint from the control
				QVector<float> input_datapoint = this->control_unit_handler->read_signal(input_addresses);
				//Add input signals to trace Text
				data_trace_text = "";
				for (int i = 0; i < input_datapoint.size(); i++)
				{
					QString signal_trace = input_signal_names->at(i) + "=  " + QString::number(input_datapoint.at(i)) + "\n";
					data_trace_text += signal_trace;
				}
				//transfom it
				QVector<float> input_datapoint_transformed = this->preprocessor->execute(input_datapoint, *this->settings->get_cov_matrix(),
					this->preprocessor->get_input_shape(), this->settings->get_cov_matrix_shape());
				/////
				//fill the buffer with the current point
				buffer = fill_buffer(buffer, input_datapoint_transformed);
				//apply prediction
				QVector<float> prediction = this->model->execute(buffer, 1, this->model->get_buffer_size(), this->model->get_input_shape()->at(2), 3, true);
				//write the prediction in the control
				data_trace_text += "Prediction= " + QVariant(prediction[0, 0]).toString() + "\n";
				this->control_unit_handler->write_value(this->settings->get_prediction_address(), QVariant(prediction[0, 0]));
				prev_time = current_time;
			}
			else
			{
				int delta = current_time.toMSecsSinceEpoch() - prev_time.toMSecsSinceEpoch();
				trace_text += "Time Delta= " + QString::number(delta) + "\n";
				must_predict_next = delta >= (*this->settings->get_response_time() * 1000);
				if (must_predict_next) {
					/////
					//read datapoint from the control
					QVector<float> input_datapoint = this->control_unit_handler->read_signal(input_addresses);
					data_trace_text = "";
					//Add input signals to trace Text
					for (int i = 0; i < input_datapoint.size(); i++)
					{
						QString signal_trace = input_signal_names->at(i) + "=  " + QString::number(input_datapoint.at(i)) + "\n";
						data_trace_text += signal_trace;
					}
					//transfom it
					QVector<float> input_datapoint_transformed = this->preprocessor->execute(input_datapoint, *this->settings->get_cov_matrix(),
						this->preprocessor->get_input_shape(), this->settings->get_cov_matrix_shape());
					/////
					//update the buffer with the new point
					buffer = shift_insert_to_buffer(buffer, input_datapoint_transformed);
					//apply prediction
					QVector<float> prediction = this->model->execute(buffer, 1, this->model->get_buffer_size(), this->model->get_input_shape()->at(2), 3, true);
					data_trace_text += "Prediction= " + QVariant(prediction[0, 0]).toString() + "\n";
					//write the prediction in the control
					this->control_unit_handler->write_value(this->settings->get_prediction_address(), QVariant(prediction[0, 0]));
					prev_time = current_time;
				}
				else {
					trace_text += "No Prediction must be calculated now. Last Prediction State: \n";
				}
			}
			trace_text = trace_text + data_trace_text;
			emit signal_changeText(trace_text);
			QThread::sleep(this->settings->get_sleeping_period());
		}
	}
	catch (std::exception exp) {
		logs(Log::E, QString("AI_Service::run(): Exception in main run ! the exeption is: %1!").arg(exp.what()));
		emit signal_changeText(exp.what());
	}
}
void AI_Service::stop()
{
	this->is_running = false;
	this->wait();
	logs(Log::F, QString("AI_Service::stop(): AI_Service is stopped!"));
}