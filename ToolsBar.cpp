#include "pch.h"
#include "resource.h"  
#include "ToolsBarPanel.h"
#include "ToolsBar.h"
#include "ToolsApp.h"  
#include "ImplicateXFusionToolsAddIn.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		bool ToolsBar::initialize() {
			TRACE("ToolsBar::initialize");
			Ptr<Workspace> workspace = toolsUI->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (!workspace) {
				TRACE("Workspace 'FusionSolidEnvironment' not found.");
				return false;
			}

			Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
			if (!panels) {
				TRACE("Failed to retrieve toolbar panels.");
				return false;
			}

			Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
			if (toolsBarPanel) {
				toolsBarPanel->deleteMe();
			}

			toolsBarPanel = panels->add(IDS_ID_TOOLS_BAR, IDS_NAME_TOOLS_BAR, "", false);
			if (!toolsBarPanel) {
				TRACE("Failed to create ToolbarPanel.");
				return false;
			}

			if (!toolsBarPanel->initialize()) {
				TRACE("Failed to initialize ToolbarPanel.");
				toolsBarPanel->deleteMe();
				return false;
			}

			return true;
		}

		void ToolsBar::terminate() {
			Ptr<Workspace> workspace = toolsUI->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (workspace) {
				TRACE("ToolsBar::terminate");
				Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
				if (panels) {
					Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
					if (toolsBarPanel) {
						toolsBarPanel->terminate();
						toolsBarPanel->deleteMe();
						TRACE("ImplicateXFusionToolsAddInPanel deleted.");
					}
				}
			}
		}
	}
}
