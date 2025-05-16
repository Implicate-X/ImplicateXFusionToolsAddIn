#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextPanel.h"

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>The dropDownSelected function processes the selection of a dropdown menu item related to sketches,</para>
		/// <para>logging the selected sketch name and aligning the model to the corresponding sketch's XY plane if found.</para>
		/// </summary>
		///
		/// <param name="eventArgs">
		/// <para> is a constant reference to a pointer of type InputChangedEventArgs </para>
		/// <para>which likely encapsulates information about changes to input events in the Autodesk API.</para>
		/// </param>
		void SketchTextPanel::dropDownSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("dropDownSelected");
			Ptr<DropDownCommandInput> dropdown = eventArgs->input();
			Ptr<Sketch> sketch = nullptr;
			if (!toolsApp->sketchTextPanel->getSelectedSketch(dropdown, sketch)) {
				LOG_ERROR("Failed to get selected sketch");
				return;
			}
			toolsApp->sketchTextPanel->alignModelToSketchXYPlane(sketch);
		}

		/// <summary>Handles the text size replace described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::textHeightReplaced(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightReplaced");
		}

		/// <summary>Handles the text size change described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::textHeightChanged(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightChanged");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return;
			}
		}

		void SketchTextPanel::textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textIdCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}

		void SketchTextPanel::textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textValueCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}

		void SketchTextPanel::textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles input change events.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type InputChangedEventArgs, </para>
		/// <para>which is typically used to handle input change events.</para>
		/// </param>
		void SketchTextPanelInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
			std::string inputId = eventArgs->input()->id();
			LOG_INFO("InputChanged: " + inputId);

			auto it = toolsApp->sketchTextPanel->sketchTextMap_.find(inputId);
			if (it != toolsApp->sketchTextPanel->sketchTextMap_.end()) {
				Ptr<SketchText> sketchText = it->second;
				if (sketchText) {

					LOG_INFO("Text = " + toolsApp->sketchTextPanel->sketchTextMap_[inputId]->text() + " - SketchText = " + sketchText->text());

					toolsApp->sketchTextPanel->getTextPosition(sketchText);
					//toolsApp->sketchTextPanel->addHighlightGraphics(sketchText);
					//toolsApp->sketchTextPanel->focusCameraOnText(sketchText);

				//for (int i = 0; i < 3; ++i) {
					//	TextStyles originalStyle = sketchText->textStyle();
					//	std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 300ms warten
					//	sketchText->textStyle((TextStyles)(TextStyles::TextStyleBold | TextStyles::TextStyleUnderline));
					//	std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 300ms warten
					//	sketchText->textStyle(originalStyle);
					//}
				}
				else {
					LOG_ERROR("SketchText not found for input ID: " + inputId);
				}
			}

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs) {
				LOG_ERROR("Invalid inputs");
				return;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return;
			}

			inputId = std::regex_replace(inputId, std::regex("^TextID_\\d+$"), IDS_CELL_TEXT_ID);
			inputId = std::regex_replace(inputId, std::regex("^TextValue_\\d+$"), IDS_CELL_TEXT_VALUE);
			inputId = std::regex_replace(inputId, std::regex("^TextHeight_\\d+$"), IDS_CELL_TEXT_HEIGHT);

			if (toolsApp->sketchTextPanel->actionHandlers_.find(inputId) != 
				toolsApp->sketchTextPanel->actionHandlers_.end()) {
				toolsApp->sketchTextPanel->actionHandlers_[inputId](eventArgs);
			} else {
				LOG_ERROR("Unknown inputId: " + inputId);
			}
			return;
		}
	}
}
