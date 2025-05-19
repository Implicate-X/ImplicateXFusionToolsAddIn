#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextHeightTab.h"
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
		void SketchTextHeightTab::dropDownSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
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
		void SketchTextHeightTab::textHeightReplaced(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightReplaced");
		}

		/// <summary>Handles the text size change described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextHeightTab::textHeightChanged(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightChanged");

			Ptr<Command> command = eventArgs->input()->parentCommand();
			if (!command) {
				LOG_ERROR("Invalid command");
				return;
			}
			Ptr<CommandInputs> inputs = command->commandInputs();
			if (!inputs) {
				LOG_ERROR("Invalid inputs");
				return;
			}
			Ptr<TabCommandInput> textHeightTab = inputs->itemById(IDS_ITEM_TAB_TEXT_HEIGHT);
			if (!textHeightTab) {
				LOG_ERROR("Failed to get height tab");
				return;
			}

			auto heightTab = toolsApp->sketchTextPanel->textHeightTab_;

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!heightTab->getTextHeightMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return;
			}
		}

		void SketchTextHeightTab::textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textIdCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}

		void SketchTextHeightTab::textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textValueCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}

		void SketchTextHeightTab::textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightCellSelected");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

		}
	}
}
