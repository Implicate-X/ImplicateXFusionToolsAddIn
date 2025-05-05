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
		/// <para>The initialize function in the SketchTextPanel class is responsible</para>
		/// <para>for setting up the palette by invoking the createCommand method.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
        bool SketchTextPanel::initialize() {  
			idHandlers_.insert({ std::string(IDS_ITEM_DROPDOWN_SELECT_SKETCH), &SketchTextPanel::handleDropDownSelect});
			idHandlers_.insert({ std::string(IDS_ITEM_TEXTSIZE_REPLACE), &SketchTextPanel::handleTextSizeReplace});
			idHandlers_.insert({ std::string(IDS_ITEM_TEXTSIZE_MIN), &SketchTextPanel::handleTextSizeChange });
			idHandlers_.insert({ std::string(IDS_ITEM_TEXTSIZE_MAX), &SketchTextPanel::handleTextSizeChange });
           return createCommand();  
        }

		/// <summary>
		/// <para>The terminate function in the SketchTextPanel class is a boolean method </para>
		/// <para>that calls and returns the result of the removeCommand function.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::terminate() {
			return removeCommand();
		}

		/// <summary>
		/// <para>The createCommand function in the SketchTextPanel class is responsible</para>
		/// <para>for creating and executing a new command definition for sketch text definitions.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::createCommand() {
			if (!removeCommand()) {
				return false;
			}

			if (toolsUI) {
				std::string selectSketchLabel = LoadStringFromResource(IDS_LABEL_SELECT_SKETCH);
				Ptr<CommandDefinition> sketchTextPanelCommandDef = 
					toolsUI->commandDefinitions()
							->addButtonDefinition(IDS_CMD_SKETCH_TEXT_DEFINITIONS, selectSketchLabel, selectSketchLabel);
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

		/// <summary>
		/// <para>The removeCommand function in the SketchTextPanel class is for deleting </para>
		/// <para>a specific command definition associated with the sketch text panel.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
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

		/// <summary>Adds a text size tab to 'tabInputs'.</summary>
		///
		/// <param name="inputs">   The inputs.</param>
		/// <param name="tabInputs">[in,out] The tab inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::addTextSizeTab(const Ptr<CommandInputs>& inputs, Ptr<CommandInputs>& tabInputs) {
			Ptr<TabCommandInput> textSizeTab = 
				inputs->addTabCommandInput(IDS_ITEM_TAB_TEXTSIZE, 
					LoadStringFromResource(IDS_LABEL_TAB_TEXTSIZE), IDS_PATH_ICON_SKETCH_TEXTSIZE);

			tabInputs = textSizeTab->children();
			if (!tabInputs) {
				Application::log("Failed to add tab command input");
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
				Application::log("AddSketchDropDown: Invalid inputs");
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
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_FILTER, "",
					LoadStringFromResource(IDS_LABEL_FILTER_TEXTSIZE_RANGE), 1, true);
			if (!textInput) {
				Application::log("Failed to add text box command input");
				return false;
			}
			Ptr<ValueInput> minTextHeightInput = ValueInput::createByReal(0.0);
			Ptr<ValueInput> maxTextHeightInput = ValueInput::createByReal(1.0);

			Ptr<ValueCommandInput> minTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTSIZE_MIN, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MIN), IDS_UNIT_MM, minTextHeightInput);
			if (!minTextHeight) {
				Application::log("Failed to add min text height command input");
				return false;
			}
			Ptr<ValueCommandInput> maxTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTSIZE_MAX, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MAX), IDS_UNIT_MM, maxTextHeightInput);
			if (!maxTextHeight) {
				Application::log("Failed to add max text height command input");
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
			Ptr<TextBoxCommandInput> textSizeMatch = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MATCH), "", 1, true);
			if (!textSizeMatch) {
				Application::log("Failed to add text size match command input");
				return false;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				Application::log("addTextSizeMatch: Failed to get text size match");
				return false;
			}
			Ptr<TableCommandInput> tableInput = 
				inputs->addTableCommandInput( IDS_ITEM_TEXTSIZE_TABLE, 
				LoadStringFromResource(IDS_LABEL_TEXTSIZE_TABLE), 
				3, // Anzahl der Spalten
				"1:3:1" // Spaltenverhältnis: ID, Text, Höhe
			);

			if (!tableInput) {
				Application::log("Failed to add TableCommandInput");
				return false;
			}

			for (size_t i = 0; i < filteredTexts.size(); ++i) {
				Ptr<SketchText> text = filteredTexts[i];

				// Spalte 1: ID (nur lesbar)
				Ptr<StringValueCommandInput> idInput = inputs->addStringValueInput(
					"TextID_" + std::to_string(i), 
					"", 
					std::to_string(i + 1)
				);
				idInput->isEnabled(false); // Nur lesbar
				tableInput->addCommandInput(idInput, i, 0);

				// Spalte 2: Text (bearbeitbar)
				Ptr<StringValueCommandInput> textInput = inputs->addStringValueInput(
					"TextValue_" + std::to_string(i), 
					"", 
					text->text()
				);
				tableInput->addCommandInput(textInput, i, 1);

				// Spalte 3: Höhe (bearbeitbar)
				Ptr<ValueCommandInput> heightInput = inputs->addValueInput(
					"TextHeight_" + std::to_string(i), 
					"", 
					IDS_UNIT_MM, 
					ValueInput::createByReal(text->height())
				);
				tableInput->addCommandInput(heightInput, i, 2);
			}

			return true;
		}

		/// <summary>Gets selected sketch.</summary>
		///
		/// <param name="dropdown">The dropdown.</param>
		/// <param name="sketch">  [in,out] The sketch.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::getSelectedSketch(const Ptr<DropDownCommandInput>& dropdown, Ptr<Sketch>& sketch) {
			if (!dropdown) {
				Application::log("getSelectedSketch: Invalid dropdown input");
				return false;
			}
			Ptr<ListItem> selectedItem = dropdown->selectedItem();
			if (selectedItem) {
				std::string selectedSketchName = selectedItem->name();
				Ptr<Design> design = toolsApp->activeProduct();
				Ptr<Component> root = design->rootComponent();
				Ptr<Sketches> sketches = root->sketches();
				if (!sketches) {
					Application::log("getSelectedSketch: No sketches found");
					return false;
				}
				if (sketches->count() <= 0) {
					Application::log("getSelectedSketch: No sketches found");
					return false;
				}
				sketch = sketches->itemByName(selectedSketchName);
				if (!sketch) {
					Application::log("getSelectedSketch: Sketch not found");
					return false;
				}
			}
			else {
				Application::log("getSelectedSketch: No sketch selected");
				return false;
			}
			return true;
		}

		/// <summary>
		/// <para>The handleDropDownSelect function processes the selection of a dropdown menu item related to sketches,</para>
		/// <para>logging the selected sketch name and aligning the model to the corresponding sketch's XY plane if found.</para>
		/// </summary>
		///
		/// <param name="eventArgs">
		/// <para> is a constant reference to a pointer of type InputChangedEventArgs </para>
		/// <para>which likely encapsulates information about changes to input events in the Autodesk API.</para>
		/// </param>
		void SketchTextPanel::handleDropDownSelect(const Ptr<InputChangedEventArgs>& eventArgs) {
			Application::log("handleDropDownSelect");
			Ptr<DropDownCommandInput> dropdown = eventArgs->input();
			Ptr<Sketch> sketch = nullptr;
			if (!toolsApp->sketchTextPanel->getSelectedSketch(dropdown, sketch)) {
				Application::log("handleDropDownSelect: Failed to get selected sketch");
				return;
			}
			toolsApp->sketchTextPanel->alignModelToSketchXYPlane(sketch);
		}

		/// <summary>Handles the text size replace described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::handleTextSizeReplace(const Ptr<InputChangedEventArgs>& eventArgs) {
			Application::log("handleTextSizeReplace");
		}

		/// <summary>Handles the text size change described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::handleTextSizeChange(const Ptr<InputChangedEventArgs>& eventArgs) {
			Application::log("handleTextSizeChange");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				Application::log("handleTextSizeChange: Failed to get text size match");
				return;
			}
		}

		/// <summary>Gets text size match.</summary>
		///
		/// <param name="inputs">The inputs.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::getTextSizeMatch(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts) {
			Ptr<ValueCommandInput> minTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MIN);
			Ptr<ValueCommandInput> maxTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MAX);
			Ptr<TextBoxCommandInput> matchesTextHeightInput = inputs->itemById(IDS_ITEM_TEXTSIZE_MATCH);
			Ptr<DropDownCommandInput> dropdown = inputs->itemById(IDS_ITEM_DROPDOWN_SELECT_SKETCH);

			Ptr<Sketch> sketch = nullptr;
			if (!toolsApp->sketchTextPanel->getSelectedSketch(dropdown, sketch)) {
				Application::log("getTextSizeMatchCount: Failed to get selected sketch");
				return false;
			}

			double minHeightValue = minTextHeight->value();
			double maxHeightValue = maxTextHeight->value();

			Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
			if (!sketchTexts) {
				Application::log("No SketchTexts found in the sketch.");
				return false;
			}

			for (size_t i = 0; i < sketchTexts->count(); ++i) {
				Ptr<SketchText> text = sketchTexts->item(i);
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
			//	Application::log("Filtered Text: " + text->text());
			//}
			return true;
		}

		/// <summary>
		/// <para>The alignModelToSketchXYPlane function aligns the model's camera view to the XY plane </para>
		/// <para>of a specified sketch, ensuring the camera is positioned correctly.</para>
		/// </summary>
		///
		/// <param name="sketch">
		/// <para> is a constant reference to a smart pointer that manages an instance of the Sketch class, </para>
		/// <para>allowing for efficient memory management and access to sketch-related functionalities.</para>
		/// </param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::alignModelToSketchXYPlane(const Ptr<Sketch>& sketch) {
			if (!sketch) {
				Application::log("Invalid sketch provided.");
				return false;
			}

			Ptr<Vector3D> sketchXDirection = sketch->xDirection();
			Ptr<Vector3D> sketchYDirection = sketch->yDirection();
			if (!sketchXDirection || !sketchYDirection) {
				Application::log("Failed to retrieve sketch directions.");
				return false;
			}

			Ptr<Vector3D> sketchZDirection = sketchXDirection->crossProduct(sketchYDirection);
			if (!sketchZDirection) {
				Application::log("Failed to calculate sketch Z direction.");
				return false;
			}

			Ptr<Point3D> sketchOrigin = sketch->origin();
			if (!sketchOrigin) {
				Application::log("Failed to retrieve sketch origin.");
				return false;
			}

			Ptr<BoundingBox3D> boundingBox = sketch->boundingBox();
			if (!boundingBox) {
				Application::log("Failed to retrieve bounding box of the sketch.");
				return false;
			}

			Ptr<Point3D> minPoint = boundingBox->minPoint();
			Ptr<Point3D> maxPoint = boundingBox->maxPoint();
			if (!minPoint || !maxPoint) {
				Application::log("Failed to retrieve bounding box points.");
				return false;
			}

			Ptr<Point3D> center = Point3D::create(
				(minPoint->x() + maxPoint->x()) / 2.0,
				(minPoint->y() + maxPoint->y()) / 2.0,
				(minPoint->z() + maxPoint->z()) / 2.0
			);

			double diagonal = sqrt(
				pow(maxPoint->x() - minPoint->x(), 2) +
				pow(maxPoint->y() - minPoint->y(), 2) +
				pow(maxPoint->z() - minPoint->z(), 2)
			);

			ViewOrientations orientation;
			if (sketchZDirection->z() > 0) {
				orientation = ViewOrientations::TopViewOrientation;
			} else {
				orientation = ViewOrientations::BottomViewOrientation;
			}
			Application::log("Orientation: " + std::to_string(static_cast<int>(orientation)));

			Ptr<Camera> camera = Camera::create();
			if (!camera) {
				Application::log("Failed to create camera.");
				return false;
			}
			
			camera->eye(Point3D::create(center->x(), center->y(), center->z() + diagonal * 1.5));
			camera->target(center);
			camera->upVector(Vector3D::create(0.0, 1.0, 0.0));
			camera->isSmoothTransition(true);
			camera->viewOrientation(orientation);

			Ptr<Viewport> viewport = Application::get()->activeViewport();
			if (!viewport) {
				Application::log("Failed to retrieve active viewport.");
				return false;
			}

			viewport->camera(camera);
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
				Application::log("Failed to get command inputs");
				return;
			}

			Ptr<CommandInputs> textSizeTabInputs = nullptr;
			if (!toolsApp->sketchTextPanel->addTextSizeTab(inputs, textSizeTabInputs)) {
				Application::log("Failed to add tab command input");
				return;
			}

			Ptr<DropDownCommandInput> dropdown = nullptr;
			if (!toolsApp->sketchTextPanel->addSketchDropDown(textSizeTabInputs, dropdown)) {
				Application::log("Failed to add dropdown command input");
				return;
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeFilter(textSizeTabInputs)) {
				Application::log("Failed to add text size filter");
				return;
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeMatch(textSizeTabInputs)) {
				Application::log("Failed to add text size match");
				return;
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR);

			Ptr<ValueInput> newTextHeightInput = ValueInput::createByReal(1.0);
			Ptr<ValueCommandInput> newTextHeightCmdInput = 
				textSizeTabInputs->addValueInput(IDS_ITEM_TEXTSIZE_NEW, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_NEW), IDS_UNIT_MM, newTextHeightInput);

			std::string buttonLabel = LoadStringFromResource(IDS_LABEL_TEXTSIZE_REPLACE);

			Ptr<BoolValueCommandInput> replaceButton = 
				textSizeTabInputs->addBoolValueInput(IDS_ITEM_TEXTSIZE_REPLACE, buttonLabel, false);

			if (!replaceButton) {
				Application::log("Failed to add Replace button");
				return;
			}

			replaceButton->tooltip(buttonLabel);
			replaceButton->text(" " + buttonLabel);
			replaceButton->resourceFolder(IDS_PATH_ICON_SKETCH_TEXTSIZE);

			command->inputChanged()->add(new SketchTextPanelInputChangedEventHandler());

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				Application::log("SketchTextPanelCommandCreatedEventHandler::notify: Failed to get text size match count");
				return;
			}
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
			Application::log("InputChanged: " + inputId);
			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs) {
				Application::log("SketchTextPanelCommandCreatedEventHandler::notify: Invalid inputs");
				return;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				Application::log("SketchTextPanelCommandCreatedEventHandler::notify: Failed to get text size match");
				return;
			}

			if (toolsApp->sketchTextPanel->idHandlers_.find(inputId) != toolsApp->sketchTextPanel->idHandlers_.end()) {
				toolsApp->sketchTextPanel->idHandlers_[inputId](eventArgs);
			} else {
				Application::log("Unknown ID: " + inputId);
			}
			return;
		}
	}
}

//void SketchTextPanelInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
//    std::string inputId = eventArgs->input()->id();
//    Application::log("InputChanged: " + inputId);
//
//    Ptr<CommandInputs> inputs = eventArgs->inputs();
//    if (!inputs) {
//        Application::log("SketchTextPanelInputChangedEventHandler::notify: Invalid inputs");
//        return;
//    }
//
//    // Überprüfe, ob die Änderung in der Tabelle erfolgt ist
//    if (inputId.find("TextID_") != std::string::npos) {
//        size_t id = std::stoul(inputId.substr(7)); // Extrahiere die ID aus "TextID_<id>"
//        toolsApp->sketchTextPanel->blinkTextById(id);
//    }
//}

//void SketchTextPanel::blinkTextById(size_t id) {
//    // Suche den Text anhand der ID
//    Ptr<SketchText> text = findTextById(id);
//    if (!text) {
//        Application::log("blinkTextById: Text not found for ID " + std::to_string(id));
//        return;
//    }
//
//    // Blink-Effekt: Sichtbarkeit des Textes ändern
//    for (int i = 0; i < 3; ++i) {
//        text->isVisible(false);
//        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 300ms warten
//        text->isVisible(true);
//        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // 300ms warten
//    }
//}

//Ptr<SketchText> SketchTextPanel::findTextById(size_t id) {
//    Ptr<Sketch> sketch = toolsApp->sketchTextPanel->sketch_;
//    if (!sketch) {
//        Application::log("findTextById: No active sketch");
//        return nullptr;
//    }
//
//    Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
//    if (!sketchTexts) {
//        Application::log("findTextById: No SketchTexts found in the sketch");
//        return nullptr;
//    }
//
//    // Iteriere durch die Texte und finde den mit der entsprechenden ID
//    for (size_t i = 0; i < sketchTexts->count(); ++i) {
//        Ptr<SketchText> text = sketchTexts->item(i);
//        if (text && i == id) { // ID entspricht dem Index
//            return text;
//        }
//    }
//
//    return nullptr;
//}