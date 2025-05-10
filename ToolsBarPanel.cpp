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
				TRACE("Failed to add Tools CommandControl.");
				return false;
			}
			
			if (!addSketchTextCommandControl()) {
				TRACE("Failed to add Sketch Text CommandControl.");
				return false;
			}

			if (!addLanguageSelector()) {
				TRACE("Failed to add Language Selector.");
				return false;
			}

            return true;
		}

		/// <summary>
		/// <para>The terminate function is responsible for cleaning up and deleting specific command definitions </para>
		/// <para>related to region selections and the Tools command from the user interface.</para>
		/// </summary>
		void ToolsBarPanel::terminate() {
			if (toolsUI) {
				removeLanguageSelector();
				removeSketchTextCommandControl();

				Ptr<CommandDefinition> implicateXCommandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX_TOOLS);
				if (implicateXCommandDef) {
					implicateXCommandDef->deleteMe();
				}
			}
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
                TRACE("Failed to retrieve ToolbarControls.");
                return false;
            }

			std::string buttonName = LoadStringFromResource(IDS_CMD_NAME_IMPLICATEX);

            Ptr<CommandDefinition> implicateXCommandDef = 
            toolsUI->commandDefinitions()->addButtonDefinition(IDS_CMD_IMPLICATEX_TOOLS, buttonName, buttonName, IDS_SUBDIR_LOGO);

            if (!implicateXCommandDef) {
                TRACE("Failed to add CommandDefinition for Tools.");
                return false;
            }

            Ptr<ToolsCommandControl> implicateXCommandControl = controls->addCommand(implicateXCommandDef, "", false);
            if (!implicateXCommandControl) {
                TRACE("Failed to add CommandControl for Implicate-X Tools.");
                return false;
            }

            if (!implicateXCommandControl) {
                TRACE("Failed to create CommandControl for Implicate-X Tools.");
                return false;
            }

            if (!implicateXCommandControl->initialize()) {
                return false;
            }

            return true;
		}

		/// <summary>
		/// <para>addSketchTextCommandControl adds a command control for sketch text functionality to the toolbar, </para>
		/// <para>ensuring proper initialization and logging any failures encountered during the process.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::addSketchTextCommandControl() {
            Ptr<ToolbarControls> controls = this->controls();
            if (!controls) {
                TRACE("Failed to retrieve ToolbarControls.");
                return false;
            }

			std::string buttonLabel = LoadStringFromResource(IDS_LABEL_SKETCHTEXT);

            Ptr<CommandDefinition> sketchTextCommandDef = 
            toolsUI->commandDefinitions()->addButtonDefinition(IDS_CMD_SKETCHTEXT, buttonLabel, buttonLabel, IDS_SUBDIR_SKETCHTEXT);

            if (!sketchTextCommandDef) {
                TRACE("Failed to add CommandDefinition for Sketch text definitions.");
                return false;
            }

            Ptr<SketchTextCommandControl> sketchTextCommandControl = controls->addCommand(sketchTextCommandDef, "", false);
            if (!sketchTextCommandControl) {
                TRACE("Failed to add CommandControl for Sketch text definitions.");
                return false;
            }

            if (!sketchTextCommandControl) {
                TRACE("Failed to create CommandControl for Sketch text definitions.");
                return false;
            }

            if (!sketchTextCommandControl->initialize()) {
                return false;
            }

            return true;
		}

		/// <summary>
		/// <para>removeSketchTextCommandControl is for deleting the sketch text command from the user interface </para>
		/// <para>if it exists, returning true upon successful execution.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::removeSketchTextCommandControl() {
			if (toolsUI)
			{
				Ptr<CommandDefinition> sketchTextCommandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_SKETCHTEXT);
				if (sketchTextCommandDef) {
					sketchTextCommandDef->deleteMe();
				}
			}
			return true;
		}

		/// <summary>
		/// <para>updateSketchTextCommandControlLabel updates the control label of the sketch text command, </para>
		/// <para>using a string loaded from resources, and returns a boolean indicating success.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::updateSketchTextCommandControlLabel() {
			if (toolsUI)
			{
				std::string controlLabel = LoadStringFromResource(IDS_LABEL_SKETCHTEXT);

				Ptr<CommandDefinition> sketchTextCommandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_SKETCHTEXT);
				if (sketchTextCommandDef) {
					sketchTextCommandDef->controlDefinition()->name(controlLabel);
				}
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
                TRACE("Failed to retrieve ToolbarControls.");
                return false;
            }

			std::string itemText = "   " + LoadStringFromResource(IDS_LABEL_SELECT_LANGUAGE);

			Ptr<LanguageDropDownControl> languageDropDown = 
				controls->addDropDown(itemText, IDS_SUBDIR_FLAGS + toolsLocaleId, IDS_ID_LANG_SELECTOR);

			if (!languageDropDown) {
				TRACE("Failed to create DropDownControl.");
				return false;
			}

			if (!languageDropDown->initialize()) {
				return false;
			}
			
			return true;
        }

		/// <summary>
		/// <para>The removeLanguageSelector function is for safely removing </para>
		/// <para>the language selector command and its associated dropdown control, </para>
		/// <para>logging any failures encountered during the process.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool ToolsBarPanel::removeLanguageSelector() {
			Ptr<CommandDefinition> commandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_LANG_SELECTOR);
			if (commandDef) {
				if (!commandDef->deleteMe()) {
					TRACE("Failed to delete CommandDefinition for Language Selector.");
					return false;
				}
			}

            Ptr<ToolbarControls> controls = this->controls();
            if (controls) {
                Ptr<LanguageDropDownControl> languageDropDown = controls->itemById(IDS_ID_LANG_SELECTOR);
				if (languageDropDown) {
					languageDropDown->terminate();
					if (!languageDropDown->deleteMe()) {
						TRACE("Failed to delete LanguageDropDownControl.");
						return false;
					}
				}
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
			TRACE(message);

			Ptr<Workspace> workspace = toolsUI->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (!workspace) {
				TRACE("Workspace 'FusionSolidEnvironment' not found.");
				return;
			}

			Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
			if (!panels) {
				TRACE("Failed to retrieve toolbar panels.");
				return;
			}

			Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
			if (toolsBarPanel) {
				std::string tempLocaleId = localeId_;
                std::replace(tempLocaleId.begin(), tempLocaleId.end(), '_', '-');

				if (!toolsBarPanel->removeLanguageSelector()) {
					TRACE("Failed to terminate LanguageDropDownControl.");
					return;
				}
				toolsLocaleId = tempLocaleId.c_str();
				if (!toolsBarPanel->addLanguageSelector()) {
					TRACE("Failed to add LanguageDropDownControl.");
					return;
				}

				toolsBarPanel->updateSketchTextCommandControlLabel();
			}
		}
	}
}
