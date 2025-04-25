#pragma once
#include <Windows.h>
#include "PrebuildMacros.h" /// Built by prebuild task: constexpr auto MODULE_NAME = L"SketchTextDefinitionsAddIn.dll";
#include "ToolsApp.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace icu;

namespace implicatex {
    namespace fusion {
        extern Ptr<ToolsApp> gApp;
        extern Ptr<UserInterface> gUi;

        /// <summary>LocaleID holds the default locale identifier, initialized to "en-US" for English (Unites States).</summary>
        extern std::string gLocaleId;

		const std::string IDS_SUBDIR_RESOURCES = "Resources/"; // Resources/
		const std::string IDS_ID_SOLID_ENV = "FusionSolidEnvironment"; // FusionSolidEnvironment
		const std::string IDS_ID_TOOLS_BAR = "ImplicateXFusionToolsAddInBar"; // ImplicateXFusionToolsAddInBar
		const std::string IDS_ID_TOOLS_RES = "Resources/logo"; // Resources/logo
		const std::string IDS_NAME_TOOLS_BAR = "Implicate-X"; // Implicate-X
		const std::string IDS_CMD_IMPLICATEX = "ImplicateXFusionToolsCommand"; // ImplicateXFusionToolsCommand
		const std::string IDS_ID_LANG_SELECTOR = "LanguageDropDownControl"; // LanguageDropDownControl
		const std::string IDS_CMD_LANG_SELECTOR = "LanguageDropDownCommand"; // LanguageDropDownCommand

        inline std::string LoadStringFromResource(UINT resourceId) {
            HMODULE hModule = GetModuleHandle(MODULE_NAME);
            wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
            size_t convertedChars = 0;
            mbstowcs_s(&convertedChars, localeName, gLocaleId.c_str(), gLocaleId.size() + 1);

            LANGID langId;
            wchar_t langIdStr[LOCALE_NAME_MAX_LENGTH];
            if (GetLocaleInfoEx(localeName, LOCALE_ILANGUAGE, langIdStr, LOCALE_NAME_MAX_LENGTH) == 0) {
                langId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US); // Fallback to English
            }
            else {
                langId = static_cast<LANGID>(wcstol(langIdStr, nullptr, 16));
            }

            HRSRC hResInfo = FindResourceEx(hModule, RT_STRING, MAKEINTRESOURCE((resourceId / 16) + 1), langId);
            if (hResInfo) {
                HGLOBAL hResData = LoadResource(hModule, hResInfo);
                if (hResData) {
                    const wchar_t* pRes = static_cast<const wchar_t*>(LockResource(hResData));
                    if (pRes) {
                        for (unsigned int i = 0; i < (resourceId & 15); ++i) {
                            pRes += *pRes + 1;
                        }
                        std::wstring wstr(pRes + 1, *pRes);
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
                        std::string str(size_needed, 0);
                        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
                        return str;
                    }
                }
            }
            return "";
        }

        /// <summary>
        /// <para>The LoadStringFromResource function retrieves a string resource from the application's module</para>
        /// <para>based on the specified resource ID, converting it from wide character format to a UTF-8 encoded std::string.</para>
        /// </summary>
        /// <param name="resourceId">Identifier for the resource.</param>
        /// <returns>The string from resource.</returns>
        inline std::string GetLocaleInfoAsString(const wchar_t* localeName, LCTYPE lcType) {
            wchar_t buffer[256];
            if (GetLocaleInfoEx(localeName, lcType, buffer, 256)) {
                int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                if (sizeNeeded > 0) {
                    std::string result(sizeNeeded, 0);
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], sizeNeeded, nullptr, nullptr);
                    return result;
                }
            }
            return "";
        }

        inline std::string WideCharToUtf8(const wchar_t* wideString) {
            if (!wideString) {
                return "";
            }

            int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, nullptr, 0, nullptr, nullptr);
            if (sizeNeeded <= 0) {
                return "";
            }
            std::string utf8String(sizeNeeded - 1, 0); // -1 to exclude the null terminator  
            WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &utf8String[0], sizeNeeded, nullptr, nullptr);
            return utf8String;
        }

        inline std::string GetLocalizedLanguageName(const wchar_t* sourceLocale, const wchar_t* targetLocale) {
            wchar_t buffer[256];
            if (GetLocaleInfoEx(sourceLocale, LOCALE_SLOCALIZEDLANGUAGENAME, buffer, 256)) {
                // Konvertiere den Namen in die Ziel-Locale
                wchar_t translatedBuffer[256];
                if (GetLocaleInfoEx(targetLocale, LOCALE_SLOCALIZEDLANGUAGENAME, translatedBuffer, 256)) {
                    return WideCharToUtf8(translatedBuffer);
                }
            }
            return "";
        }
    }
}
