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
		bool SketchTextHeightTab::initialize(Ptr<Command> command, const Ptr<TabCommandInput>& tabInput) {
			actions_.insert({ std::string(IDS_ITEM_DROPDOWN_SELECT_SKETCH), &SketchTextHeightTab::dropDownSelected});
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_REPLACE), &SketchTextHeightTab::textHeightReplaced});
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_MIN), &SketchTextHeightTab::textHeightChanged });
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_MAX), &SketchTextHeightTab::textHeightChanged });
			actions_.insert({ std::string(IDS_CELL_TEXT_ID), &SketchTextHeightTab::textIdCellSelected });
			actions_.insert({ std::string(IDS_CELL_TEXT_VALUE), &SketchTextHeightTab::textValueCellSelected });
			actions_.insert({ std::string(IDS_CELL_TEXT_HEIGHT), &SketchTextHeightTab::textHeightCellSelected });

			Ptr<CommandInputs> tabInputs = tabInput->children();
			if (!tabInputs) {
				LOG_ERROR("Invalid tab inputs");
				return false;
			}
			
			// Add the sketch drop down to the tab inputs
			Ptr<DropDownCommandInput> sketchDropDown = nullptr;
			if (!addSketchDropDown(tabInputs, sketchDropDown)) {
				LOG_ERROR("Failed to add sketch drop down");
				return false;
			}

			tabInputs->addSeparatorCommandInput(IDS_ITEM_TEXT_HEIGHT_SEPARATOR);

			if (!addTextHeightFilter(tabInputs)) {
				LOG_ERROR("Failed to add text size filter");
				return false;
			}

			tabInputs->addSeparatorCommandInput(IDS_ITEM_TEXT_HEIGHT_MATCH_SEPARATOR);

			if (!addTextHeightMatch(tabInputs)) {
				LOG_ERROR("Failed to add text size match");
				return false;
			}

			tabInputs->addSeparatorCommandInput(IDS_ITEM_TEXT_HEIGHT_MATCH_SEPARATOR);

			Ptr<ValueInput> newTextHeightInput = ValueInput::createByReal(1.0);
			Ptr<ValueCommandInput> newTextHeightCmdInput = 
				tabInputs->addValueInput(IDS_ITEM_TEXT_HEIGHT_NEW, 
					LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_NEW), IDS_UNIT_MM, newTextHeightInput);

			std::string buttonLabel = LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_REPLACE);

			Ptr<BoolValueCommandInput> replaceButton = 
				tabInputs->addBoolValueInput(IDS_ITEM_TEXT_HEIGHT_REPLACE, buttonLabel, false);

			if (!replaceButton) {
				LOG_ERROR("Failed to add Replace button");
				return false;
			}

			replaceButton->tooltip(buttonLabel);
			replaceButton->text(" " + buttonLabel);
			replaceButton->resourceFolder(IDS_PATH_ICON_SKETCH_TEXT_HEIGHT);

			command->inputChanged()->add(new SketchTextHeightTabInputChangedEventHandler());
		}

		/// <summary>Adds a sketch drop down to 'dropdown'.</summary>
		///
		/// <param name="inputs">  The inputs.</param>
		/// <param name="dropdown">[in,out] The dropdown.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextHeightTab::addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown) {
			if (!inputs) {
				LOG_ERROR("Invalid inputs");
				return false;
			}
			std::string selectSketchLabel = LoadStringFromResource(IDS_LABEL_SELECT_SKETCH);
			dropdown = 
				inputs->addDropDownCommandInput(IDS_ITEM_DROPDOWN_SELECT_SKETCH, selectSketchLabel, DropDownStyles::LabeledIconDropDownStyle);
			Ptr<Design> design = toolsApp->activeProduct();
			Ptr<Sketches> sketches = design->rootComponent()->sketches();
			for (size_t i = 0; i < sketches->count(); ++i) {
				Ptr<Sketch> sketch = sketches->item(i);
				dropdown->listItems()->add(sketch->name(), (i == 0) ? true : false, IDS_PATH_ICON_SKETCH_TEXT);
			}
			return true;
		}

		/// <summary>Adds a text size filter.</summary>
		///
		/// <param name="inputs">The inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextHeightTab::addTextHeightFilter(const Ptr<CommandInputs>& inputs) {
			Ptr<TextBoxCommandInput> textInput = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXT_HEIGHT_FILTER, "",
					LoadStringFromResource(IDS_LABEL_FILTER_TEXT_HEIGHT_RANGE), 1, true);
			if (!textInput) {
				LOG_ERROR("Failed to add text box command input");
				return false;
			}
			Ptr<ValueInput> minTextHeightInput = ValueInput::createByReal(0.0);
			Ptr<ValueInput> maxTextHeightInput = ValueInput::createByReal(1.0);

			Ptr<ValueCommandInput> minTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXT_HEIGHT_MIN, 
					LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_MIN), IDS_UNIT_MM, minTextHeightInput);
			if (!minTextHeight) {
				LOG_ERROR("Failed to add min text height command input");
				return false;
			}
			Ptr<ValueCommandInput> maxTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXT_HEIGHT_MAX, 
					LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_MAX), IDS_UNIT_MM, maxTextHeightInput);
			if (!maxTextHeight) {
				LOG_ERROR("Failed to add max text height command input");
				return false;
			}
			return true;
		}

		/// <summary>Adds a text size match.</summary>
		///
		/// <param name="inputs">The inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextHeightTab::addTextHeightMatch(const Ptr<CommandInputs>& inputs) {
			Ptr<TextBoxCommandInput> textHeightMatch = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXT_HEIGHT_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_MATCH), "", 1, true);
			if (!textHeightMatch) {
				LOG_ERROR("Failed to add text size match command input");
				return false;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!getTextHeightMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return false;
			}
			Ptr<TableCommandInput> tableInput = 
				inputs->addTableCommandInput( IDS_ITEM_TEXT_HEIGHT_TABLE, 
				LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_TABLE), 
				3, // Anzahl der Spalten
				"1:3:1" // Spaltenverhältnis: ID, Text, Höhe
			);

			if (!tableInput) {
				LOG_ERROR("Failed to add TableCommandInput");
				return false;
			}

			auto& idTextMap = toolsApp->sketchTextPanel->textHeightTab_->idTextMap_;

			idTextMap.clear();

			for (int i = 0; i < (int)filteredTexts.size(); ++i) {
				Ptr<SketchText> text = filteredTexts[i];

				// Column 1: ID (readonly)
				Ptr<StringValueCommandInput> idInput = inputs->addStringValueInput(
					"TextID_" + std::to_string(i), 
					"", 
					std::to_string(i + 1)
				);
				idInput->isEnabled(false); // Nur lesbar
				tableInput->addCommandInput(idInput, i, 0);

				// Column 2: Text (editable)
				std::string textValueId = "TextValue_" + std::to_string(i);
				Ptr<StringValueCommandInput> textInput = inputs->addStringValueInput(
					textValueId, 
					"", 
					text->text()
				);
				tableInput->addCommandInput(textInput, i, 1);

				// Column 3: Height (editable)
				std::string textHeightId = "TextHeight_" + std::to_string(i);
				Ptr<ValueCommandInput> heightInput = inputs->addValueInput(
					textHeightId, 
					"", 
					IDS_UNIT_MM, 
					ValueInput::createByReal(text->height())
				);
				tableInput->addCommandInput(heightInput, i, 2);

				idTextMap[textValueId] = text;
				idTextMap[textHeightId] = text;
			}

			return true;
		}

		/// <summary>
		/// <para>getTextSizeMatch retrieves and filters sketch texts based on specified minimum and maximum height,</para>
		/// <para>updating a command input with the count of matching texts.</para>
		/// </summary>
		///
		/// <param name="inputs">
		/// <para>is a constant reference to a smart pointer of type adsk::core::CommandInputs, </para>
		/// <para>which is typically used to manage command input parameters in the Autodesk Fusion 360 API.</para>
		/// </param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextHeightTab::getTextHeightMatch(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts) {
			Ptr<ValueCommandInput> minTextHeight = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MIN);
			Ptr<ValueCommandInput> maxTextHeight = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MAX);
			Ptr<TextBoxCommandInput> matchesTextHeightInput = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MATCH);
			Ptr<DropDownCommandInput> dropdown = inputs->itemById(IDS_ITEM_DROPDOWN_SELECT_SKETCH);

			Ptr<Sketch> sketch = nullptr;
			if (!toolsApp->sketchTextPanel->getSelectedSketch(dropdown, sketch)) {
				LOG_ERROR("Failed to get selected sketch");
				return false;
			}

			double minHeightValue = minTextHeight->value();
			double maxHeightValue = maxTextHeight->value();

			Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
			if (!sketchTexts) {
				LOG_ERROR("No SketchTexts found in the sketch.");
				return false;
			}

			for (size_t i = 0; i < sketchTexts->count(); ++i) {
				Ptr<SketchText> text = sketchTexts->item(i);
				Ptr<Point3D> minPoint = text->boundingBox()->minPoint();
				Ptr<Point3D> maxPoint = text->boundingBox()->maxPoint();
				if (!minPoint || !maxPoint) {
					LOG_ERROR("Failed to get bounding box points");
					return false;
				}

				Ptr<Point3D> center = Point3D::create(
					(minPoint->x() + maxPoint->x()) / 2.0,
					(minPoint->y() + maxPoint->y()) / 2.0,
					(minPoint->z() + maxPoint->z()) / 2.0
				);

				LOG_INFO("Min: " + std::to_string(minPoint->x()) + ", " + std::to_string(minPoint->y()) + ", " + std::to_string(minPoint->z()));
				LOG_INFO("Max: " + std::to_string(maxPoint->x()) + ", " + std::to_string(maxPoint->y()) + ", " + std::to_string(maxPoint->z()));
				LOG_INFO("Center: " + std::to_string(center->x()) + ", " + std::to_string(center->y()) + ", " + std::to_string(center->z()));

				if (text) {
					double textHeight = text->height();
					if (textHeight >= minHeightValue && textHeight <= maxHeightValue) {
						filteredTexts.push_back(text);
					}
				}
			}

			size_t textHeightMatchCount = filteredTexts.size();

			matchesTextHeightInput->text(std::to_string(textHeightMatchCount));

			//for (const auto& text : filteredTexts) {
			//	TRACE("Filtered Text: " + text->text());
			//}
			return true;
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
			LOG_INFO("InputChanged: " + inputId);

			auto& idTextMap = toolsApp->sketchTextPanel->textHeightTab_->idTextMap_;
			
			auto it = idTextMap.find(inputId);
			if (it != idTextMap.end()) {
				Ptr<SketchText> sketchText = it->second;
				if (sketchText) {

					LOG_INFO("Text = " + idTextMap[inputId]->text() + " - SketchText = " + sketchText->text());

					toolsApp->sketchTextPanel->addHighlightGraphics(sketchText);
					toolsApp->sketchTextPanel->focusCameraOnText(sketchText);

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
				LOG_ERROR("Failed to get text height match");
				return;
			}

			inputId = std::regex_replace(inputId, std::regex("^TextID_\\d+$"), IDS_CELL_TEXT_ID);
			inputId = std::regex_replace(inputId, std::regex("^TextValue_\\d+$"), IDS_CELL_TEXT_VALUE);
			inputId = std::regex_replace(inputId, std::regex("^TextHeight_\\d+$"), IDS_CELL_TEXT_HEIGHT);

			if (heightTab->actions_.find(inputId) != 
				heightTab->actions_.end()) {
				heightTab->actions_[inputId](eventArgs);
			} else {
				LOG_ERROR("Unknown inputId: " + inputId);
			}
			return;
		}
	}
}
