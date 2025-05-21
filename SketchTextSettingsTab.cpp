#include "pch.h"
#include "resource.h"
#include "ResourceHelper.h"
#include "FileHelper.h"
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
				tabInputs->addFloatSliderCommandInput(IDS_ITEM_TEXT_ZOOM_FACTOR, 
					LoadStringFromResource(IDS_LABEL_TEXT_ZOOM_FACTOR), IDS_UNIT_MM, 0.1, 5.0, false);
			if (!zoomSlider) {
				LOG_ERROR("Failed to create zoom slider");
				return false;
			}

			ensureUserSettingsDirectoryExists();

			zoomSlider->valueOne(load());

			command->inputChanged()->add(new SketchTextSettingsTabInputChangedEventHandler());

			return true;
		}

		void SketchTextSettingsTab::save(double zoomFactor) {
			json j;
			j["SketchText"]["zoomFactor"] = zoomFactor;
			std::string path = getUserSettingsPath();
			LOG_INFO("Saving settings to: " + path);
			std::ofstream file(path);
			file << j.dump(4);
		}

		double SketchTextSettingsTab::load() {
			std::string path = getUserSettingsPath();
			LOG_INFO("Loading settings from: " + path);
			std::ifstream file(path);
			LOG_INFO("File opened: " + std::to_string(file.is_open()));
			if (!file) return 1.0;
			json j;
			file >> j;
			LOG_INFO("File read: " + std::to_string(file.good()));
			LOG_INFO(j.dump(4));
			return j.value("SketchText", json{}).value("zoomFactor", 1.0);
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
			LOG_INFO("SettingsTab InputChanged: " + inputId);

			std::weak_ptr<SketchTextSettingsTab> settingsTabTemp = toolsApp->sketchTextPanel->settingsTab_;
			auto settingsTab = settingsTabTemp.lock();
			if (!settingsTab) {
				LOG_ERROR("Failed to get settings tab");
				return;
			}

			while (true)
			{
				Ptr<Command> command = eventArgs->input()->parentCommand();
				if (!command) {
					LOG_ERROR("Failed to get command from event args");
					break;
				}
				Ptr<CommandInputs> inputs = command->commandInputs();
				if (!inputs) {
					LOG_ERROR("Failed to get command inputs from command");
					break;
				}
				Ptr<FloatSliderCommandInput> zoomSlider = inputs->itemById(IDS_ITEM_TEXT_ZOOM_FACTOR);
				if (!zoomSlider) {
					LOG_ERROR("Failed to get slider input from command inputs");
					break;
				}

				if (eventArgs->input()->id() == IDS_ITEM_TEXT_ZOOM_FACTOR) {
					double lastZoomFactor = zoomSlider->valueOne();
					LOG_INFO("Zoomfaktor: " + std::to_string(lastZoomFactor));
					settingsTab->save(lastZoomFactor);
				}

				break;
			}

			settingsTab.reset();
		}
	}
}
