#include "pch.h"  
#include "resource.h"  
#include "ToolsBar.h"
#include "ToolsApp.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ResourceHelper.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		std::unique_ptr<ToolsBar> ToolsApp::sToolsBar = nullptr;

		std::map<UserLanguages, std::string> ToolsApp::sLocaleIdMap;

		/// <summary>
		/// <para>The ToolsApp::initialize() function initializes the Implicate-X tools application</para>
		/// <para>by setting up the user interface and creating the necessary panel,</para>
		/// <para>while handling any exceptions that may occur during the process.</para>
		/// </summary>
		/// <returns>True if it succeeds, false if it fails.</returns>
		/// 
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

			gLocaleId = getFusion360LocaleId();

			Application::log(gLocaleId);
			Application::log(LoadStringFromResource(IDS_MSG_APP_INITIALIZED));

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
		/// 
		void ToolsApp::terminate() {
			removeBar();
			Application::log(LoadStringFromResource(IDS_MSG_APP_TERMINATED));
		}

		/// <summary>
		/// <para>The ToolsApp::createBar() function is responsible for initializing and displaying a panel</para>
		/// <para>for sketch text definitions, ensuring that any existing panel is removed before creating a new one.</para>
		/// </summary>
		///
		bool ToolsApp::createBar() {
			if (sToolsBar) {
				removeBar();
			}

			if (!sToolsBar) {
				sToolsBar = std::make_unique<ToolsBar>();
			}

			if (!sToolsBar) {
				return false;
			}

			if (!sToolsBar->initialize()) {
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
		/// 
		void ToolsApp::removeBar() {
			if (sToolsBar) {
				sToolsBar->terminate();
				sToolsBar.reset();
				sToolsBar = nullptr;
			}
		}

		/// <summary>
		/// <para>The getFusion360LocaleId function retrieves the locale identifier for Fusion 360</para>
		/// <para>based on the user's language preferences, defaulting to "en-US" if no match is found.</para>
		/// </summary>
		///
		/// <returns>The fusion 360 locale identifier.</returns>
		/// 
		std::string ToolsApp::getFusion360LocaleId() {
			sLocaleIdMap = {};
			sLocaleIdMap.insert({ UserLanguages::ChinesePRCLanguage,			 "zh-CN" });
			sLocaleIdMap.insert({ UserLanguages::ChineseTaiwanLanguage,		 "zh-TW" });
			sLocaleIdMap.insert({ UserLanguages::CzechLanguage,				 "cs-CZ" });
			sLocaleIdMap.insert({ UserLanguages::EnglishLanguage,			 "en-US" });
			sLocaleIdMap.insert({ UserLanguages::FrenchLanguage,				 "fr-FR" });
			sLocaleIdMap.insert({ UserLanguages::GermanLanguage,				 "de-DE" });
			sLocaleIdMap.insert({ UserLanguages::HungarianLanguage,			 "hu-HU" });
			sLocaleIdMap.insert({ UserLanguages::ItalianLanguage,			 "it-IT" });
			sLocaleIdMap.insert({ UserLanguages::JapaneseLanguage,			 "ja-JP" });
			sLocaleIdMap.insert({ UserLanguages::KoreanLanguage,				 "ko-KR" });
			sLocaleIdMap.insert({ UserLanguages::PolishLanguage,				 "pl-PL" });
			sLocaleIdMap.insert({ UserLanguages::PortugueseBrazilianLanguage, "pt-BR" });
			sLocaleIdMap.insert({ UserLanguages::RussianLanguage,			 "ru-RU" });
			sLocaleIdMap.insert({ UserLanguages::SpanishLanguage,			 "es-ES" });
			sLocaleIdMap.insert({ UserLanguages::TurkishLanguage,			 "tr-TR" });

			std::string localeId = "en-US"; // Default to English

			Ptr<Preferences> preferences = this->preferences();
			if (preferences) {
				Ptr<GeneralPreferences> generalPreferences = preferences->generalPreferences();
				if (generalPreferences) {
					UserLanguages userLanguage = generalPreferences->userLanguage();
					if (sLocaleIdMap.find(userLanguage) != sLocaleIdMap.end()) {
						localeId = sLocaleIdMap.at(userLanguage);// .substr(0, 2);
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
		/// 
		void ToolsApp::getLanguageRegionNames(std::string selectedLocale, std::map<std::string,std::string>& localeLanguageRegionMap) {
			UErrorCode status = U_ZERO_ERROR;
			UnicodeString result;
			Locale localeBuilder;
			std::string languageCode;
			std::string regionCode;
			std::string selectedCode = selectedLocale.substr(0, 2);;
			std::string languageRegionName;
			
			localeLanguageRegionMap.clear();

			for (const auto& [index, locale] : ToolsApp::sLocaleIdMap) {
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
		/// <param name="selectedLocale">		   The selected locale.</param>
		/// <param name="localeLanguageRegionMap">[in,out] The locale language region map.</param>
		/// 
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
