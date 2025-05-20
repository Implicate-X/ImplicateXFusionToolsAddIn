#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextSettingsTab.h"
#include "SketchTextHeightTab.h"
#include "SketchTextPanel.h"

using json = nlohmann::json;

namespace implicatex {
	namespace fusion {
		bool SketchTextSettingsTab::initialize(Ptr<Command> command, const Ptr<TabCommandInput>& tabInput) {
			if (!tabInput) {
				LOG_ERROR("Invalid tab input");
				return false;
			}
			Ptr<CommandInputs> tabInputs = tabInput->children();
			if (!tabInputs) {
				LOG_ERROR("Invalid tab inputs");
				return false;
			}

			Ptr<FloatSliderCommandInput> zoomSlider = 
				tabInputs->addFloatSliderCommandInput("textZoomSlider", "Text-Zoomfaktor", "mm", 0.1, 5.0, false);
			if (!zoomSlider) {
				LOG_ERROR("Failed to create zoom slider");
				return false;
			}
			zoomSlider->valueOne(1.0);

			command->inputChanged()->add(new SketchTextSettingsTabInputChangedEventHandler());

			return true;
		}

		void saveSettings(double zoomFactor) {
			json j;
			j["zoomFactor"] = zoomFactor;
			std::ofstream file("SketchTextSettings.json");
			file << j.dump(4);
		}

		double loadSettings() {
			std::ifstream file("SketchTextSettings.json");
			if (!file) return 1.0; // Default-Wert
			json j;
			file >> j;
			return j.value("zoomFactor", 1.0);
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles input change events.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type InputChangedEventArgs, </para>
		/// <para>which is typically used to handle input change events.</para>
		/// </param>
		void SketchTextSettingsTabInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
			std::string inputId = eventArgs->input()->id();
			LOG_INFO("InputChanged: " + inputId);

			std::weak_ptr<SketchTextSettingsTab> settingsTabTemp = toolsApp->sketchTextPanel->settingsTab_;
			auto settingsTab = settingsTabTemp.lock();
			if (!settingsTab) {
				LOG_ERROR("Failed to get settings tab");
				return;
			}
			settingsTab.reset();

			Ptr<Command> command = eventArgs->input()->parentCommand();
			if (!command) {
				LOG_ERROR("Failed to get command from event args");
				return;
			}
			Ptr<CommandInputs> inputs = command->commandInputs();
			if (!inputs) {
				LOG_ERROR("Failed to get command inputs from command");
				return;
			}
			Ptr<FloatSliderCommandInput> zoomSlider = inputs->itemById("textZoomSlider");
			if (!zoomSlider) {
				LOG_ERROR("Failed to get slider input from command inputs");
				return;
			}

			double lastZoomFactor = 1.0;

			zoomSlider->valueOne(lastZoomFactor);

			if (eventArgs->input()->id() == "textZoomSlider") {
				lastZoomFactor = zoomSlider->valueOne();
				LOG_INFO("Zoomfaktor: " + std::to_string(lastZoomFactor));
			}
		}
	}
}
