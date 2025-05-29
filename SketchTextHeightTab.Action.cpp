#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "Logging.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextSettingsTab.h"
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
			LOG_INFO("SketchTextHeightTab::textHeightChanged");

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

			SketchTextHeightTab* heightTab = toolsApp->sketchTextPanel->getTextHeightTab().get();
			if (heightTab == nullptr) {
				LOG_ERROR("Failed to get height tab");
				return;
			}

			std::vector<Ptr<SketchText>> filteredTexts;
			bool isSucceeded = heightTab->getTextHeightMatchItems(inputs, filteredTexts);
			if (!isSucceeded) {
				LOG_ERROR("Failed to get text height match items");
				return;
			}
		}

		void SketchTextHeightTab::textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textIdCellSelected");
			SketchTextHeightTab::get()->localizeText(eventArgs);
		}

		void SketchTextHeightTab::textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textValueCellSelected");
			SketchTextHeightTab::get()->localizeText(eventArgs);

		}

		void SketchTextHeightTab::textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textHeightCellSelected");
			SketchTextHeightTab::get()->localizeText(eventArgs);

		}

		void SketchTextHeightTab::textToggleCellSelected(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("textToggleCellSelected");
			SketchTextHeightTab::get()->localizeText(eventArgs);

		}

		void SketchTextHeightTab::localizeText(const Ptr<InputChangedEventArgs>& eventArgs) {
			std::string inputId = eventArgs->input()->id();
			LOG_INFO("localizeText InputChanged: " + inputId);

			unsigned int selectedRow = getSelectedRowNumber(inputId);

			if (selectedRow == 0) {
				LOG_ERROR("Invalid selected row: " + std::to_string(selectedRow));
				return;
			}

			LOG_INFO("Selected Row = " + std::to_string(selectedRow));

			auto& idTextMap = idTextMap_;

			auto it = idTextMap.find(selectedRow);
			if (it != idTextMap.end()) {
				Ptr<SketchText> sketchText = it->second;
				if (sketchText) {
					LOG_INFO("Text = " + idTextMap[selectedRow]->text() + " - SketchText = " + sketchText->text());

					setSelectedText(sketchText);

					toolsApp->sketchTextPanel->addHighlightGraphics(sketchText);
					toolsApp->sketchTextPanel->focusCameraOnText(sketchText);
				}
				else {
					LOG_ERROR("SketchText not found for input ID: " + inputId);
				}
			}
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles input change events.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type InputChangedEventArgs, </para>
		/// <para>which is typically used to handle input change events.</para>
		/// </param>
		void SketchTextHeightTabInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
			std::string inputId = eventArgs->input()->id();
			LOG_INFO("Notify InputChanged: " + inputId);

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

			//int selectedRow = -1;
			//std::smatch match;
			//std::string expression = 
			//	std::format("^({}|{}|{}|{})_(\\d+)$", IDS_CELL_TEXT_ID, IDS_CELL_TEXT_VALUE, IDS_CELL_TEXT_HEIGHT, IDS_CELL_TEXT_TOGGLE);

			//if (std::regex_match(inputId, match, std::regex(expression))) {
			//	selectedRow = std::stoi(match[2]);
			//}
			//LOG_INFO("Selected Row: " + std::to_string(selectedRow));

			//if (selectedRow <= 0) {
			//	LOG_ERROR("Invalid selected row: " + std::to_string(selectedRow));
			//	return;
			//}

			//SketchTextHeightTab* heightTab = SketchTextHeightTab::get();

			//if (heightTab->getTextValueCellInput() != nullptr) {
			//	if (!heightTab->getTextValueCellInput()->isReadOnly()) {
			//		LOG_INFO("TextValueCellInput changed: " + inputId);
			//		Ptr<SketchText> sketchText = heightTab->getTextById(selectedRow-1);
			//		if (!sketchText) {
			//			LOG_ERROR("SketchText not found for inputId: " + inputId);
			//			return;
			//		}
			//		//sketchText->text(heightTab->getTextValueCellInput()->value());
			//	}

			//	heightTab->getTextValueCellInput()->isReadOnly(true);
			//}


			//if (inputId.find(IDS_CELL_TEXT_VALUE) != std::string::npos) {
			//	Ptr<StringValueCommandInput> textInput = inputs->itemById(inputId);
			//	if (textInput) {
			//		heightTab->setPendingTextValue(textInput->value());
			//	}
			//	textInput->isReadOnly(false);
			//	heightTab->setTextValueCellInput(textInput);
			//}

			//Ptr<TableCommandInput> tableInput =
			//	inputs->itemById(IDS_ITEM_TEXT_HEIGHT_TABLE);
			//if (!tableInput) {
			//	LOG_ERROR("TableCommandInput not found: " + IDS_ITEM_TEXT_HEIGHT_TABLE);
			//	return;
			//}

			//int selectedRow = tableInput->selectedRow();
			//if (selectedRow < 0) {
			//	LOG_INFO("No row selected");
			//	return;
			//}
			//LOG_INFO("Row selected: " + std::to_string(selectedRow));
			//Ptr<StringValueCommandInput> textValueInput = tableInput->getInputAtPosition(selectedRow, 1);
			//if (!textValueInput) {
			//	LOG_ERROR("TextValueCommandInput not found at row: " + std::to_string(selectedRow));
			//	return;
			//}
			//LOG_INFO("Text = " + textValueInput->value());

			//std::vector<Ptr<SketchText>> filteredTexts;
			//bool isSucceeded = heightTab->getTextHeightMatch(inputs, filteredTexts);
			//if (!isSucceeded) {
			//	LOG_ERROR("Failed to get text height match");
			//	return;
			//}

			inputId = 
				std::regex_replace(inputId,
					std::regex(std::format("^({}|{}|{}|{})_(\\d+)$", 
						IDS_CELL_TEXT_ID, IDS_CELL_TEXT_VALUE, IDS_CELL_TEXT_HEIGHT, IDS_CELL_TEXT_TOGGLE)), "$1");
            
			SketchTextHeightTab* heightTab = toolsApp->sketchTextPanel->getTextHeightTab().get();

			if (heightTab->getActions().find(inputId) !=
				heightTab->getActions().end()) {
				heightTab->getActions()[inputId](eventArgs);
			} else {
				LOG_INFO("Unknown inputId: " + inputId);
			}

			return;
		}
	}
}
