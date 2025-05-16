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
		/// <summary>Adds a text size tab to 'tabInputs'.</summary>
		///
		/// <param name="inputs">   The inputs.</param>
		/// <param name="tabInputs">[in,out] The tab inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::addTextSizeTab(const Ptr<CommandInputs>& inputs, Ptr<CommandInputs>& tabInputs) {
			Ptr<TabCommandInput> textHeightTab = 
				inputs->addTabCommandInput(IDS_ITEM_TAB_TEXTHEIGHT, 
					LoadStringFromResource(IDS_LABEL_TAB_TEXTHEIGHT), IDS_PATH_ICON_SKETCH_TEXTHEIGHT);

			tabInputs = textHeightTab->children();
			if (!tabInputs) {
				LOG_ERROR("Failed to add tab command input");
				return false;
			}

			return true;
		}

		/// <summary>Adds a sketch drop down to 'dropdown'.</summary>
		///
		/// <param name="inputs">  The inputs.</param>
		/// <param name="dropdown">[in,out] The dropdown.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown) {
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
		bool SketchTextPanel::addTextSizeFilter(const Ptr<CommandInputs>& inputs) {
			Ptr<TextBoxCommandInput> textInput = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTHEIGHT_FILTER, "",
					LoadStringFromResource(IDS_LABEL_FILTER_TEXTHEIGHT_RANGE), 1, true);
			if (!textInput) {
				LOG_ERROR("Failed to add text box command input");
				return false;
			}
			Ptr<ValueInput> minTextHeightInput = ValueInput::createByReal(0.0);
			Ptr<ValueInput> maxTextHeightInput = ValueInput::createByReal(1.0);

			Ptr<ValueCommandInput> minTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTHEIGHT_MIN, 
					LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_MIN), IDS_UNIT_MM, minTextHeightInput);
			if (!minTextHeight) {
				LOG_ERROR("Failed to add min text height command input");
				return false;
			}
			Ptr<ValueCommandInput> maxTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTHEIGHT_MAX, 
					LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_MAX), IDS_UNIT_MM, maxTextHeightInput);
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
		bool SketchTextPanel::addTextSizeMatch(const Ptr<CommandInputs>& inputs) {
			Ptr<TextBoxCommandInput> textHeightMatch = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTHEIGHT_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_MATCH), "", 1, true);
			if (!textHeightMatch) {
				LOG_ERROR("Failed to add text size match command input");
				return false;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return false;
			}
			Ptr<TableCommandInput> tableInput = 
				inputs->addTableCommandInput( IDS_ITEM_TEXTHEIGHT_TABLE, 
				LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_TABLE), 
				3, // Anzahl der Spalten
				"1:3:1" // Spaltenverhältnis: ID, Text, Höhe
			);

			if (!tableInput) {
				LOG_ERROR("Failed to add TableCommandInput");
				return false;
			}

			sketchTextMap_.clear();

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

				sketchTextMap_[textValueId] = text;
				sketchTextMap_[textHeightId] = text;
			}

			return true;
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles the creation </para>
		/// <para>of a command by setting up its user interface elements,including tabs, dropdowns, </para>
		/// <para>and input fields for user interaction.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type CommandCreatedEventArgs,</para>
		/// <para>which likely contains information related to the creation of a command event.</para>
		/// </param>
		void SketchTextPanelCommandCreatedEventHandler::notify(const Ptr<CommandCreatedEventArgs>& eventArgs) {
			Ptr<Command> command = eventArgs->command();
			Ptr<CommandInputs> inputs = command->commandInputs();

			if (!inputs) {
				LOG_ERROR("Failed to get command inputs");
				return;
			}

			Ptr<CommandInputs> textHeightTabInputs = nullptr;
			if (!toolsApp->sketchTextPanel->addTextSizeTab(inputs, textHeightTabInputs)) {
				LOG_ERROR("Failed to add tab command input");
				return;
			}

			Ptr<DropDownCommandInput> dropdown = nullptr;
			if (!toolsApp->sketchTextPanel->addSketchDropDown(textHeightTabInputs, dropdown)) {
				LOG_ERROR("Failed to add dropdown command input");
				return;
			}

			textHeightTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTHEIGHT_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeFilter(textHeightTabInputs)) {
				LOG_ERROR("Failed to add text size filter");
				return;
			}

			textHeightTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTHEIGHT_MATCH_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeMatch(textHeightTabInputs)) {
				LOG_ERROR("Failed to add text size match");
				return;
			}

			textHeightTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTHEIGHT_MATCH_SEPARATOR);

			Ptr<ValueInput> newTextHeightInput = ValueInput::createByReal(1.0);
			Ptr<ValueCommandInput> newTextHeightCmdInput = 
				textHeightTabInputs->addValueInput(IDS_ITEM_TEXTHEIGHT_NEW, 
					LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_NEW), IDS_UNIT_MM, newTextHeightInput);

			std::string buttonLabel = LoadStringFromResource(IDS_LABEL_TEXTHEIGHT_REPLACE);

			Ptr<BoolValueCommandInput> replaceButton = 
				textHeightTabInputs->addBoolValueInput(IDS_ITEM_TEXTHEIGHT_REPLACE, buttonLabel, false);

			if (!replaceButton) {
				LOG_ERROR("Failed to add Replace button");
				return;
			}

			replaceButton->tooltip(buttonLabel);
			replaceButton->text(" " + buttonLabel);
			replaceButton->resourceFolder(IDS_PATH_ICON_SKETCH_TEXTHEIGHT);

			command->inputChanged()->add(new SketchTextPanelInputChangedEventHandler());

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match count");
				return;
			}
		}
	}
}
