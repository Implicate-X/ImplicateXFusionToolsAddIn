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
		bool SketchTextHeightTab::initialize(Ptr<Command> command, const Ptr<TabCommandInput>& tabInput) {
			actions_.insert({ std::string(IDS_ITEM_DROPDOWN_SELECT_SKETCH), &SketchTextHeightTab::dropDownSelected});
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_REPLACE), &SketchTextHeightTab::textHeightReplaced});
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_MIN), &SketchTextHeightTab::textHeightChanged });
			actions_.insert({ std::string(IDS_ITEM_TEXT_HEIGHT_MAX), &SketchTextHeightTab::textHeightChanged });
			actions_.insert({ std::string(IDS_CELL_TEXT_ID), &SketchTextHeightTab::textIdCellSelected });
			actions_.insert({ std::string(IDS_CELL_TEXT_VALUE), &SketchTextHeightTab::textValueCellSelected });
			actions_.insert({ std::string(IDS_CELL_TEXT_HEIGHT), &SketchTextHeightTab::textHeightCellSelected });
			actions_.insert({ std::string(IDS_CELL_TEXT_TOGGLE), &SketchTextHeightTab::textToggleCellSelected });

			textValueCellInput_ = nullptr;

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

			if (!addTextHeightMatchTable(tabInputs)) {
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

			return true;
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
		bool SketchTextHeightTab::addTextHeightMatchTable(const Ptr<CommandInputs>& inputs) {
			Ptr<TextBoxCommandInput> textHeightMatch = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXT_HEIGHT_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_MATCH), "", 1, true);
			if (!textHeightMatch) {
				LOG_ERROR("Failed to add text size match command input");
				return false;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!getTextHeightMatchItems(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return false;
			}
			Ptr<TableCommandInput> tableInput = 
				inputs->addTableCommandInput( IDS_ITEM_TEXT_HEIGHT_TABLE, 
				LoadStringFromResource(IDS_LABEL_TEXT_HEIGHT_TABLE), 
				4, // Anzahl der Spalten
				"1:4:2:1" // Spaltenverhältnis: ID, Text, Höhe, Toggle
			);

			if (!tableInput) {
				LOG_ERROR("Failed to add TableCommandInput");
				return false;
			}

			tableInput->hasGrid(false);
			tableInput->tablePresentationStyle(TablePresentationStyles::itemBorderTablePresentationStyle);
			tableInput->columnSpacing(1);
			tableInput->rowSpacing(1);

			SketchTextHeightTab* heightTab = toolsApp->sketchTextPanel->getTextHeightTab().get();

			auto& idTextMap = heightTab->idTextMap_;

			idTextMap.clear();

			for (unsigned int row = 0; row < (unsigned int)filteredTexts.size(); ++row) {
				unsigned int key = row + 1;

				Ptr<SketchText> sketchText = filteredTexts[row];

				// Column 1: ID
				Ptr<StringValueCommandInput> idInput = inputs->addStringValueInput(
					std::format("{}_{}", IDS_CELL_TEXT_ID, key), "", std::to_string(key));
				idInput->isReadOnly(true);
				tableInput->addCommandInput(idInput, row, 0);

				// Column 2: Text
				Ptr<StringValueCommandInput> textInput = inputs->addStringValueInput(
					std::format("{}_{}", IDS_CELL_TEXT_VALUE, key),	"",  sketchText->text());
				textInput->isReadOnly(true);
				tableInput->addCommandInput(textInput, row, 1);

				// Column 3: Height
				double heightCm = sketchText->height();
				double heightMm = heightCm * 10.0;
				std::ostringstream heightString;
				heightString << std::fixed << std::setprecision(2) << heightMm << " " << IDS_UNIT_MM;
				Ptr<StringValueCommandInput> heightInput = inputs->addStringValueInput(
					std::format("{}_{}", IDS_CELL_TEXT_HEIGHT, key), "", heightString.str()	);
				heightInput->isReadOnly(true);
				tableInput->addCommandInput(heightInput, row, 2);

				// Column 4: Toggle (checkbox)
				Ptr<BoolValueCommandInput> toggleInput = inputs->addBoolValueInput(
					std::format("{}_{}", IDS_CELL_TEXT_TOGGLE, key), "", true, "", false);
				tableInput->addCommandInput(toggleInput, row, 3);

				idTextMap[key] = sketchText;
			}

			return true;
		}
	}
}
