#include "pch.h"
#include "resource.h"  
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"

namespace implicatex {
namespace fusion {
        /// <summary>
        /// <para>The initialize function of the SketchTextCommandControl class sets up a command </para>
        /// <para>by adding an event handler for the command creation event, returning true if successful.</para>
        /// </summary>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        bool SketchTextCommandControl::initialize() {
			if (toolsUI) {
				Ptr<CommandDefinition> sketchTextCommandDef = toolsUI->commandDefinitions()->itemById(IDS_CMD_SKETCHTEXT);
                if (sketchTextCommandDef) {
                    auto handler = new SketchTextCommandCreatedEventHandler();

                    if (!sketchTextCommandDef->commandCreated()->add(handler)) {
                        TRACE("Failed to add event handler for SketchTextCommandControl");
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
        void SketchTextCommandCreatedEventHandler::notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) {
            TRACE(IDS_CMD_SKETCHTEXT);
            toolsApp->createSketchTextPanel();
        }
    }
}
