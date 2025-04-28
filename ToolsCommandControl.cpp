#include "pch.h"
#include "resource.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ToolsCommandControl.h"

namespace implicatex {
    namespace fusion {
        /// <summary>
        /// <para>The initialize function of the ToolsCommandControl class sets up a command </para>
        /// <para>by adding an event handler for the command creation event, returning true if successful.</para>
        /// </summary>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        bool ToolsCommandControl::initialize() {
			if (toolsUI) {
				Ptr<CommandDefinition> implicateXCommandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX_TOOLS);
                if (implicateXCommandDef) {
                    auto handler = new ToolsCommandCreatedEventHandler();

                    if (!implicateXCommandDef->commandCreated()->add(handler)) {
                        Application::log("Failed to add event handler for ToolsCommandControl");
                        return false;
                    }
                }
			}

            return true;
        }

        /// <summary>The notify method is an overridden virtual function that handles the event when a command is created.</summary>
        ///
        /// <param name="eventArgs">
		/// <para>- The event arguments that contain information about the command creation event.</para>
        /// </param>
        void ToolsCommandCreatedEventHandler::notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) {
            Application::log(IDS_CMD_IMPLICATEX_TOOLS);
        }
    }
}
