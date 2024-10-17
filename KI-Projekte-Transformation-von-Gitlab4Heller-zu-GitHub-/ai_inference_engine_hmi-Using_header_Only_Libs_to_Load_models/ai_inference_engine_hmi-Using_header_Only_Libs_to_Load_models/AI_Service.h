#pragma once
#include "Control_Unit_handler.h"
#include "Configs.h"
#include "Model.h"
#include "Fdeep_handler.h"
#include "Json_Handler.h"
#include <QThread>
#include <QObject>
#include "slextracesvrdefines.h"


class AI_Service: public QThread
{
	Q_OBJECT
private:
	Configs* settings = nullptr;
	Model * preprocessor = nullptr;
	Model * model = nullptr;
	Control_Unit_handler* control_unit_handler = nullptr;
	bool is_running = false;
	QList<QString> get_reading_addresses();
	QVector<float> fill_buffer(QVector<float>& buffer, QVector<float> datapoint);
	QVector<float> shift_insert_to_buffer(QVector<float>& buffer, QVector<float> datapoint);
protected:
	virtual void run();
public:
	AI_Service();
	~AI_Service();
	void init();
	void stop();
signals:
		void signal_changeText(QString newText= "");

};




