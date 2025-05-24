#pragma once
#ifdef _LOG_FILE_
#define LOG_TYPE LogTypes::FileLogType
#else
#define LOG_TYPE LogTypes::ConsoleLogType
#endif

#ifdef _LOG_INFO_
#define LOG_INFO(msg) toolsApp->log(msg, InfoLogLevel, LOG_TYPE)
#define LOG_INFO_EX(msg) \
    toolsApp->log(msg + " | Function: " + __func__ + " | File: " + __FILE__ + " | Line: " + std::to_string(__LINE__), InfoLogLevel, LOG_TYPE)
#else
#define LOG_INFO(msg) ((void)0)
#define LOG_INFO_EX(msg) ((void)0)
#endif

#ifndef LOG_ERROR
/// <summary>
/// <para>LOG_ERROR is a macro that logs an error message along with the last error details, </para>
/// <para>the current function name, file name, and line number to a logging system.</para>
/// </summary>
///
/// <param name="msg">The message.</param>
#define LOG_ERROR(msg) \
    std::string path = __FILE__; \
    std::string fileName = path.substr(path.find_last_of("/\\") + 1); \
    std::string errMsg; \
    toolsApp->getLastError(&errMsg); \
    toolsApp->log(std::string("[ERROR] ") + msg + " | " + errMsg + " | Function: " + __func__ + " | File: " + fileName + " | Line: " + std::to_string(__LINE__), ErrorLogLevel, LOG_TYPE)
#endif
