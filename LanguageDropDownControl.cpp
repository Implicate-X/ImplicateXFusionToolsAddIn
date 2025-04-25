#include "pch.h"
#include "resource.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ToolsBarPanel.h"
#include "LanguageDropDownControl.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>commandDefCounters_ is a static member of the LanguageDropDownControl class,</para>
		/// <para>which stores a mapping of command definitions to their corresponding integer counters using a std::map.</para>
		/// <para>This map is used to keep track of the number of command definitions created for each language.</para>
		/// </summary>
		std::map<std::string, int> LanguageDropDownControl::commandDefCounters_;


		/// <summary>
		/// <para>The eventHandlers symbol is a std::map that associates std::string keys with pointers</para>
		/// <para>to LanguageCommandCreatedEventHandler objects within the implicatex::fusion namespace,</para>
		/// <para>likely used to manage event handlers for language command creation events.</para>
		/// </summary>
		std::map<std::string, LanguageCommandCreatedEventHandler*> eventHandlers;

		/// <summary>
		/// <para>The LanguageDropDownControl class is responsible for creating a dropdown menu</para>
		/// <para>for selecting countries, populating it with command buttons for each region,</para>
		/// <para>and logging any failures encountered during the process.</para></summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool LanguageDropDownControl::initialize(bool doTerminate) {
			initializeCommandDefCounters();
			if (doTerminate) {
				terminate();
			}
			if (gUi) {
				std::vector<std::pair<std::string, std::string>> sortedLocaleLanguageRegionList;
				gApp->getLanguageRegionNamesSorted(gLocaleId, sortedLocaleLanguageRegionList);
				for (const auto& entry : sortedLocaleLanguageRegionList) {
					std::string locale = entry.first;
                    std::string locale_ = locale;
					std::string language = entry.second;
                    std::replace(locale_.begin(), locale_.end(), '-', '_');
					std::string commandId = generateUniqueCommandId(locale_);
					std::string commandName = "   " + language;
					std::string commandDescription = language;
					auto commandDef = gUi->commandDefinitions()->itemById(commandId);
					if (commandDef) {
						Application::log("Terminate: CommandDefinition already exists for " + commandId);
						if (!commandDef->deleteMe()) {
							Application::log("Terminate: Failed to delete CommandDefinition for " + commandId);
							continue;
						}
					}
					commandDef = gUi->commandDefinitions()->addButtonDefinition(commandId.c_str(), commandName.c_str(), commandDescription.c_str(), "Resources/" + locale);
					if (!commandDef) {
						Application::log("Failed to add CommandDefinition for " + commandId);
						continue;
					}
                    auto handler = new LanguageCommandCreatedEventHandler(locale_);
					eventHandlers[commandId] = handler;
					if (!commandDef->commandCreated()->add(handler)) {
						Application::log("Failed to add event handler for " + commandId);
						return false;
					}
					auto commandControl = this->controls()->addCommand(commandDef, "", false);
					if (!commandControl) {
						Application::log("Failed to add CommandControl for " + commandId);
						return false;
					}
				}
			}
			return true;
		}

		/// <summary>
		/// <para>The terminate function is responsible for deleting command definitions associated</para>
		/// <para>with region commands based on the current locale in the user interface.</para>
		/// </summary>
		void LanguageDropDownControl::terminate() {
			if (gUi) {
				std::vector<std::pair<std::string, std::string>> sortedLocaleLanguageRegionList;
				gApp->getLanguageRegionNamesSorted(gLocaleId, sortedLocaleLanguageRegionList);
				for (const auto& entry : sortedLocaleLanguageRegionList) {
					std::string locale = entry.first;
                    std::string locale_ = locale;
					std::string language = entry.second;
                    std::replace(locale_.begin(), locale_.end(), '-', '_');
					std::string commandId = getCurrentCommandId(locale_);
					auto commandDef = gUi->commandDefinitions()->itemById(commandId);
					if (commandDef) {
						auto event = commandDef->commandCreated();
						if (event) {
							if (!event->remove(eventHandlers[commandId])) {
								Application::log("Failed to remove event handler for " + commandId);
							}
						}
						adsk::doEvents();
						commandDef->deleteMe();
					}
					auto commandControl = this->controls()->itemById(commandId);
					if (commandControl) {
						if (!commandControl->deleteMe()) {
							Application::log("Failed to delete CommandControl for " + commandId);
						}
					}
				}
			}
		}

		/// <summary>
		/// <para>The generateUniqueCommandId function generates a unique command identifier string</para>
		/// <para>based on the provided localeId, incrementing a counter associated with that locale.</para>
		/// </summary>
		///
		/// <param name="localeId">
		/// <para>localeId is a constant reference to a std::string</para>
		/// <para>that likely represents a unique identifier for a specific locale in a program.</para>
		/// </param>
		///
		/// <returns>The unique command identifier.</returns>
		std::string LanguageDropDownControl::generateUniqueCommandId(const std::string& localeId) {
			int counter = commandDefCounters_[localeId]+1;
			commandDefCounters_[localeId] = ++counter;
			return "LanguageCmd_" + localeId + "_" + std::to_string(counter);
		}

		/// <summary><para>The getCurrentCommandId function generates a unique command ID string</para>
		/// 		 <para>based on the provided locale identifier and a counter associated with that
		/// 		 locale.</para></summary>
		///
		/// <param name="localeId"><para>localeId is a constant reference to a std::string</para>
		/// 					   <para>that likely represents a unique identifier for a specific
		/// 					   locale in a program.</para></param>
		///
		/// <returns>The current command identifier.</returns>
		std::string LanguageDropDownControl::getCurrentCommandId(const std::string& localeId) {
			int counter = commandDefCounters_[localeId];
			return "LanguageCmd_" + localeId + "_" + std::to_string(counter);
		}

		/// <summary>
		/// <para>The initializeCommandDefCounters function initializes a map of command definition counters</para>
		/// <para>for various languages, setting their initial values to zero if the map is empty.</para>
		/// </summary>
		void LanguageDropDownControl::initializeCommandDefCounters() {
			if (commandDefCounters_.empty()) {
				commandDefCounters_.insert({ "zh-CN", 0 });
				commandDefCounters_.insert({ "zh-TW", 0 });
				commandDefCounters_.insert({ "cs-CZ", 0 });
				commandDefCounters_.insert({ "en-US", 0 });
				commandDefCounters_.insert({ "fr-FR", 0 });
				commandDefCounters_.insert({ "de-DE", 0 });
				commandDefCounters_.insert({ "hu-HU", 0 });
				commandDefCounters_.insert({ "it-IT", 0 });
				commandDefCounters_.insert({ "ja-JP", 0 });
				commandDefCounters_.insert({ "ko-KR", 0 });
				commandDefCounters_.insert({ "pl-PL", 0 });
				commandDefCounters_.insert({ "pt-BR", 0 });
				commandDefCounters_.insert({ "ru-RU", 0 });
				commandDefCounters_.insert({ "es-ES", 0 });
				commandDefCounters_.insert({ "tr-TR", 0 });
			}
		}
	}
}
