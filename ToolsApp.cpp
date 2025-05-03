#include "pch.h"  
#include "resource.h"  
#include "ResourceHelper.h"
#include "ToolsBar.h"
#include "ToolsApp.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextPanel.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		std::unique_ptr<ToolsBar> ToolsApp::toolsBar = nullptr;
		std::unique_ptr<SketchTextPanel> ToolsApp::sketchTextPanel = nullptr;

		std::map<UserLanguages, std::string> ToolsApp::localeIdMap;

		/// <summary>
		/// <para>The ToolsApp::initialize() function initializes the Implicate-X tools application</para>
		/// <para>by setting up the user interface and creating the necessary panel,</para>
		/// <para>while handling any exceptions that may occur during the process.</para>
		/// </summary>
		/// <returns>True if it succeeds, false if it fails.</returns>
        // The error "E0029: expected an expression" occurs because `IDS_MSG_APP_INITIALIZED` is likely a macro or constant that is not properly defined or included.  
        // To fix this, ensure that `IDS_MSG_APP_INITIALIZED` is defined in the included resource header file (e.g., "resource.h").  
        // If it is defined, ensure it resolves to a valid integer or string resource ID.  

        // Assuming `IDS_MSG_APP_INITIALIZED` is a resource ID, you can fix the issue by ensuring it is properly defined in "resource.h" and used correctly.  
        // Example fix:  

        bool ToolsApp::initialize() {  
           if (!userInterface())  
               return false;  

           if (!userInterface()->palettes()) {  
               return false;  
           }  

           Ptr<TextCommandPalette> textCommandPalette = userInterface()->palettes()->itemById("TextCommands");  
           if (textCommandPalette) {  
               textCommandPalette->isVisible(true);  
           }  

           toolsLocaleId = getFusion360LocaleId();  

           Application::log(toolsLocaleId);  
           Application::log(LoadStringFromResource(IDS_MSG_APP_INITIALIZED)); // Ensure IDS_MSG_APP_INITIALIZED is defined  

           if (!createBar()) {  
               Application::log(LoadStringFromResource(IDS_ERR_CREATE_BAR));  
               return false;  
           }  

           return true;  
        }  

		/// <summary>
		/// <para>The ToolsApp::terminate() function is responsible for removing the application panel</para>
		/// <para>and logging a termination message when the application is closed.</para>
		/// </summary>
		void ToolsApp::terminate() {
			removeBar();
			Application::log(LoadStringFromResource(IDS_MSG_APP_TERMINATED));
		}

		/// <summary>
		/// <para>The ToolsApp::createBar() function is responsible for initializing and displaying a panel</para>
		/// <para>for sketch text definitions, ensuring that any existing panel is removed before creating a new one.</para>
		/// </summary>
		bool ToolsApp::createBar() {
			if (toolsBar) {
				removeBar();
			}

			if (!toolsBar) {
				toolsBar = std::make_unique<ToolsBar>();
			}

			if (!toolsBar) {
				return false;
			}

			if (!toolsBar->initialize()) {
				removeBar();
				return false;
			}

			return true;
		}

		/// <summary>
		/// <para>The ToolsApp::removeBar() function is responsible for safely removing and resetting a pointer</para>
		/// <para> to a panel associated with sketch text definitions,</para>
		/// <para>ensuring that any commands related to the panel are also removed.</para>
		/// </summary>
		void ToolsApp::removeBar() {
			if (toolsBar) {
				toolsBar->terminate();
				toolsBar.reset();
				toolsBar = nullptr;
			}
		}

		bool ToolsApp::createSketchTextPanel() {
			if (sketchTextPanel) {
				removeSketchTextPanel();
			}

			if (!sketchTextPanel) {
				sketchTextPanel = std::make_unique<SketchTextPanel>();
			}

			if (!sketchTextPanel) {
				return false;
			}

			if (!sketchTextPanel->initialize()) {
				removeSketchTextPanel();
				return false;
			}

			return true;
		}

		void ToolsApp::removeSketchTextPanel() {
			if (sketchTextPanel) {
				sketchTextPanel->terminate();
				sketchTextPanel.reset();
				sketchTextPanel = nullptr;
			}
		}

		/// <summary>
		/// <para>The getFusion360LocaleId function retrieves the locale identifier for Fusion 360</para>
		/// <para>based on the user's language preferences, defaulting to "en-US" if no match is found.</para>
		/// </summary>
		///
		/// <returns>The fusion 360 locale identifier.</returns>
		std::string ToolsApp::getFusion360LocaleId() {
			localeIdMap = {};
			localeIdMap.insert({ UserLanguages::ChinesePRCLanguage,			 "zh-CN" });
			localeIdMap.insert({ UserLanguages::ChineseTaiwanLanguage,		 "zh-TW" });
			localeIdMap.insert({ UserLanguages::CzechLanguage,				 "cs-CZ" });
			localeIdMap.insert({ UserLanguages::EnglishLanguage,			 "en-US" });
			localeIdMap.insert({ UserLanguages::FrenchLanguage,				 "fr-FR" });
			localeIdMap.insert({ UserLanguages::GermanLanguage,				 "de-DE" });
			localeIdMap.insert({ UserLanguages::HungarianLanguage,			 "hu-HU" });
			localeIdMap.insert({ UserLanguages::ItalianLanguage,			 "it-IT" });
			localeIdMap.insert({ UserLanguages::JapaneseLanguage,			 "ja-JP" });
			localeIdMap.insert({ UserLanguages::KoreanLanguage,				 "ko-KR" });
			localeIdMap.insert({ UserLanguages::PolishLanguage,				 "pl-PL" });
			localeIdMap.insert({ UserLanguages::PortugueseBrazilianLanguage, "pt-BR" });
			localeIdMap.insert({ UserLanguages::RussianLanguage,			 "ru-RU" });
			localeIdMap.insert({ UserLanguages::SpanishLanguage,			 "es-ES" });
			localeIdMap.insert({ UserLanguages::TurkishLanguage,			 "tr-TR" });

			std::string localeId = "en-US"; // Default to English

			Ptr<Preferences> preferences = this->preferences();
			if (preferences) {
				Ptr<GeneralPreferences> generalPreferences = preferences->generalPreferences();
				if (generalPreferences) {
					UserLanguages userLanguage = generalPreferences->userLanguage();
					if (localeIdMap.find(userLanguage) != localeIdMap.end()) {
						localeId = localeIdMap.at(userLanguage);// .substr(0, 2);
					}
				}
			}

			return localeId;
		}

		/// <summary>
		/// <para>The getLanguageRegionNames function retrieves language and region names</para>
		/// <para>based on a specified localeand populates a provided map with the results.</para>
		/// </summary>
		///
		/// <param name="selectedLocale">		   The selected locale.</param>
		/// <param name="localeLanguageRegionMap">[in,out] The locale language region map.</param>
		void ToolsApp::getLanguageRegionNames(std::string selectedLocale, std::map<std::string,std::string>& localeLanguageRegionMap) {
			UErrorCode status = U_ZERO_ERROR;
			UnicodeString result;
			Locale localeBuilder;
			std::string languageCode;
			std::string regionCode;
			std::string selectedCode = selectedLocale.substr(0, 2);;
			std::string languageRegionName;
			
			localeLanguageRegionMap.clear();

			for (const auto& [index, locale] : ToolsApp::localeIdMap) {
				languageCode = locale.substr(0, 2);
				regionCode = locale.substr(3, 2);
				localeBuilder =
					LocaleBuilder()
					.setLanguage(languageCode.c_str())
					.setRegion(regionCode.c_str())
					.build(status);
				if (U_SUCCESS(status)) {
					localeBuilder.getDisplayName(Locale::forLanguageTag(selectedCode.c_str(), status), result);
					result.toUTF8String(languageRegionName);
					localeLanguageRegionMap.insert({ locale, languageRegionName });
				}
				else {
					Application::log("Error creating LocaleBuilder: " + std::to_string(status));
				}

				languageRegionName.clear();
			}
		}

		/// <summary>
		/// <para>The getLanguageRegionNamesSorted function retrieves and sorts a list of language-region pairs</para>
		/// <para>based on the specified locale, populating the provided vector with the sorted results.</para>
		/// </summary>
		///
		/// <param name="selectedLocale">		    The selected locale.</param>
		/// <param name="localeLanguageRegionMap">[in,out] The locale language region map.</param>
		void ToolsApp::getLanguageRegionNamesSorted(std::string selectedLocale, std::vector<std::pair<std::string, std::string>>& sortedLocaleLanguageRegionList) {
			std::map<std::string, std::string> localeLanguageRegionMap;

			getLanguageRegionNames(selectedLocale, localeLanguageRegionMap);

			sortedLocaleLanguageRegionList.assign(localeLanguageRegionMap.begin(), localeLanguageRegionMap.end());

			std::sort(sortedLocaleLanguageRegionList.begin(), sortedLocaleLanguageRegionList.end(), [](const auto& a, const auto& b) {
				return a.second < b.second;
			});
		}
	}
}
