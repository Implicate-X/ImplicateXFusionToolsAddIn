#include "pch.h"
#include "resource.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "PrebuildMacros.h" /// Built by prebuild task: constexpr auto MODULE_NAME = L"ImplicateXFusionToolsAddIn.dll";
#include "ResourceHelper.h"

#define ICU77

namespace implicatex {
    namespace fusion {
        /// <summary>
        /// <para>The LoadStringFromResource function retrieves a string resource from the application's module </para>
        /// <para>based on the specified resource ID and the current locale, </para>
        /// <para>converting it from wide character format to a standard UTF-8 encoded std::string.</para>
        /// </summary>
        ///
        /// <param name="resourceId">
        /// <para>is an unsigned integer that typically represents a unique identifier for a resource, </para>
        /// <para>such as a graphical element or a string, used in applications or libraries.</para>
        /// </param>
        ///
        /// <returns>The string from resource.</returns>
        std::string LoadStringFromResource(UINT resourceId) {
            HMODULE hModule = GetModuleHandle(MODULE_NAME);
            wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
            size_t convertedChars = 0;
            mbstowcs_s(&convertedChars, localeName, toolsLocaleId.c_str(), toolsLocaleId.size() + 1);

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

                        #ifndef ICU77
                        std::wstring wstr(pRes + 1, *pRes);
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
                        std::string str(size_needed, 0);
                        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
                        return str;
                        #else
                        icu::UnicodeString unicodeStr(pRes + 1, *pRes);
                        std::string utf8String;
                        unicodeStr.toUTF8String(utf8String);
                        return utf8String;
                        #endif
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
        std::string GetLocaleInfoAsString(const wchar_t* localeName, LCTYPE lcType) {
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

        /// <summary>
        /// <para>The WideCharToUtf8 function converts a wide character string (UTF-16) to a UTF-8 encoded std::string, </para>
        /// <para>returning an empty string if the input is null or if the conversion fails.</para>
        /// </summary>
        ///
        /// <param name="wideString">The wide string.</param>
        ///
        /// <returns>A std::string.</returns>
        std::string WideCharToUtf8(const wchar_t* wideString) {
            if (!wideString) {
                return "";
            }
	        icu::UnicodeString unicodeStr(wideString);
            std::string utf8String;
            unicodeStr.toUTF8String(utf8String);
            return utf8String;
        }

        /// <summary>
        /// <para>The GetLocalizedLanguageName function retrieves the localized name of a language </para>
        /// <para>based on the specified source and target locale identifiers, converting the result to a UTF-8 encoded string.</para>
        /// </summary>
        ///
        /// <param name="sourceLocale">Source locale.</param>
        /// <param name="targetLocale">Target locale.</param>
        ///
        /// <returns>The localized language name.</returns>
        std::string GetLocalizedLanguageName(const wchar_t* sourceLocale, const wchar_t* targetLocale) {
            wchar_t buffer[256];
            if (GetLocaleInfoEx(sourceLocale, LOCALE_SLOCALIZEDLANGUAGENAME, buffer, 256)) {
                wchar_t translatedBuffer[256];
                if (GetLocaleInfoEx(targetLocale, LOCALE_SLOCALIZEDLANGUAGENAME, translatedBuffer, 256)) {
                    return WideCharToUtf8(translatedBuffer);
                }
            }
            return "";
        }
    }
}
