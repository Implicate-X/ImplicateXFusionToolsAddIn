#include "pch.h"  
#include "resource.h"  
#include "ToolsBar.h"
#include "ToolsApp.h"  
#include "ImplicateXFusionToolsAddIn.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		std::unique_ptr<ToolsBar> ToolsApp::toolsBar = nullptr;

		std::map<UserLanguages, std::string> ToolsApp::LocaleIdMap;

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

		void ToolsApp::terminate() {
			removeBar();
			Application::log(LoadStringFromResource(IDS_MSG_APP_TERMINATED));
		}

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

		void ToolsApp::removeBar() {
			if (toolsBar) {
				toolsBar->terminate();
				toolsBar.reset();
				toolsBar = nullptr;
			}
		}

		/// <summary>
		/// <para>The getFusion360LocaleId function retrieves the locale identifier for Fusion 360</para>
		/// <para>based on the user's language preferences, defaulting to "en-US" if no match is found.</para>
		/// </summary>
		///
		/// <returns>The fusion 360 locale identifier.</returns>
		std::string ToolsApp::getFusion360LocaleId() {
			LocaleIdMap = {};
			LocaleIdMap.insert({ UserLanguages::ChinesePRCLanguage,			 "zh-CN" });
			LocaleIdMap.insert({ UserLanguages::ChineseTaiwanLanguage,		 "zh-TW" });
			LocaleIdMap.insert({ UserLanguages::CzechLanguage,				 "cs-CZ" });
			LocaleIdMap.insert({ UserLanguages::EnglishLanguage,			 "en-US" });
			LocaleIdMap.insert({ UserLanguages::FrenchLanguage,				 "fr-FR" });
			LocaleIdMap.insert({ UserLanguages::GermanLanguage,				 "de-DE" });
			LocaleIdMap.insert({ UserLanguages::HungarianLanguage,			 "hu-HU" });
			LocaleIdMap.insert({ UserLanguages::ItalianLanguage,			 "it-IT" });
			LocaleIdMap.insert({ UserLanguages::JapaneseLanguage,			 "ja-JP" });
			LocaleIdMap.insert({ UserLanguages::KoreanLanguage,				 "ko-KR" });
			LocaleIdMap.insert({ UserLanguages::PolishLanguage,				 "pl-PL" });
			LocaleIdMap.insert({ UserLanguages::PortugueseBrazilianLanguage, "pt-BR" });
			LocaleIdMap.insert({ UserLanguages::RussianLanguage,			 "ru-RU" });
			LocaleIdMap.insert({ UserLanguages::SpanishLanguage,			 "es-ES" });
			LocaleIdMap.insert({ UserLanguages::TurkishLanguage,			 "tr-TR" });

			std::string localeId = "en-US"; // Default to English

			Ptr<Preferences> preferences = this->preferences();
			if (preferences) {
				Ptr<GeneralPreferences> generalPreferences = preferences->generalPreferences();
				if (generalPreferences) {
					UserLanguages userLanguage = generalPreferences->userLanguage();
					if (LocaleIdMap.find(userLanguage) != LocaleIdMap.end()) {
						localeId = LocaleIdMap.at(userLanguage);// .substr(0, 2);
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

			for (const auto& [index, locale] : ToolsApp::LocaleIdMap) {
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
