#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "Logging.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextSettingsTab.h"
#include "SketchTextHeightTab.h"
#include "SketchTextPanel.h"

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>The initialize function in the SketchTextPanel class is responsible</para>
		/// <para>for setting up the palette by invoking the createCommand method.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
        bool SketchTextPanel::initialize() {  
           return createCommand();  
        }

		/// <summary>
		/// <para>The terminate function in the SketchTextPanel class is a boolean method </para>
		/// <para>that calls and returns the result of the removeCommand function.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::terminate() {
			if (settingsTab_) {
				settingsTab_.reset();
				settingsTab_ = nullptr;
			}

			if (textHeightTab_) {
				textHeightTab_.reset();
				textHeightTab_ = nullptr;
			}
			return removeCommand();
		}

		/// <summary>
		/// <para>The createCommand function in the SketchTextPanel class is responsible</para>
		/// <para>for creating and executing a new command definition for sketch text definitions.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::createCommand() {
			if (!removeCommand()) {
				return false;
			}
			
			if (toolsUI) {
				std::string selectSketchLabel = LoadStringFromResource(IDS_LABEL_SELECT_SKETCH);
				Ptr<CommandDefinition> sketchTextPanelCommandDef = 
					toolsUI->commandDefinitions()
							->addButtonDefinition(IDS_CMD_SKETCH_TEXT_DEFINITIONS, selectSketchLabel, selectSketchLabel);
				if (sketchTextPanelCommandDef) {
                    auto handler = new SketchTextPanelCommandCreatedEventHandler();
					if (!sketchTextPanelCommandDef->commandCreated()->add(handler)) {
						LOG_ERROR("Failed to add event handler for SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
					if (!sketchTextPanelCommandDef->execute()) {
						LOG_ERROR("Failed to execute SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
				}
				else {
					LOG_ERROR("Failed to add button definition for SketchTextPanelCommand");
					return false;
				}
			}
			return true;
		}

		/// <summary>
		/// <para>The removeCommand function in the SketchTextPanel class is for deleting </para>
		/// <para>a specific command definition associated with the sketch text panel.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::removeCommand() {
			if (toolsUI) {
				Ptr<CommandDefinition> sketchTextPanelCommandDef = 
					toolsUI->commandDefinitions()->itemById(IDS_CMD_SKETCH_TEXT_DEFINITIONS);
				if (sketchTextPanelCommandDef) {
					if (!sketchTextPanelCommandDef->deleteMe()) {
						LOG_ERROR("Failed to delete SketchTextPanelCommand");
						sketchTextPanelCommandDef = nullptr;
						return false;
					}
					sketchTextPanelCommandDef = nullptr;
				}
			}
			return true;
		}
	}
}
