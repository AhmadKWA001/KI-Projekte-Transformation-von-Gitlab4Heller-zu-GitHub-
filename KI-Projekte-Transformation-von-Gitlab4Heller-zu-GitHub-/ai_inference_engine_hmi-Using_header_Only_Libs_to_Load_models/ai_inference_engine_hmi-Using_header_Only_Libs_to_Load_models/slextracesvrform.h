///////////////////////////////////////////////////////////////////////////////
/*! \file	slextracesvrform.h
 *  \brief	Class Definition file for class SlExTraceSvrForm
 *
 *  This file is part of the HMI Solutionline Programming Package.
 *
 *  (C) Copyright Siemens AG A&D MC 2007. All rights reserved.
 */
///////////////////////////////////////////////////////////////////////////////

#ifndef SL_EXTRACESVRFORM_H
#define SL_EXTRACESVRFORM_H
#pragma once
#include "AI_Service.h"
#include "slgfwdialogform.h"
#include "sltrace.h"
#include "slextracesvrdefines.h"
#include "log.h"


class SlGfwLineEdit;
class QTextBrowser;

/*! \class SlExTraceSvrForm slextracesvrform.h
 *  \brief form class 
 */
class SlExTraceSvrForm : public SlGfwDialogForm
{
	Q_OBJECT;

	///////////////////////////////////////////////////////////////////////////
    // CREATORS
    ///////////////////////////////////////////////////////////////////////////
public:

	/*! \fn SlExTraceSvrForm(QWidget* pParent = 0, const QString& rszName = QString::null)
	 *  \param pParent		parent widget
     *  \param rszName		object name
     *
	 *  Constructor
	*/
	AI_Service* ai_Service = NULL;
	SlExTraceSvrForm(QWidget* pParent = 0, const QString& rszName = QString::null);

	/*! \fn ~SlExTraceSvrForm(void)
     *
     *  destructor
    */
    ~SlExTraceSvrForm(void);


	///////////////////////////////////////////////////////////////////////////////
	// OVERWRITTEN METHODS
	///////////////////////////////////////////////////////////////////////////////
private:

	/*! \fn virtual void attachedToScreen(const QString& rszScreenName);
     *  \param rszScreenName the attached screen
     *
     *  a screen was attached
     */
    void attachedToScreen(const QString& rszScreenName);

	/*! \fn void onFunction(const QString& rszFunction, const QString& rszArgs, bool& rbHandled);
     *  \param rszFunction	function name
     *  \param rszArgs		function arguments
     *  \param rbHandled	true, if the method handled the function.
     * 
     *  Will be called by the framework, if a function should be executed, e.g. if a softkey
     *  should execute a command
    */
    void onFunction(const QString& /*rszFunction*/, const QString& /*rszArgs*/, bool& /*rbHandled*/);


	///////////////////////////////////////////////////////////////////////////
    // PRIVATE METHODS
    ///////////////////////////////////////////////////////////////////////////
private:
	/*! \fn long CreateSession()
	 * retVal long			Null, if the command was successful
	 *
	 * it creates a session
    */	
	long CreateSession();

	/*! \fn long CaptureSetup()
	 * retVal long			Null, if the command was successful
	 *
	 * it captures a setup from a file
    */	
	//long CaptureSetup();

	/*! \fn long StartTrace()
	 * retVal long			Null, if the command was successful
	 *
	 * it starts the trace
    */
	//long StartTrace();

	/*! \fn long StopTrace()
	 * retVal long			Null, if the command was successful
	 *
	 * it stops the trace
    */	
	//long StopTrace();

	/*! \fn long TracedData()
	 * retVal long			Null, if the command was successful
	 *
	 * it gets trace data 
    */	
	long TracedData();

	/*! \fn long ClearData()
	 * retVal long			Null, if the command was successful
	 *
	 * it clears trace data 
    */	
	//long ClearData();

	/*! \fn long CloseSession()
	 * retVal long			Null, if the command was successful
	 *
	 * it closes the session
    */	
	//long CloseSession();

	/*! \fn long DeleteSession()
	 * retVal long			Null, if the command was successful
	 *
	 * it deletes the session
    */	
	long DeleteSession();


	///////////////////////////////////////////////////////////////////////////
    // PRIVATE SLOTS
    ///////////////////////////////////////////////////////////////////////////
	/*! \fn void OnDataReady(ulong Intervals)
	 *
	 * is send, if data is provided
    */	
	//void OnDataReady(ulong Intervals);

	/*! \fn void OnStateChange(TraceSessStateEnum SessionState, SlTraceErrEnum Error)
	 *
	 * is send, if state has changed
    */	
	//void OnStateChange(TraceSessStateEnum SessionState, SlTraceErrEnum Error);
public slots:
	void OnTextChange(QString newText= "");

	///////////////////////////////////////////////////////////////////////////
    // PRIVATE HELPERS
    ///////////////////////////////////////////////////////////////////////////
private:
	/*! \fn QString getStateAsText(TraceSessStateEnum SessionState)
	 *  \param SessionState		session state
	 *  \retVal 				session state as a QString (for display)
	 *
	 * converts a session state to a QString (for display it in StatusBar)
    */	
	//QString getStateAsText(TraceSessStateEnum SessionState);

	/*! \fn void enableSoftkey(QString szFunction, bool bEnabled)
	 *  \param szFunction		functionname of the softkey
	 *  \param bEnabled			if true, the softkey will be enable
	 *
	 * enable/disable softkeys
    */	
	void enableSoftkey(QString szFunction, bool bEnabled);

	/*! \fn void handleRetVal(long lError, QString szSuccess)
     *  \param lError			error value from performed action
	 *  \param szSuccess		show this text, if no error occurs
     * 
     *  write success or error text to the status bar
    */
	void handleRetVal(long lError, QString szSuccess);

	///////////////////////////////////////////////////////////////////////////
    // PRIVATE MEMBERS
    ///////////////////////////////////////////////////////////////////////////
private:
	/*! pointer for widgets */
	SlGfwLineEdit*			m_pLeStatus;
    QTextBrowser*			m_pTbDisplayedXml;

	/*! SlTrace objects */
	//SlTraceAdapterPtr		m_pSLTraceAdapter;			// smart pointer to TraceService adaptor provides access to trace service
	//SlTraceQSessConn*		m_pISessConn;				// session connection pointer for calling session methods

	/*! SlTrace data */
	QString					m_szSaveDataXML;			// used for output data in XML format
};

#endif // SL_EXTRACESVRFORM_H

//extern SlExTraceSvrForm* mainform;