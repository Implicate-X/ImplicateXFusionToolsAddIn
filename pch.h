#pragma once
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <Fusion/Sketch/SketchTexts.h>
#include <Fusion/Sketch/SketchText.h>
#include <Cam/CamAll.h>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <cmath>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <memory>
#include <algorithm>
#include <regex>
// Ensure ICU library is properly included
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4635)
#endif
#include <unicode/utypes.h>
#include <unicode/locid.h>
#include <unicode/ustream.h>
#include <unicode/locdspnm.h>
#include <unicode/localebuilder.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
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
#define LOG_ERROR(msg) \
    toolsApp->log(std::string("[ERROR] ") + msg + " | Function: " + __func__ + " | File: " + __FILE__ + " | Line: " + std::to_string(__LINE__), ErrorLogLevel, LOG_TYPE)
#endif

#include "Symbols.h"
