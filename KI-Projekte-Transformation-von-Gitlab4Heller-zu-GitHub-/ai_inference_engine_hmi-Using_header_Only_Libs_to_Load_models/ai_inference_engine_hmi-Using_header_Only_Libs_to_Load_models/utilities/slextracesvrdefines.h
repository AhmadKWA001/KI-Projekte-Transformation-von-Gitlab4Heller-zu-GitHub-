///////////////////////////////////////////////////////////////////////////////
/*! \file   slextracesvrdefines.h
 *  \brief  Header file that contains trace server sample defines
 *
 *
 *  (C) Copyright Siemens AG 2007.
 */
///////////////////////////////////////////////////////////////////////////////
//
//#define CLIENT_NAME			"SampleClient"
//#define SESSION_PASSWORD	"SamplePassword"     
//#define SESSION_NAME		"SampleSession"

// maximum size of data we would like to read from trace service
#define MAX_READ_DATA_SIZE				1000000			// maximum data characters for each XML data requests
#define MAX_LOG_FILE_SIZE		1000000
#define FILTER			1111111111111111
//enum enumTraceClientErrors
//{
//	ERR_NO_ERROR = 0,						// Action performed successfully
//	ERR_CANT_OPEN_XML_FILE,				// Error opening the capture setup XML file 
//	ERR_BAD_SESS_POINTER,					// Invalid null session connection pointer returned
//	ERR_ADAPTER_FAILURE,					// An adapter pointer not acquired, check for trace service successful startup
//	ERR_BAD_SESSION_POINTER,				// Unexpected null pointer value returned for creating Session
//	ERR_CAPTURESETUP_FAILURE,				// CaptureSetup error for session
//	ERR_NO_DATA_FAILURE,					// Failure to session's data present
//};

#define ERR_TXT_ADAPTER_FAILURE			"Adapter pointer not acquired, check for trace service successful startup"
#define ERR_TXT_BAD_SESSION_POINTER		"Unexpected null pointer value returned for creating session"
#define ERR_TXT_BAD_SESS_POINTER		"Invalid null session connection pointer returned"
#define ERR_TXT_CANT_OPEN_XML_FILE		"Error opening the capture setup XML file"
#define ERR_TXT_CAPTURESETUP_FAILURE	"Error in capture setup XML"
#define ERR_TXT_NO_DATA_FAILURE			"No trace data available"

#define SUCC_SESSION_CREATE				"Creating new session successful"
#define SUCC_CAPTURE_SETUP_PUT			"Reading capture setup from sample.xml and putting it to the existing session successful"
#define SUCC_TRACE_START				"Starting the existing trace session successful. Session is logging data, please wait ..."
#define SUCC_TRACE_STOP					"Stopping the existing session successful"
#define SUCC_DATA_TRACED				"Traced data retrieved successfully"
#define SUCC_DATA_CLEAR					"Clearing trace data successful"
#define SUCC_SESSION_CLOSE				"Closing the existing session successful"
#define SUCC_SESSION_DELETE				"Deleting the existing session successful"
#define LOG_DIR				"ai_engine"
#define LOG_FN				"log.txt"
#define APPLPATH			"../../../../oem/sinumerik/hmi/appl"
#define USERPATH			"../../../../user/sinumerik/hmi/appl"
