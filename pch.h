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
#include "Symbols.h"
