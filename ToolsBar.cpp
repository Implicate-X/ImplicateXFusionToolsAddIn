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
			Application::log("ToolsBar::initialize");
			Ptr<Workspace> workspace = toolsUI->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (!workspace) {
				Application::log("Workspace 'FusionSolidEnvironment' not found.");
				return false;
			}

			Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
			if (!panels) {
				Application::log("Failed to retrieve toolbar panels.");
				return false;
			}

			Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
			if (toolsBarPanel) {
				toolsBarPanel->deleteMe();
			}

			toolsBarPanel = panels->add(IDS_ID_TOOLS_BAR, IDS_NAME_TOOLS_BAR, "", false);
			if (!toolsBarPanel) {
				Application::log("Failed to create ToolbarPanel.");
				return false;
			}

			if (!toolsBarPanel->initialize()) {
				Application::log("Failed to initialize ToolbarPanel.");
				toolsBarPanel->deleteMe();
				return false;
			}

			return true;
		}

		void ToolsBar::terminate() {
			Ptr<Workspace> workspace = toolsUI->workspaces()->itemById(IDS_ID_SOLID_ENV);
			if (workspace) {
				Application::log("ToolsBar::terminate");
				Ptr<ToolbarPanels> panels = workspace->toolbarPanels();
				if (panels) {
					Ptr<ToolsBarPanel> toolsBarPanel = panels->itemById(IDS_ID_TOOLS_BAR);
					if (toolsBarPanel) {
						toolsBarPanel->terminate();
						toolsBarPanel->deleteMe();
						Application::log("ImplicateXFusionToolsAddInPanel deleted.");
					}
				}
			}
		}
	}
}
