#include "log.h"

//#include "version.h"
#include "slextracesvrdefines.h"
//#include "settings.h"
//#include "apvdialog.h"

//Erweiterung Versionsinfo
#include "slverversioninfo.h"
//#include "myversionsstring.h"
//#include "slverversionstring.h"


#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QMutexLocker>

static void qtMessage(QtMsgType type, const char *msg)
{
	switch (type) {
	//case QtDebugMsg:
	//	log(Log::Qt, "QtDebugMsg: %s", msg); break;
	case QtWarningMsg:
		logs(Log::Qt, "QtWarningMsg: %s", msg); break;
	case QtCriticalMsg:
		logs(Log::Qt, "QtCriticalMsg: %s", msg); break;
	case QtFatalMsg:
		logs(Log::Qt, "QtFatalMsg: v", msg); break;
	}
}

Log* Service_Log;

Log::Log()
	: QThread()
{
	m_CondFlag = 0;
	m_firstSigLoggingFilter = true;
}

Log::~Log()
{
	if (isRunning()) {
		exit();
		wait(3000);	// ?
	}
}

void Log::init(QString dirName, QString fileName, QString logName)
{
	QString s, s1;

	//m_LogName = logName;
	m_FileName = QString(APPLPATH) + "/" + dirName + "/" + fileName;
	m_FileSizeMax = MAX_LOG_FILE_SIZE;
	write("");
	//s = QString("----------------------------- HELLER tool planing (TRP) ");
	if (logName.isEmpty() == false)
		s1.append(" (").append(logName).append(")");  
	//s.append("----------------------------");
	//write(s);

	//Versionsinfo ermitteln und loggen
	const char* ver = "v0.0.0";//SL_MODULE::SL_VER_VERSION_INFO; // = getModuleVersion();      //slVerGetVersion(); funktioniert nicht
	SlVerVersionInfo verInfo;
	verInfo.setVersionString(ver);
	s = QString("%1: %2%3 - %4 (%5)").arg(verInfo.productName()).arg(verInfo.moduleName()).arg(s1).arg(verInfo.comment()).arg(verInfo.version());
	write(s);


	s = QString::number(FILTER);
	QString fs;
	if (s.length() <= 16) 
		fs = s.leftJustified(16, '1'); 
	else
		fs = s.left(16); 

	bool ok;
	m_Filter = fs.toInt(&ok, 2); 
	if (ok == false)
	{
		m_Filter = 0xFFFF;
		write(QString("Log::init() ERROR: Filter settings"));
	}

	//installMsgHandler();

	write(QString("Log::init() Filter=0x%1 MaxFileSize=%2").
		arg(QString::number(m_Filter, 16)).arg(m_FileSizeMax));
}

void Log::setConditionFlag(int f)
{
	m_CondFlag = f;
}

// Handler für Messages installieren
// Muss nach Dialog::open() wiederholt werden 
void Log::installMsgHandler() 
{
/*
	write("Log::installMsgHandler()");
	qInstallMsgHandler(&qtMessage);
#if 1
	if (qInstallMsgHandler(&qtMessage) == &qtMessage)
		;//write(m_FileName, "Log::init(): qInstallMsgHandler() ok");
	else 
	{
		write("Log::installMsgHandler(): qInstallMsgHandler(&qtMessage) != &qtMessage");
		qInstallMsgHandler(&qtMessage);
	}
#endif
*/
}
 
void Log::run()
{
	write("Log::run()");

	if (!connect(this, SIGNAL(signal_Send(QString)),
		this, SLOT(slot_Write(QString)), Qt::QueuedConnection)) {
		write("Log::run() Error connect(signal_Send, slot_Write)"); return;
	}
	exec();
	write("Log::run() End"); // ?
}

QString Log::send(enum eLogType f, QString s)
{
	//int allFlags = m_CondFlag | f;

	//if ((m_Filter & m_CondFlag) && (m_Filter & f))
	if (m_Filter & f)
	{
//if (isRunning()) 
//	emit signal_Send(s);
//else 
		{
//write("Log thread not running!");
			write(s);
		}
	}
	return s;
}

QString Log::send(enum eLogType f, const char* format, ...)
{
	va_list marker;
	va_start(marker, format);
	char b[1024];
	vsprintf(b, format, marker);
	return send(f, QString(b));
}

void Log::slot_Write(QString s)
{
	write(s);
}

void Log::write(QString s)
{
	QFile file(m_FileName);
	if (file.exists()) { // Größe prüfen
		if (file.size() >= m_FileSizeMax) {
			QString fbakName = QString(m_FileName).append(".old");
			QFile fbak; fbak.setFileName(fbakName); 
			if (fbak.exists()) fbak.remove(); 
			file.rename(fbakName); 
		}
	}
	if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
		return;
	QString date = "";
	if (s != "")
		date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
	QTextStream out(&file);
	out << date << s << endl;
	file.close(); 
}


void Log::slot_LoggingFilter(int filter)
{
	checkForBitChanged(filter);

	m_Filter = filter;
	QString s = QString("LOG FILTER CHANGED: 0x%1").arg(QString::number(m_Filter, 16));
	emit signal_Send(s);
}

void Log::signalONforBitChanged(int bit)
{
	m_lstBitChanged.append(bit);
}

void Log::signalOFFforBitChanged(int bit)
{
	int index;
	if ((index = m_lstBitChanged.indexOf(bit)) >= 0)
		m_lstBitChanged.takeAt(index);
}

void Log::checkForBitChanged(int newFilter)
{
	for (int i = 0; i < m_lstBitChanged.count(); i++)
	{
		int oldBitVal = m_Filter & m_lstBitChanged.at(i);
		int newBitVal = newFilter & m_lstBitChanged.at(i);

		if (m_firstSigLoggingFilter == true)
		{
			m_firstSigLoggingFilter = false;
			emit sig_BitChanged(m_lstBitChanged.at(i), newBitVal);
		}
		else
		{
			if (oldBitVal != newBitVal)
				emit sig_BitChanged(m_lstBitChanged.at(i), newBitVal);
		}
	}
}
