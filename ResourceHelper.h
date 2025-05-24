#pragma once
#include <Windows.h>

namespace implicatex {
    namespace fusion {

        /// <summary>
        /// <para>The LoadStringFromResource function retrieves a string resource from the application's module </para>
        /// <para>based on the specified resource ID and the current locale, </para>
        /// <para>converting it from wide character format to a standard UTF-8 encoded std::string.</para>
        /// </summary>
        ///
        /// <param name="resourceId">
        /// <para>resourceId is an unsigned integer that typically represents a unique identifier for a resource, </para>
        /// <para>such as a graphical element or a string, used in applications or libraries.</para>
        /// </param>
        ///
        /// <returns>The string from resource.</returns>
        std::string LoadStringFromResource(UINT resourceId);

        /// <summary>
        /// <para>The LoadStringFromResource function retrieves a string resource from the application's module</para>
        /// <para>based on the specified resource ID, converting it from wide character format to a UTF-8 encoded std::string.</para>
        /// </summary>
        /// <param name="resourceId">Identifier for the resource.</param>
        /// <returns>The string from resource.</returns>
        std::string GetLocaleInfoAsString(const wchar_t* localeName, LCTYPE lcType);

        /// <summary>
        /// <para>The WideCharToUtf8 function converts a wide character string (UTF-16) to a UTF-8 encoded std::string, </para>
        /// <para>returning an empty string if the input is null or if the conversion fails.</para>
        /// </summary>
        ///
        /// <param name="wideString">The wide string.</param>
        ///
        /// <returns>A std::string.</returns>
        std::string WideCharToUtf8(const wchar_t* wideString);

        /// <summary>
        /// <para>The GetLocalizedLanguageName function retrieves the localized name of a language </para>
        /// <para>based on the specified source and target locale identifiers, converting the result to a UTF-8 encoded string.</para>
        /// </summary>
        ///
        /// <param name="sourceLocale">Source locale.</param>
        /// <param name="targetLocale">Target locale.</param>
        ///
        /// <returns>The localized language name.</returns>
        std::string GetLocalizedLanguageName(const wchar_t* sourceLocale, const wchar_t* targetLocale);
    }
}
