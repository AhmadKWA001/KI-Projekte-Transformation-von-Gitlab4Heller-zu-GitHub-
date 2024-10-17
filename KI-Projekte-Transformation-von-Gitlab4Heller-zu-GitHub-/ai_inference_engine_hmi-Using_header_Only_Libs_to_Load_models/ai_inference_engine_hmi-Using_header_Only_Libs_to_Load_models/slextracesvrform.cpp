///////////////////////////////////////////////////////////////////////////////
/*! \file   slextracesvrform.cpp
 *  \brief  Class implementation file for class SlExTraceSvrForm
 *
 *  This file is part of the HMI Solutionline Programming Package
 *
 *  (C) Copyright Siemens AG A&D MC 2007. All rights reserved.
 */
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QHBoxLayout>
#include "slextracesvrform.h"
#include "slgfwhmidialog.h"
#include "slgfwsoftkey.h"
#include "slgfwlabel.h"
#include "slgfwlineedit.h"
#include "slhmisettingsqt.h"
# include <QObject>
///////////////////////////////////////////////////////////////////////////////
// CREATORS
///////////////////////////////////////////////////////////////////////////////

SlExTraceSvrForm::SlExTraceSvrForm(QWidget* pParent, const QString& rszName)
	: SlGfwDialogForm(pParent, rszName),
	  m_pLeStatus(0),
	  m_pTbDisplayedXml(0)
{
	Service_Log = new Log();
	Service_Log->init(LOG_DIR, LOG_FN);
	Service_Log->start(QThread::HighestPriority);
	Service_Log->setPriority(QThread::NormalPriority);
	//Service_Log->moveToThread(Service_Log);
	logs(Log::F, QString("SlExTraceSvrForm::SlExTraceSvrForm()"));
	//mainform = this;
	// create the label widgets
	SlGfwLabel* pLblStatus = new SlGfwLabel("APP State: ",this);
	// create the line edit widgets
	m_pLeStatus  = new SlGfwLineEdit(QString::null, this);

	// create the text widget for XML data
    m_pTbDisplayedXml = new QTextBrowser(this);

	// set properties
	if ( 0 != m_pLeStatus )
		m_pLeStatus->setEnabled(false);					// disable display widget

	// create layout (one vertical layout contains a horizontal layouts and the text widget)
	QVBoxLayout* pVLayout = new QVBoxLayout(this);	
	if ( 0 != pVLayout )
	{
		QHBoxLayout* pHLayout = new QHBoxLayout(this);
		if ( 0 != pHLayout )
		{
			pHLayout->addStretch(1);
			pHLayout->addWidget(pLblStatus, 1);
			pHLayout->addWidget(m_pLeStatus, 2);
			pHLayout->addStretch(1);
		}

		// add layouts to central layout
		pVLayout->addLayout(pHLayout);
		pVLayout->addWidget(m_pTbDisplayedXml);
	}

	// initialize status bar
	setStatusBarElementCount(1);
	setStatusBarVisible(true);
	setStatusBarElementAlignment(Qt::AlignLeft, 0);
	// set the text of the caption bar
	setCaptionBarElementCount(1);    
	setCaptionBarElementText("AI Inference Engine Prototype", 0);

	
}

SlExTraceSvrForm::~SlExTraceSvrForm (void)
{
} 


////////////////////////////////////////////////////////////////////////////////
// OVERWRITTEN METHODS
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// a screen was attached
void SlExTraceSvrForm::attachedToScreen(const QString& rszScreenName)
{
	logs(Log::F, QString("SlExTraceSvrForm::attachedToScreen()"));
	// disable all softkeys
	enableSoftkey("myStopPrediction", false);

	// clear status bar
	setStatusBarElementText("", 0);
}

///////////////////////////////////////////////////////////////////////////
// Will be called by the framework, if a function should be executed, e.g. if a softkey
// should execute a command
void SlExTraceSvrForm::onFunction(const QString& rszFunction, const QString& rszArgs, bool& rbHandled)
{
	long		lRetVal = 0;
	QString		szRetVal = QString::null;
	logs(Log::F, QString("SlExTraceSvrForm::onFunction()"));
	// create session
	if ( "myStartPrediction" == rszFunction )
	{
		if ( 0 == (lRetVal = CreateSession()) )
			szRetVal = SUCC_SESSION_CREATE;

		// set that the event was handled by this function
		rbHandled = true;
	}
	// get trace data
    else if ( "myTracedData" == rszFunction )
	{
		//if ( 0 == (lRetVal = TracedData()) )
		//	szRetVal = SUCC_DATA_TRACED;

		//// set that the event was handled by this function
		rbHandled = true;
	}
	// Stop prediction
	else if ( "myStopPrediction" == rszFunction )
	{
		if ( 0 == (lRetVal = DeleteSession()) )
			szRetVal = SUCC_SESSION_DELETE;	
		
		// set that the event was handled by this function
		rbHandled = true;
	}

	handleRetVal(lRetVal, szRetVal);
	
	// need to call the base class
	if ( false == rbHandled )
		SlGfwDialogForm::onFunction(rszFunction, rszArgs, rbHandled);
}


///////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// it creates a session
long SlExTraceSvrForm::CreateSession()
{
	logs(Log::F, QString("SlExTraceSvrForm::CreateSession()"));
	ai_Service= new AI_Service();
	ai_Service->init();
	//QThread::currentThread()->moveToThread(ai_Service);
	bool result = QObject::connect(ai_Service, SIGNAL(signal_changeText(QString)), this, SLOT(OnTextChange(QString)));
	ai_Service->start();
	m_pLeStatus->setValue("Running");
	enableSoftkey("myStopPrediction", true);
	enableSoftkey("myStartPrediction", false);
	//Q_ASSERT(m_pSLTraceAdapter != 0);
	//if (m_pSLTraceAdapter == 0)
		//return(ERR_ADAPTER_FAILURE);
 
	//eSlError = m_pSLTraceAdapter->CreateSession(szClientName, szPassword, szSessionName, m_pISessConn);
    ///if ( 0 > eSlError )
		//return(eSlError);
          
   // if ( !m_pISessConn )
       // return(ERR_BAD_SESSION_POINTER);
/*
	QObject::connect(m_pISessConn, SIGNAL(StateChange(TraceSessStateEnum, SlTraceErrEnum)),
                     this,         SLOT(OnStateChange(TraceSessStateEnum, SlTraceErrEnum)));

	QObject::connect(m_pISessConn, SIGNAL(DataReady(ulong)),
					 this,        SLOT(OnDataReady(ulong)));
					 */
	// softkeys disable/enable
	//enableSoftkey("myStartPrediction", false);
	//enableSoftkey("myCaptureSetup", true);
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////
// it captures a setup from a file
//long SlExTraceSvrForm::CaptureSetup()
//{
//	long				lErrOffset = 0;
//	QString				szCaptureSetupXML;				// Session Capture setup in XML format
//	QString				szCaptureSetupFile;
//	SlHmiFileAccessQt	slFileAccessQt;
//	SlTraceErrEnum		eSlError = TRACE_NO_ERROR;      // error enumerations returned by Trace Service adaptor
//
//	this->ai_service->stop();
//	// Get path of the CaptureSetup file: sample.xml
//	if ( 0 > slFileAccessQt.searchFile("sample.xml", "appl", szCaptureSetupFile) )
//		szCaptureSetupFile = QString::null;
//
//	QFile	qFileXML(szCaptureSetupFile);
//	if (!qFileXML.open(QFile::ReadOnly | QFile::Text))	// Open CaptureSetup file
//         return(ERR_CANT_OPEN_XML_FILE);
//
//	QTextStream		qTextFileXML(&qFileXML);
//	szCaptureSetupXML = qTextFileXML.readAll();			// Read content of CaptureSetup file
//
//	qFileXML.close();									// Close CaptureSetup file
//
//	m_pTbDisplayedXml->clear();
//
//    if ( !m_pISessConn )
//        return(ERR_BAD_SESS_POINTER);
//
//    // Assign the session capture setup data to the session. We pass the capture setup XML element to 
//    // the Trace Service to specify what the logging session will do, what signals to collect.
//	eSlError = m_pISessConn->PutCaptureSetup(szCaptureSetupXML, lErrOffset);
//	if ( 0 > eSlError )
//		return(eSlError);
//
//	m_pTbDisplayedXml->setText(szCaptureSetupXML);		// Show CaptureSetup
//
//	// softkeys disable/enable
//	enableSoftkey("myCaptureSetup", false);
//	enableSoftkey("myStartTrace", true);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////
// it starts the trace
//long SlExTraceSvrForm::StartTrace()
//{
//	SlTraceErrEnum		eSlError = TRACE_NO_ERROR;          // error enumerations returned by Trace Service adaptor
//
//
//    if ( !m_pISessConn )
//        return(ERR_BAD_SESS_POINTER);
//
//	// Start / Arm the session to initiate the monitoring of events to log.
//    // If there is a start trigger, logging will begin when the start trigger conditions are met, 
//    // otherwise logging will begin immediately. The session status will be either Armed or Recording.
//   	eSlError = m_pISessConn->Start();
//	if ( 0 > eSlError )
//		  return(eSlError);
//
//    m_szSaveDataXML.clear();
//
//	// softkeys disable/enable
//	enableSoftkey("myStartTrace", false);
//	enableSoftkey("myStopTrace", true);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////
// it stops the trace
//long SlExTraceSvrForm::StopTrace()
//{
//	TraceSessStateEnum	CurrentSessionState = traceStateFault;
//	SlTraceErrEnum		eSessionFault = TRACE_NO_ERROR;
//	SlTraceErrEnum		eSlError = TRACE_NO_ERROR;          // error enumerations returned by Trace Service adaptor
//
//
//    if ( !m_pISessConn )
//        return(ERR_BAD_SESS_POINTER);
//
//    // Verify the current status of the session
//    eSlError = m_pISessConn->GetSessionState(CurrentSessionState, eSessionFault);
//    if ( 0 > eSlError )
//		return(eSlError);
//
//    // if the session is armed or recording stop the session now.
//	if ( (CurrentSessionState == traceStateRecording) || (CurrentSessionState == traceStateArmed) )
//    {
//        eSlError = m_pISessConn->Stop();
//		if ( 0 > eSlError )
//			return(eSlError);
//    }
//
//	// softkeys disable/enable
//	enableSoftkey("myStopTrace", false);
//	enableSoftkey("myTracedData", true);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////
 //it gets trace data 
long SlExTraceSvrForm::TracedData()
{
	//long			lMaxDataSize = MAX_READ_DATA_SIZE;		// maximum data size in bytes to request on GetDataXML()
	//long			lGetDataOptions = 0;					// GetDataXML switches, requests hearder, signal defs, data
	//bool			fDataReadyToRead = false;
	//bool			fMoreDataLogged = true;					// More data has been logged
	//QString			szLogDataInXML;							// holds the raw XML returned from the Trace Service
	//SlTraceErrEnum	eSlError = TRACE_NO_ERROR;				// error enumerations returned by Trace Service adaptor


 //   if ( !m_pISessConn )
 //       return(ERR_BAD_SESS_POINTER);

	//eSlError = m_pISessConn->DataPresent(fDataReadyToRead);
	//if ( 0 > eSlError )
	//	  return(eSlError);

	//if ( false == fDataReadyToRead )
	//{
	//	// softkeys disable/enable
	//	enableSoftkey("myTracedData", false);
	//	enableSoftkey("myCloseSession", true);
	//	return(ERR_NO_DATA_FAILURE);
	//}

 //   // Data is ready in the Trace Service, prepare to retrieve it.
 //   // Continue to attempt to ready data, as long as the more data Boolean is true and not detected an error.

 //   if ( !m_szSaveDataXML.size() )							// if first data packet ...
	//	lGetDataOptions = traceGetDataFrames +				// return the data frame
	//					  traceGetDataHeader +				// return the description header
	//					  traceGetDataSignals;				// return the data signal description
	//else													// if subesquent data packets ...
	//	lGetDataOptions = traceGetDataFrames;				// return the data frame only
 //          
 //   szLogDataInXML.clear();
 //   // Read the available log data from the trace service in the XML data format
 //   eSlError = m_pISessConn->GetDataXml(lGetDataOptions, lMaxDataSize, szLogDataInXML, fMoreDataLogged);
 //   if ( 0 > eSlError )							// check if data request failed
	//	return(eSlError);
 //         
 //   if ( szLogDataInXML.size() > 0 )
	//{
	//	m_szSaveDataXML += szLogDataInXML;				// if data returned, then save it
	//	m_pTbDisplayedXml->setText(m_szSaveDataXML);	// show the acquired data in the text box
	//}

 //   // When no data returned, check session status and wait a while for more data to be collected in trace service

 //   if ( !fMoreDataLogged )
	//{
	//	// softkeys disable/enable
	//	enableSoftkey("myTracedData", false);
	//	enableSoftkey("myClearData", true);
	//}
	//
	m_pTbDisplayedXml->setText(m_szSaveDataXML);
	return 0;
}

///////////////////////////////////////////////////////////////////////////
// it clears the traced data
//long SlExTraceSvrForm::ClearData()
//{
//	SlTraceErrEnum		eSlError = TRACE_NO_ERROR;          // error enumerations returned by Trace Service adaptor
//
//
//    if ( !m_pISessConn )
//        return(ERR_BAD_SESS_POINTER);
//
//    // Clear the data from the Session data buffers
//    eSlError = m_pISessConn->ClearData();
//    if ( 0 > eSlError )
//		return(eSlError);
//
//	m_pTbDisplayedXml->clear();
//
//	// softkeys disable/enable
//	enableSoftkey("myClearData", false);
//	enableSoftkey("myCloseSession", true);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////
// it closes the session
//long SlExTraceSvrForm::CloseSession()
//{
//	SlTraceErrEnum	eSlError = TRACE_NO_ERROR;          // error enumerations returned by Trace Service adaptor
//
//
//    if ( !m_pISessConn )
//        return(ERR_BAD_SESS_POINTER);
//
//	QObject::disconnect(m_pISessConn, SIGNAL(DataReady(ulong)),
//                        this,         SLOT(OnDataReady(ulong)));
//
//	QObject::disconnect(m_pISessConn, SIGNAL(StateChange(TraceSessStateEnum, SlTraceErrEnum)),
//                        this,         SLOT(OnStateChange(TraceSessStateEnum, SlTraceErrEnum)));
//
//    // Close the Session Connection
//    eSlError = m_pISessConn->Close();
//    if ( 0 > eSlError )
// 		return(eSlError);
//
//	m_pISessConn = 0;
//
//	// softkeys disable/enable
//	enableSoftkey("myCloseSession", false);
//	enableSoftkey("myStopPrediction", true);
//
//	return 0;
//}

///////////////////////////////////////////////////////////////////////////
// it deletes the session
long SlExTraceSvrForm::DeleteSession()
{
	logs(Log::F, QString("SlExTraceSvrForm::DeleteSession()"));
	SlTraceErrEnum	eSlError = TRACE_NO_ERROR;          // error enumerations returned by Trace Service adaptor
	if ( 0 > eSlError )
		return(eSlError);

	m_pLeStatus->setValue("Stopped");
	ai_Service->stop();
	// softkeys disable/enable
	enableSoftkey("myStopPrediction", false);
	enableSoftkey("myStartPrediction", true);
	ai_Service->~AI_Service();
//	return 0;
	return(eSlError);
}

void SlExTraceSvrForm::OnTextChange(QString newText)
{
	m_pTbDisplayedXml->setText(newText);
}


///////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// is send, if data is provided
//void SlExTraceSvrForm::OnDataReady(ulong)
//{
//	setStatusBarElementText("Traced data is ready to retrieve it", 0);
//
//	m_pTbDisplayedXml->clear();
//
//	enableSoftkey("myTracedData", true);			// softkey enable
//}

///////////////////////////////////////////////////////////////////////////////
//// is send, if state has changed
//void SlExTraceSvrForm::OnStateChange(TraceSessStateEnum SessionState, SlTraceErrEnum Error)
//{
//	if ( 0 != m_pLeStatus )
//		m_pLeStatus->setValue(getStateAsText(SessionState));
//
//	if ( SessionState == traceStateStopped )
//		enableSoftkey("myStopTrace", false);		// softkeys disable
//}


///////////////////////////////////////////////////////////////////////////
// PRIVATE HELPERS
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// converts a session state to a QString (for display it in StatusBar)
//QString SlExTraceSvrForm::getStateAsText(TraceSessStateEnum SessionState)
//{
//	QString szRetVal = QString::null;
//
//	// switch to session state and return the value as QString
//	switch (SessionState)
//	{
//		case traceStateReserved:
//			szRetVal = "Reserved"; 
//			break;
//
//		case traceStateReady:
//			szRetVal = "Ready"; 
//			break;
//
//		case traceStateArmed:
//			szRetVal = "Armed"; 
//			break;
//
//		case traceStateRecording:
//			szRetVal = "Recording"; 
//			break;
//
//		case traceStateStopped:
//			szRetVal = "Stopped"; 
//			break;
//
//		case traceStateUnloaded:
//			szRetVal = "Unloaded"; 
//			break;
//
//		case traceStateUnloadedData:
//			szRetVal = "UnloadedData"; 
//			break;
//
//		case traceStateFault:
//			szRetVal = "Fault"; 
//			break;
//
//		default:
//			szRetVal = "unknown state"; 
//			break;
//	}
//
//	return szRetVal;
//}

///////////////////////////////////////////////////////////////////////////////
// enable/disable softkeys
void SlExTraceSvrForm::enableSoftkey(QString szFunction, bool bEnabled)
{
	logs(Log::F, QString("SlExTraceSvrForm::enableSoftkey()1"));
	// get screen pointer
	SlGfwScreenPtr pScreen = dialog()->screen("screenMain");
	if (!pScreen.isNull())
	{
		logs(Log::F, QString("SlExTraceSvrForm::enableSoftkey()2"));
		// get softkey pointer
		QString szMenu("hu");
		QString szEtcLevel = QString::null;
		unsigned int nPos = 0;
		bool bSoftkeyFound = pScreen->functionSoftkeyPos(szFunction, QString(), szMenu, szEtcLevel, nPos);

		// set enabled/disabled
		if ( true == bSoftkeyFound )
			pScreen->setSoftkeyProperty(szMenu, szEtcLevel, nPos, "disabledSK", QVariant(!bEnabled));
		logs(Log::F, QString("SlExTraceSvrForm::enableSoftkey()3"));
	}
}

///////////////////////////////////////////////////////////////////////////
// write success or error text to the status bar
void SlExTraceSvrForm::handleRetVal(long lError, QString szSuccess)
{
	//QString			szMessage;

	//// evaluate return value
	//switch (lError)
	//{
	//	case ERR_NO_ERROR:
	//		szMessage = szSuccess;
	//		break;

	//	case ERR_ADAPTER_FAILURE:
	//		szMessage = ERR_TXT_ADAPTER_FAILURE;
	//		break;

	//	case ERR_BAD_SESSION_POINTER:
	//		szMessage = ERR_TXT_BAD_SESSION_POINTER;
	//		break;

	//	case ERR_BAD_SESS_POINTER:
	//		szMessage = ERR_TXT_BAD_SESS_POINTER;
	//		break;

	//	case ERR_CANT_OPEN_XML_FILE:
	//		szMessage = ERR_TXT_CANT_OPEN_XML_FILE;
	//		break;

	//	case ERR_CAPTURESETUP_FAILURE:
	//		szMessage = ERR_TXT_CAPTURESETUP_FAILURE;
	//		break;

	//	case ERR_NO_DATA_FAILURE:
	//		szMessage = ERR_TXT_NO_DATA_FAILURE;
	//		break;

	//	default :
	//		szMessage = SL_TRACE_ERR::SlTraceErrEnumToString((SlTraceErrEnum) lError);
	//		break;
	//}

	//// write text in status bar
	//setStatusBarElementText(szMessage, 0);  
}
