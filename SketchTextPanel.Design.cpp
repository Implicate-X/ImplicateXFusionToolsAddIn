#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextHeightTab.h"
#include "SketchTextPanel.h"
#include "SketchTextSettingsTab.h"

namespace implicatex {
	namespace fusion {
		/// <summary>Adds a text size tab to 'tabInputs'.</summary>
		///
		/// <param name="inputs">   The inputs.</param>
		/// <param name="tabInputs">[in,out] The tab inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::addTextHeightTab(const Ptr<Command>& command) {
			Ptr<CommandInputs> inputs = command->commandInputs();
			if (!inputs) {
				LOG_ERROR("Failed to get command inputs");
				return false;
			}

			Ptr<TabCommandInput> textHeightTab = 
				inputs->addTabCommandInput(IDS_ITEM_TAB_TEXT_HEIGHT, 
					LoadStringFromResource(IDS_LABEL_TAB_TEXT_HEIGHT), IDS_PATH_ICON_SKETCH_TEXT_HEIGHT);

			if (!textHeightTab) {
				LOG_ERROR("Failed to add text height tab command input");
				return false;
			}

			textHeightTab_ = std::make_shared<SketchTextHeightTab>();

            if (!textHeightTab_ || !textHeightTab_->initialize(command, textHeightTab)) {
                LOG_ERROR("Failed to initialize text height tab command input");
                return false;
            }

			return true;
		}

		bool SketchTextPanel::addSettingsTab(const Ptr<CommandInputs>& inputs, Ptr<CommandInputs>& tabInputs) {
			//Ptr<SketchTextSettingsTab> settingsTab = 
			//	inputs->addTabCommandInput(IDS_ITEM_TAB_SETTINGS, 
			//		LoadStringFromResource(IDS_LABEL_TAB_SETTINGS), IDS_PATH_ICON_SKETCH_TEXT_SETTINGS);

			//tabInputs = settingsTab->children();
			//if (!tabInputs) {
			//	LOG_ERROR("Failed to add settings tab command input");
			//	return false;
			//}

			return true;
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles the creation </para>
		/// <para>of a command by setting up its user interface elements,including tabs, dropdowns, </para>
		/// <para>and input fields for user interaction.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type CommandCreatedEventArgs,</para>
		/// <para>which likely contains information related to the creation of a command event.</para>
		/// </param>
		void SketchTextPanelCommandCreatedEventHandler::notify(const Ptr<CommandCreatedEventArgs>& eventArgs) {
			Ptr<Command> command = eventArgs->command();
			Ptr<CommandInputs> inputs = command->commandInputs();

			if (!inputs) {
				LOG_ERROR("Failed to get command inputs");
				return;
			}

			if (!toolsApp->sketchTextPanel->addTextHeightTab(command)) {
				LOG_ERROR("Failed to add text height tab command input");
				return;
			}

			Ptr<CommandInputs> settingsTabInputs = nullptr;
			if (!toolsApp->sketchTextPanel->addSettingsTab(inputs, settingsTabInputs)) {
				LOG_ERROR("Failed to add settings tab command input");
				return;
			}
		}
	}
}
