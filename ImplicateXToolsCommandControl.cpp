#include "pch.h"
#include "resource.h"  
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXToolsCommandControl.h"

namespace implicatex {
namespace fusion {
        class ImplicateXToolsCommandCreatedEventHandler : public CommandCreatedEventHandler {
        public:
            explicit ImplicateXToolsCommandCreatedEventHandler() {}

            void notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) override {
                gUi->messageBox("ImplicateXToolsCommand");
            }
        };

        bool ImplicateXToolsCommandControl::initialize() {
			if (gUi) {
				Ptr<CommandDefinition> implicateXCommandDef = gUi->commandDefinitions()->itemById(IDS_CMD_IMPLICATEX);
                if (implicateXCommandDef) {
                    auto handler = new ImplicateXToolsCommandCreatedEventHandler();

                    if (!implicateXCommandDef->commandCreated()->add(handler)) {
                        Application::log("Failed to add event handler for Implicate-X Tools CommandControl");
                        return false;
                    }
                }
			}

            return true;
        }
    }
}
