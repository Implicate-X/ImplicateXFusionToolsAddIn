#include "pch.h"
#include "resource.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "ResourceHelper.h"
#include "ToolsCommandControl.h"
#include "LanguageDropDownControl.h"
#include "SketchTextCommandControl.h"
#include "ToolsBarPanel.h"
#include "ToolsBar.h"
#include "ToolsApp.h"

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>The initialize function sets up the toolbar panel by terminating any existing processes </para>
		/// <para>and adding necessary controls, returning true if successful or false if any additions fail.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::initialize() {
			terminate();

			if (!addToolsCommandControl()) {
				Application::log("Failed to add Tools CommandControl.");
				return false;
			}

			if (!addLanguageSelector()) {
				Application::log("Failed to add Language Selector.");
				return false;
			}

            return true;
		}

		/// <summary>
		/// <para>The terminate function is responsible for cleaning up and deleting specific command definitions </para>
		/// <para>related to region selections and the Tools command from the user interface.</para>
		/// </summary>
		void ToolsBarPanel::terminate() {
			if (gUi) {
				terminateLanguageSelector();

				Ptr<CommandDefinition> implicateXCommandDef = gUi->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX);
				if (implicateXCommandDef) {
					implicateXCommandDef->deleteMe();
				}
			}
		}

		/// <summary>
		/// <para>The terminateLanguageSelector function is for safely terminating and deleting </para>
		/// <para>the language selector command and its associated dropdown control, </para>
		/// <para>logging any failures encountered during the process.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::terminateLanguageSelector() {
			Ptr<CommandDefinition> commandDef = gUi->commandDefinitions()->itemById(IDS_CMD_LANG_SELECTOR);
			if (commandDef) {
				if (!commandDef->deleteMe()) {
					Application::log("Failed to delete CommandDefinition for Language Selector.");
					return false;
				}
			}

            Ptr<ToolbarControls> controls = this->controls();
            if (controls) {
                Ptr<LanguageDropDownControl> languageDropDown = controls->itemById(IDS_ID_LANG_SELECTOR);
				if (languageDropDown) {
					languageDropDown->terminate();
					if (!languageDropDown->deleteMe()) {
						Application::log("Failed to delete LanguageDropDownControl.");
						return false;
					}
				}
            }

			return true;
		}

		/// <summary>
		/// <para>The addToolsCommandControl function is responsible for adding a command control </para>
		/// <para>for the Implicate-X tool to the toolbar, ensuring that the necessary command definitions </para>
		/// <para>and controls are successfully created and initialized.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::addToolsCommandControl() {
            Ptr<ToolbarControls> controls = this->controls();
            if (!controls) {
                Application::log("Failed to retrieve ToolbarControls.");
                return false;
            }

            Ptr<CommandDefinition> toolsCommandDef = 
            gUi->commandDefinitions()->addButtonDefinition(IDS_CMD_IMPLICATEX,
                LoadStringFromResource(IDS_CMD_NAME_IMPLICATEX),
				LoadStringFromResource(IDS_CMD_NAME_IMPLICATEX),
                IDS_ID_TOOLS_RES);

            if (!toolsCommandDef) {
                Application::log("Failed to add CommandDefinition for Tools.");
                return false;
            }

            Ptr<ToolsCommandControl> toolsCommandControl = controls->addCommand(toolsCommandDef, "", false);
            if (!toolsCommandControl) {
                Application::log("Failed to add CommandControl for Implicate-X Tools.");
                return false;
            }

            if (!toolsCommandControl) {
                Application::log("Failed to create CommandControl for Implicate-X Tools.");
                return false;
            }

            if (!toolsCommandControl->initialize()) {
                return false;
            }

            return true;
		}

        /// <summary>
        /// <para>The addLanguageSelector function is responsible for creating a dropdown menu </para>
        /// <para>for selecting countries, populating it with command buttons for each region, </para>
        /// <para>and logging any failures encountered during the process.</para></summary>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        bool ToolsBarPanel::addLanguageSelector() {
            Ptr<ToolbarControls> controls = this->controls();
            if (!controls) {
                Application::log("Failed to retrieve ToolbarControls.");
                return false;
            }

			Ptr<LanguageDropDownControl> languageDropDown = controls->addDropDown(
                "   " + LoadStringFromResource(IDS_LABEL_SELECT_LANGUAGE),
                IDS_SUBDIR_RESOURCES + gLocaleId,
                IDS_ID_LANG_SELECTOR
            );

			if (!languageDropDown) {
				Application::log("Failed to create DropDownControl.");
				return false;
			}

			if (!languageDropDown->initialize()) {
				return false;
			}
			
			return true;
        }

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles the event </para>
		/// <para>when a language command is created, logging a message with the selected language.</para>
		/// </summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void LanguageCommandCreatedEventHandler::notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) {
			std::string message = "Selected language: " + localeId_;
			Application::log(message);

			Ptr<Workspace> workspace = gUi->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (!workspace) {
				Application::log("Workspace 'FusionSolidEnvironment' not found.");
				return;
			}

			Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
			if (!panels) {
				Application::log("Failed to retrieve toolbar panels.");
				return;
			}

			Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
			if (toolsBarPanel) {
				std::string tempLocaleId = localeId_;
                std::replace(tempLocaleId.begin(), tempLocaleId.end(), '_', '-');

				if (!toolsBarPanel->terminateLanguageSelector()) {
					Application::log("Failed to terminate LanguageDropDownControl.");
					return;
				}
				gLocaleId = tempLocaleId.c_str();
				if (!toolsBarPanel->addLanguageSelector()) {
					Application::log("Failed to add LanguageDropDownControl.");
					return;
				}
			}
		}
	}
}
