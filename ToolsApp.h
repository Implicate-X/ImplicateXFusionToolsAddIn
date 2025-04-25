#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace icu;

#include "ToolsBar.h"

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>The ToolsApp class in the implicatex::fusion namespace is designed to manage text definition settings in sketches,</para>
		/// <para>providing methods for initialization, termination, panel creation, and removal.</para>
		/// </summary>
		class ToolsApp :
			public Application
		{
		public:
			static std::unique_ptr<ToolsBar> toolsBar;
			static std::map<UserLanguages, std::string> LocaleIdMap;

			/// <summary>
			/// <para>The ToolsApp::initialize() function initializes the Implicate-X tools application</para>
			/// <para>by setting up the user interface and creating the necessary panel,</para>
			/// <para>while handling any exceptions that may occur during the process.</para>
			/// </summary>
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool initialize();

			/// <summary>
			/// <para>The ToolsApp::terminate() function is responsible for removing the application panel</para>
			/// <para>and logging a termination message when the application is closed.</para>
			/// </summary>
			void terminate();

			/// <summary>
			/// <para>The ToolsApp::createBar() function is responsible for initializing and displaying a panel</para>
			/// <para>for sketch text definitions, ensuring that any existing panel is removed before creating a new one.</para>
			/// </summary>
			bool createBar();

			/// <summary>
			/// <para>The ToolsApp::removeBar() function is responsible for safely removing and resetting a pointer</para>
			/// <para> to a panel associated with sketch text definitions,</para>
			/// <para>ensuring that any commands related to the panel are also removed.</para>
			/// </summary>
			void removeBar();

			/// <summary>
			/// <para>The getFusion360Language() function retrieves the user's preferred locale id for Fusion 360,</para>
			/// <para>returning a corresponding language code based on predefined mappings.</para>
			/// </summary>
			/// <returns>The fusion 360 user interface language code eg. 'en','de','fr'...</returns>
			std::string getFusion360LocaleId();

			/// <summary>
			/// <para>The GetLanguageRegionNames function retrieves language and region names</para>
			/// <para>based on a specified localeand populates a provided map with the results.</para>
			/// </summary>
			///
			/// <param name="selectedLocale">		   The selected locale.</param>
			/// <param name="localeLanguageRegionMap">[in,out] The locale language region map.</param>
			void getLanguageRegionNames(std::string selectedLocale, std::map<std::string,std::string>& localeLanguageRegionMap);

			/// <summary>
			/// <para>The GetLanguageRegionNamesSorted function retrieves and sorts a list of language-region pairs</para>
			/// <para>based on the specified locale, populating the provided vector with the sorted results.</para>
			/// </summary>
			///
			/// <param name="selectedLocale">		   The selected locale.</param>
			/// <param name="localeLanguageRegionMap">[in,out] The locale language region map.</param>
			void getLanguageRegionNamesSorted(std::string selectedLocale, std::vector<std::pair<std::string, std::string>>& sortedLocaleLanguageRegionList);
		};
	}
}
