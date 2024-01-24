/**
 * A basic log handling program for lightwm the open source window managment program for Windows
 * Written By Demetry Romanowski
 * demetryromanowski@gmail.com
 * 
 * The program will create a temp file in the windows temp directory to dump log files into depending on 
 * the log level set. 
**/ 
#include <Windows.h> 
#include <assert.h> 

#include "logger.h" 


wchar_t* TempDirectoryPath = NULL; 

/**
 * Function: InitializeLogger
 * ----------------------------
 * Initializes a new log file and has it ready for reading 
 *
 * @param LogLevel: Default LOG_LEVEL_ALL
 * @returns 1 on success 0 on fail
 **/
uint8_t InitializeLogger(uint8_t logLevel)
{
	DWORD nTempDirPathLen = GetTempPath2W(1, TempDirectoryPath); 
	
	if(nTempDirPathLen == 0) { 
		//An error has occurred getting a temp file path
		//TODO Handle this error
		return 0; 
	}
	
	TempDirectoryPath = (wchar_t*)malloc(sizeof(wchar_t) * nTempDirPathLen); 
	DWORD pathLen = GetTempPath2W(nTempDirPathLen, TempDirectoryPath); 
	
	assert(pathLen == nTempDirPathLen); 
	
	
	
}

/**
 * Function: LogWriteLineW
 * -----------------------
 * Write a wide char string to the log file
 *
 * @param message: The message to append to the log file
 * @param loglevel: Default LOG_LEVEL_ALL
 * returns 1 on success 0 on fail
 **/
uint8_t LogWriteLineW(wchar_t* message, uint8_t logLevel)
{
	
}

uint8_t LogWriteW(wchar_t* message, uint8_t logLevel)
{
	
}

uint8_t LogWriteLine(char* message, uint8_t logLevel)
{
	
} 

uint8_t LogWrite(char* message, uint8_t logLevel)
{
	
}

void SetLogLevel(uint8_t logLevel)
{
	
}

uint8_t DestroyLogger()
{
	
}

uint8_t DeleteOldLogs()
{
	
}
