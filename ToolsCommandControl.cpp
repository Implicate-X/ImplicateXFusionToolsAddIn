#include "pch.h"
#include "resource.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ToolsCommandControl.h"

namespace implicatex {
namespace fusion {
        class ToolsCommandCreatedEventHandler : public CommandCreatedEventHandler {
        public:
            explicit ToolsCommandCreatedEventHandler() {}

            void notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) override {
                gUi->messageBox("ToolsCommand");
            }
        };

        bool ToolsCommandControl::initialize() {
			if (gUi) {
				Ptr<CommandDefinition> toolsCommandDef = gUi->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX);
                if (toolsCommandDef) {
                    auto handler = new ToolsCommandCreatedEventHandler();

                    if (!toolsCommandDef->commandCreated()->add(handler)) {
                        Application::log("Failed to add event handler for Tools CommandControl");
                        return false;
                    }
                }
			}

            return true;
        }
    }
}
