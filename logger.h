/**
 * A basic log handling program for lightwm the open source window managment program for Windows
 * Written By Demetry Romanowski
 * demetryromanowski@gmail.com
 * 
 * The program will create a temp file in the windows temp directory to dump log files into depending on 
 * the log level set. 
**/ 

#include <stdint.h> 
#include <wchar.h> 

#ifndef __LOGGER_H
#define __LOGGER_H

#define LOG_LEVEL_ALL 0 
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_MESSAGE 4
//...
#define LOG_LEVEL_NONE 15

uint8_t InitializeLogger(uint8_t logLevel); 
uint8_t LogWriteLineW(wchar_t* message, uint8_t logLevel); 
uint8_t LogWriteW(wchar_t* message, uint8_t logLevel); 
uint8_t LogWriteLine(char* message, uint8_t logLevel); 
uint8_t LogWrite(char* message, uint8_t logLevel); 

void SetLogLevel(uint8_t logLevel); 

uint8_t DestroyLogger(); 
uint8_t DeleteOldLogs(); 

#endif