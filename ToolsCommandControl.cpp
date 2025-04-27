#include "pch.h"
#include "resource.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ToolsCommandControl.h"

namespace implicatex {
    namespace fusion {
        /// <summary>
        /// <para>ToolsCommandCreatedEventHandler is a event handler that inherits from CommandCreatedEventHandler</para>
		/// <para> and overrides the notify method to display a message box when a tools command is created.</para>
        /// </summary>
        class ToolsCommandCreatedEventHandler : public CommandCreatedEventHandler {
        public:
            explicit ToolsCommandCreatedEventHandler() {}

            void notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) override {
                Application::log(IDS_CMD_IMPLICATEX_TOOLS);
            }
        };

        /// <summary>
        /// <para>The initialize function of the ToolsCommandControl class sets up a command </para>
        /// <para>by adding an event handler for the command creation event, returning true if successful.</para>
        /// </summary>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        bool ToolsCommandControl::initialize() {
			if (gUi) {
				Ptr<CommandDefinition> implicateXCommandDef = gUi->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX_TOOLS);
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
    }
}
