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
		bool SketchTextPanel::createCommand() {
			if (!removeCommand()) {
				return false;
			}

			if (toolsUI) {
				Ptr<CommandDefinition> sketchTextPanelCommandDef = 
					toolsUI->commandDefinitions()->addButtonDefinition(IDS_CMD_SKETCH_TEXT_DEFINITIONS,
					LoadStringFromResource(IDS_LABEL_SKETCHTEXT), "Select a sketch from the dropdown");
				if (sketchTextPanelCommandDef) {
                    auto handler = new SketchTextPanelCommandCreatedEventHandler();
					if (!sketchTextPanelCommandDef->commandCreated()->add(handler)) {
						Application::log("Failed to add event handler for SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
					if (!sketchTextPanelCommandDef->execute()) {
						Application::log("Failed to execute SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
				}
				else {
					Application::log("Failed to add button definition for SketchTextPanelCommand");
					return false;
				}
			}
			return true;
		}

		bool SketchTextPanel::removeCommand() {
			if (toolsUI) {
				Ptr<CommandDefinition> sketchTextPanelCommandDef = 
					toolsUI->commandDefinitions()->itemById(IDS_CMD_SKETCH_TEXT_DEFINITIONS);
				if (sketchTextPanelCommandDef) {
					if (!sketchTextPanelCommandDef->deleteMe()) {
						Application::log("Failed to delete SketchTextPanelCommand");
						sketchTextPanelCommandDef = nullptr;
						return false;
					}
					sketchTextPanelCommandDef = nullptr;
				}
			}
			return true;
		}

		/// <summary>
		/// <para>The initialize function in the SketchTextDefinitionsPanel class is responsible</para>
		/// <para>for setting up the palette by invoking the createCommand method.</para>
		/// </summary>
		bool SketchTextPanel::initialize() {
			return createCommand();
		}

		bool SketchTextPanel::terminate() {
			return removeCommand();
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles the creation of a command by setting up</para>
		/// <para>its user interface elements,including tabs, dropdowns, and input fields for user interaction.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>The symbol eventArgs is a constant reference to a pointer of type CommandCreatedEventArgs,</para>
		/// <para>which likely contains information related to the creation of a command event in the Autodesk API.</para>
		/// </param>
		void SketchTextPanelCommandCreatedEventHandler::notify(const Ptr<CommandCreatedEventArgs>& eventArgs) {
			Ptr<Command> command = eventArgs->command();
			Ptr<CommandInputs> inputs = command->commandInputs();

			Ptr<TabCommandInput> textSizeTab = 
				inputs->addTabCommandInput(IDS_ITEM_TAB_TEXTSIZE, 
					LoadStringFromResource(IDS_LABEL_TAB_TEXTSIZE), IDS_PATH_ICON_SKETCH_TEXTSIZE);

			if (!textSizeTab) {
				Application::log("Failed to add tab command input");
				return;
			}

			Ptr<CommandInputs> textSizeTabInputs = textSizeTab->children();

			Ptr<DropDownCommandInput> dropdown = 
				textSizeTabInputs->addDropDownCommandInput(IDS_ITEM_DROPDOWN_SELECT_SKETCH, 
					LoadStringFromResource(IDS_LABEL_SELECT_SKETCH), DropDownStyles::LabeledIconDropDownStyle);

			Ptr<Design> design = toolsApp->activeProduct();
			Ptr<Sketches> sketches = design->rootComponent()->sketches();

			for (size_t i = 0; i < sketches->count(); ++i) {
				Ptr<Sketch> sketch = sketches->item(i);
				dropdown->listItems()->add(sketch->name(), (i==0)?true:false, IDS_PATH_ICON_SKETCH_TEXT);
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_SEPARATOR);

			Ptr<TextBoxCommandInput> textInput = 
				textSizeTabInputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_FILTER, "",
					LoadStringFromResource(IDS_LABEL_FILTER_TEXTSIZE_RANGE), 1, true);

			Ptr<ValueInput> minTextHeightInput = ValueInput::createByReal(0.0);
			Ptr<ValueInput> maxTextHeightInput = ValueInput::createByReal(1.0);

			Ptr<ValueCommandInput> minTextHeight = 
				textSizeTabInputs->addValueInput(IDS_ITEM_TEXTSIZE_MIN, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MIN), IDS_UNIT_MM, minTextHeightInput);
			Ptr<ValueCommandInput> maxTextHeight = 
				textSizeTabInputs->addValueInput(IDS_ITEM_TEXTSIZE_MAX, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MAX), IDS_UNIT_MM, maxTextHeightInput);

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR);

			Ptr<TextBoxCommandInput> textSizeMatch = 
				textSizeTabInputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MATCH), "", 1, true);

			Ptr<ValueInput> newTextHeightInput = ValueInput::createByReal(1.0);
			Ptr<ValueCommandInput> newTextHeightCmdInput = 
				textSizeTabInputs->addValueInput(IDS_ITEM_TEXTSIZE_NEW, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_NEW), IDS_UNIT_MM, newTextHeightInput);

			command->inputChanged()->add(new SketchTextPanelInputChangedEventHandler());
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles input change events by updating</para>
		/// <para>a read-only text fieldwith the count of sketch texts based on the selected item from a dropdown menu.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>The symbol eventArgs is a constant reference to a pointer of type InputChangedEventArgs,</para>
		/// <para>which is typically used to handle input change events in Autodesk's API.</para>
		/// </param>
		void SketchTextPanelInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
			Ptr<CommandInputs> inputs = eventArgs->inputs();
			Ptr<DropDownCommandInput> dropdown = inputs->itemById(IDS_ITEM_DROPDOWN_SELECT_SKETCH);
			Ptr<ListItem> selectedItem = dropdown->selectedItem();
			Ptr<ValueCommandInput> minTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MIN);
			Ptr<ValueCommandInput> maxTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MAX);
			Ptr<TextBoxCommandInput> matchesTextHeightInput = inputs->itemById(IDS_ITEM_TEXTSIZE_MATCH);

			if (selectedItem) {
				std::string selectedSketchName = selectedItem->name();
				Application::get()->log("InputChanged: " + selectedSketchName);
				double minHeightValue = minTextHeight->value();
				double maxHeightValue = maxTextHeight->value();

				// Berechnen der Anzahl der gefundenen Sketchtexte
				int textHeightMatchCount = 89;
				// Placeholder for actual implementation to count the found texts
				// foundTextsCount = ...

				// Aktualisieren des Nur-Lesen-Felds mit der Anzahl der gefundenen Sketchtexte
				matchesTextHeightInput->text(std::to_string(textHeightMatchCount));

				// Handle the selected sketch
				// Placeholder for actual implementation
			}
			else {
				Application::get()->log("InputChanged: No sketch selected");
			}
		}
	}
}
