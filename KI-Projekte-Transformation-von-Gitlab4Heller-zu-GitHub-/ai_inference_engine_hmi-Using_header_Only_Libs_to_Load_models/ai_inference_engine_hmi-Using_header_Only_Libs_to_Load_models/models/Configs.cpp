#include "Configs.h"

Configs::Configs(Assets_Handler * assets_Handler):Asset(assets_Handler){
	this->load();
}

Configs::~Configs()
{
}

void Configs::read_Configs()
{
	this->model_input_shape = this->read_model_input_shape();
	this->model_output_shape = this->read_model_output_shape();
	this->input_signals = this->read_input_signals();
	this->model_path= this->read_model_path();
	this->preprocessor_path= this->read_preprocessor_path();
	this->prediction_address= this->read_prediction_address();
	this->addresses= this->read_addresses();
	this->time_relevance= this->read_time_relevance();
	this->recording_Frequency= this->read_recording_Frequency();
	this->sleeping_period= this->read_sleeping_period();
	this->response_time = this->read_response_time();
	this->timeout = this->read_timeout();
	this->cov_matrix = this->read_cov_matrix();
	this->cov_matrix_shape = this->read_cov_matrix_shape();
	logs(Log::F, QString("Configs::read_Configs(): Configs read successfully"));
}


QVector<int>* Configs::read_model_input_shape() {
	QVector<int>* result = new QVector<int>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonArray array = my_handler->parse("model_input_shape").toArray();
	foreach(const QJsonValue & val, array) {
		int signal = val.toInt();
		result->append(signal);
	}
	return  result;
}
QVector<int>* Configs::read_model_output_shape() {
	QVector<int>* result = new QVector<int>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonArray array = my_handler->parse("model_output_shape").toArray();
	foreach(const QJsonValue & val, array) {
		int signal = val.toInt();
		result->append(signal);
	}
	return result;
}


QVector<QString>* Configs::read_input_signals() {
	QVector<QString>* result = new QVector<QString>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonArray array = my_handler->parse("input_signals").toArray();
	foreach(const QJsonValue & val, array) {
		QString signal= val.toString();
		result->append(signal);
	}
	return  result;
}
QString Configs::read_model_path() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QString result = my_handler->parse("model_path").toString();
	return  result;
}
QString Configs::read_preprocessor_path() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QString result = my_handler->parse("preprocessor_path").toString();
	return  result;
}
QString Configs::read_prediction_address() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QString result = my_handler->parse("prediction_address").toString();
	return  result;
}
QHash<QString, QString> Configs::read_addresses() {
	QHash<QString, QString> result= QHash<QString, QString>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonObject address_list = my_handler->parse("addresses").toObject();
	QStringList keys = address_list.keys();
	foreach(const QString key, keys) {
		QString address = address_list.value(key).toString();
		result[key] = address;
	}
	return  result;
}
QVector<float>* Configs::read_cov_matrix()
{
	QVector<float>* result = new QVector<float>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonArray array = my_handler->parse("cov_matrix").toArray();
	foreach(const QJsonValue & val, array) {
		float signal = val.toDouble();
		result->append(signal);
	}
	return result;
}
int Configs::read_time_relevance() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	int result = my_handler->parse("time_relevance").toInt();
	return  result;
}
QVector<int>* Configs::read_cov_matrix_shape()
{
	QVector<int>* result = new QVector<int>();
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	QJsonArray array = my_handler->parse("cov_matrix_shape").toArray();
	foreach(const QJsonValue & val, array) {
		int signal = val.toInt();
		result->append(signal);
	}
	return result;
}
int Configs::read_recording_Frequency() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	int result = my_handler->parse("recording_Frequency").toInt();
	return  result;
}
float Configs::read_sleeping_period() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	float result= (float) my_handler->parse("sleeping_period").toDouble();
	return  result;
}


int Configs::read_response_time() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	int result = my_handler->parse("response_time").toInt();
	return  result;
}

int Configs::read_timeout() {
	Json_Handler* my_handler = (Json_Handler*)this->assets_handler;
	int result = my_handler->parse("timeout").toInt();
	return  result;
}

QVector<int>* Configs::get_model_input_shape()
{
	return this->model_input_shape;
}

QVector<int>* Configs::get_model_output_shape()
{
	return this->model_output_shape;
}

QVector<float>* Configs::get_cov_matrix()
{
	return this->cov_matrix;
}
QVector<int>* Configs::get_cov_matrix_shape()
{
	return this->cov_matrix_shape;
}
QVector<QString>* Configs::get_input_signals()
{
	return this->input_signals;
}

QString Configs::get_model_path()
{
	return this->model_path;
}

QString Configs::get_preprocessor_path()
{
	return this->preprocessor_path;
}

QString Configs::get_prediction_address()
{
	return this->prediction_address;
}

QHash<QString, QString> Configs::get_addresses()
{
	return this->addresses;
}

int Configs::get_time_relevance()
{
	return this->time_relevance;
}

int Configs::get_recording_Frequency()
{
	return this->recording_Frequency;
}
int*  Configs::get_response_time()
{
	return &this->response_time;
}
float Configs::get_sleeping_period()
{
	return this->sleeping_period;
}

int Configs::get_timeout()
{
	return this->timeout;
}
