#pragma once

#include <QThread>
//#include <QMutex>

#define LOGLF "\n                        " ///

class Log : public QThread
{
	Q_OBJECT

public:
	enum eLogType {
		E   	= (1 <<  0), //0x0001 : error
		W		= (1 <<  1), //0x0003 :	warning
		Qt 		= (1 <<  2), //0x0007 : qt warning
		F		= (1 <<  3), //0x000F : function calls
		S   	= (1 <<  4), //0x001F : startup
		C   	= (1 <<  5), //0x003F : com
		PDF		= (1 <<  6), //0x007F : creating PDF
		f7		= (1 <<  7), //0x00FF : 
		f8  	= (1 <<  8), //0x01FF : 
		f9		= (1 <<  9), //0x03FF : 
		f10		= (1 << 10), //0x07FF : 
		f11		= (1 << 11), //0x0FFF : 
		f12		= (1 << 12), //0x1FFF : 
		f13		= (1 << 13), //0x3FFF : 
		f14		= (1 << 14), //0x7FFF :
		f15		= (1 << 15), //0xFFFF : 
	};


    Log();
    ~Log();

	void init(QString dirName, QString fileName, QString addHeaderText = "");
	void installMsgHandler();
	QString send(enum eLogType f, QString s);
	QString send(enum eLogType f, const char* format, ...);
	int	filter() { return m_Filter; };
	void setConditionFlag(int f);
	void signalONforBitChanged(int bit);
	void signalOFFforBitChanged(int bit);

protected:
     void run();

private:
	void write(QString s);	
	void checkForBitChanged(int oldFilter);

	// ?? QMutex		m_Mutex;
	int			m_Filter;
	QString		m_FileName; 
	int			m_FileSizeMax;
	int			m_CondFlag;
	QList<int>	m_lstBitChanged;
	bool		m_firstSigLoggingFilter;

signals:
	void signal_Send(QString s);
	void sig_BitChanged(int bit, int newBitVal);
   
public slots:
	void slot_Write(QString s);

private slots:
	void slot_LoggingFilter(int filter);
};

extern Log* Service_Log;
#define logs Service_Log->send
