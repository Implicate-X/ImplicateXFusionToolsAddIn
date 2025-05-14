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
						LOG_ERROR("Failed to add event handler for SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
					if (!sketchTextPanelCommandDef->execute()) {
						LOG_ERROR("Failed to execute SketchTextPanelCommandCreatedEventHandler");
						return false;
					}
				}
				else {
					LOG_ERROR("Failed to add button definition for SketchTextPanelCommand");
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
						LOG_ERROR("Failed to delete SketchTextPanelCommand");
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
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_FILTER, "",
					LoadStringFromResource(IDS_LABEL_FILTER_TEXTSIZE_RANGE), 1, true);
			if (!textInput) {
				LOG_ERROR("Failed to add text box command input");
				return false;
			}
			Ptr<ValueInput> minTextHeightInput = ValueInput::createByReal(0.0);
			Ptr<ValueInput> maxTextHeightInput = ValueInput::createByReal(1.0);

			Ptr<ValueCommandInput> minTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTSIZE_MIN, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MIN), IDS_UNIT_MM, minTextHeightInput);
			if (!minTextHeight) {
				LOG_ERROR("Failed to add min text height command input");
				return false;
			}
			Ptr<ValueCommandInput> maxTextHeight = 
				inputs->addValueInput(IDS_ITEM_TEXTSIZE_MAX, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MAX), IDS_UNIT_MM, maxTextHeightInput);
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
			Ptr<TextBoxCommandInput> textSizeMatch = 
				inputs->addTextBoxCommandInput(IDS_ITEM_TEXTSIZE_MATCH, 
					LoadStringFromResource(IDS_LABEL_TEXTSIZE_MATCH), "", 1, true);
			if (!textSizeMatch) {
				LOG_ERROR("Failed to add text size match command input");
				return false;
			}
			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
				return false;
			}
			Ptr<TableCommandInput> tableInput = 
				inputs->addTableCommandInput( IDS_ITEM_TEXTSIZE_TABLE, 
				LoadStringFromResource(IDS_LABEL_TEXTSIZE_TABLE), 
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

		/// <summary>Gets selected sketch.</summary>
		///
		/// <param name="dropdown">The dropdown.</param>
		/// <param name="sketch">  [in,out] The sketch.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::getSelectedSketch(const Ptr<DropDownCommandInput>& dropdown, Ptr<Sketch>& sketch) {
			if (!dropdown) {
				LOG_ERROR("getSelectedSketch: Invalid dropdown input");
				return false;
			}
			Ptr<ListItem> selectedItem = dropdown->selectedItem();
			if (selectedItem) {
				std::string selectedSketchName = selectedItem->name();
				Ptr<Design> design = toolsApp->activeProduct();
				Ptr<Component> root = design->rootComponent();
				Ptr<Sketches> sketches = root->sketches();
				if (!sketches) {
					LOG_ERROR("No sketches found");
					return false;
				}
				if (sketches->count() <= 0) {
					LOG_ERROR("No sketches found");
					return false;
				}
				sketch = sketches->itemByName(selectedSketchName);
				if (!sketch) {
					LOG_ERROR("Sketch not found");
					return false;
				}
			}
			else {
				LOG_ERROR("No sketch selected");
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
			LOG_INFO("handleDropDownSelect");
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
		void SketchTextPanel::handleTextSizeReplace(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("handleTextSizeReplace");
		}

		/// <summary>Handles the text size change described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::handleTextSizeChange(const Ptr<InputChangedEventArgs>& eventArgs) {
			LOG_INFO("handleTextSizeChange");

			Ptr<CommandInputs> inputs = eventArgs->inputs();
			if (!inputs)
				return;

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match");
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
		/// <para>The getTextPoints function retrieves the bounding box coordinates and center point </para>
		/// <para>of a given sketch text, returning false if the input is invalid or no valid points are found.</para>
		/// </summary>
		///
		/// <param name="sketchText"> The sketch text.</param>
		/// <param name="centerPoint">[in,out] The center point.</param>
		/// <param name="minPoint">   [in,out] The minimum point.</param>
		/// <param name="maxPoint">   [in,out] The maximum point.</param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextPanel::getTextPoints(const Ptr<SketchText>& sketchText, Ptr<Point3D>& centerPoint, Ptr<Point3D>& minPoint, Ptr<Point3D>& maxPoint) {
			if (!sketchText) {
				LOG_ERROR("Invalid SketchText");
				return false;
			}

			Ptr<SketchTextDefinition> textDef = sketchText->definition();
			Ptr<MultiLineTextDefinition> multiLineTextDef = textDef;
			if (!multiLineTextDef) {
				LOG_ERROR("No MultiLineTextDefinition found for the sketch text.");
				return false;
			}

			std::vector<Ptr<SketchLine>> lines = multiLineTextDef->rectangleLines();
			if (lines.size() != 4) {
				LOG_ERROR("rectangleLines does not have the expected number of lines.");
				return false;
			}

			double minX = (std::numeric_limits<double>::max)();
			double minY = (std::numeric_limits<double>::max)();
			double maxX = (std::numeric_limits<double>::lowest)();
			double maxY = (std::numeric_limits<double>::lowest)();

			for (const auto& line : lines) {
				if (!line) continue;

				Ptr<Point3D> startPoint = line->startSketchPoint()->geometry();
				Ptr<Point3D> endPoint = line->endSketchPoint()->geometry();

				if (startPoint) {
					minX = (std::min)(minX, startPoint->x());
					minY = (std::min)(minY, startPoint->y());
					maxX = (std::max)(maxX, startPoint->x());
					maxY = (std::max)(maxY, startPoint->y());
				}

				if (endPoint) {
					minX = (std::min)(minX, endPoint->x());
					minY = (std::min)(minY, endPoint->y());
					maxX = (std::max)(maxX, endPoint->x());
					maxY = (std::max)(maxY, endPoint->y());
				}
			}
			if (minX == (std::numeric_limits<double>::max)() || minY == (std::numeric_limits<double>::max)() ||
				maxX == (std::numeric_limits<double>::lowest)() || maxY == (std::numeric_limits<double>::lowest)()) {
				LOG_INFO("No valid points found for the bounding box.");
				return false;
			}
			double centerX = (minX + maxX) / 2.0;
			double centerY = (minY + maxY) / 2.0;

			minPoint = Point3D::create(minX, minY, 0.0);
			maxPoint = Point3D::create(maxX, maxY, 0.0);
			centerPoint = Point3D::create(centerX, centerY, 0.0);

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
				LOG_ERROR("Invalid sketch provided.");
				return false;
			}

			Ptr<Vector3D> sketchXDirection = sketch->xDirection();
			Ptr<Vector3D> sketchYDirection = sketch->yDirection();
			if (!sketchXDirection || !sketchYDirection) {
				LOG_ERROR("Failed to retrieve sketch directions.");
				return false;
			}

			Ptr<Vector3D> sketchZDirection = sketchXDirection->crossProduct(sketchYDirection);
			if (!sketchZDirection) {
				LOG_ERROR("Failed to calculate sketch Z direction.");
				return false;
			}

			Ptr<Point3D> sketchOrigin = sketch->origin();
			if (!sketchOrigin) {
				LOG_ERROR("Failed to retrieve sketch origin.");
				return false;
			}

			Ptr<BoundingBox3D> boundingBox = sketch->boundingBox();
			if (!boundingBox) {
				LOG_ERROR("Failed to retrieve bounding box of the sketch.");
				return false;
			}

			Ptr<Point3D> minPoint = boundingBox->minPoint();
			Ptr<Point3D> maxPoint = boundingBox->maxPoint();
			if (!minPoint || !maxPoint) {
				LOG_ERROR("Failed to retrieve bounding box points.");
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
			LOG_INFO("Orientation: " + std::to_string(static_cast<int>(orientation)));

			Ptr<Camera> camera = Camera::create();
			if (!camera) {
				LOG_ERROR("Failed to create camera.");
				return false;
			}
			
			camera->eye(Point3D::create(center->x(), center->y(), center->z() + diagonal * 1.5));
			camera->target(center);
			camera->upVector(Vector3D::create(0.0, 1.0, 0.0));
			camera->isSmoothTransition(true);
			camera->viewOrientation(orientation);

			Ptr<Viewport> viewport = Application::get()->activeViewport();
			if (!viewport) {
				LOG_ERROR("Failed to retrieve active viewport.");
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
				LOG_ERROR("Failed to get command inputs");
				return;
			}

			Ptr<CommandInputs> textSizeTabInputs = nullptr;
			if (!toolsApp->sketchTextPanel->addTextSizeTab(inputs, textSizeTabInputs)) {
				LOG_ERROR("Failed to add tab command input");
				return;
			}

			Ptr<DropDownCommandInput> dropdown = nullptr;
			if (!toolsApp->sketchTextPanel->addSketchDropDown(textSizeTabInputs, dropdown)) {
				LOG_ERROR("Failed to add dropdown command input");
				return;
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeFilter(textSizeTabInputs)) {
				LOG_ERROR("Failed to add text size filter");
				return;
			}

			textSizeTabInputs->addSeparatorCommandInput(IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR);

			if (!toolsApp->sketchTextPanel->addTextSizeMatch(textSizeTabInputs)) {
				LOG_ERROR("Failed to add text size match");
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
				LOG_ERROR("Failed to add Replace button");
				return;
			}

			replaceButton->tooltip(buttonLabel);
			replaceButton->text(" " + buttonLabel);
			replaceButton->resourceFolder(IDS_PATH_ICON_SKETCH_TEXTSIZE);

			command->inputChanged()->add(new SketchTextPanelInputChangedEventHandler());

			std::vector<Ptr<SketchText>> filteredTexts;
			if (!toolsApp->sketchTextPanel->getTextSizeMatch(inputs, filteredTexts)) {
				LOG_ERROR("Failed to get text size match count");
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
			LOG_INFO("InputChanged: " + inputId);

			auto it = toolsApp->sketchTextPanel->sketchTextMap_.find(inputId);
			if (it != toolsApp->sketchTextPanel->sketchTextMap_.end()) {
				Ptr<SketchText> sketchText = it->second;
				if (sketchText) {

					LOG_INFO("Text = " + toolsApp->sketchTextPanel->sketchTextMap_[inputId]->text() + " - SketchText = " + sketchText->text());

					//toolsApp->sketchTextPanel->addHighlightGraphics(sketchText);
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

			//inputId = std::regex_replace(
			//	inputId,
			//	std::regex("^(TextID_|TextValue_|TextHeight_)\\d+$"),
			//	[](const std::smatch& m) -> std::string {
			//		if (m[1] == "TextID_") return "textId";
			//		if (m[1] == "TextValue_") return "textValue";
			//		if (m[1] == "TextHeight_") return "textHeight";
			//		return m[0];
			//	}
			//);

			inputId = std::regex_replace(inputId, std::regex("^TextID_\\d+$"), "textID");
			inputId = std::regex_replace(inputId, std::regex("^TextValue_\\d+$"), "textValue");
			inputId = std::regex_replace(inputId, std::regex("^TextHeight_\\d+$"), "textHeight");

			if (toolsApp->sketchTextPanel->idHandlers_.find(inputId) != toolsApp->sketchTextPanel->idHandlers_.end()) {
				toolsApp->sketchTextPanel->idHandlers_[inputId](eventArgs);
			} else {
				LOG_ERROR("Unknown inputId: " + inputId);
			}
			return;
		}

		//void SketchTextPanel::blinkTextPosition(const Ptr<SketchText>& text) {
		//	if (!text) {
		//		TRACE("blinkTextPosition: Invalid SketchText");
		//		return;
		//	}

		//	// Speichere die ursprüngliche Position
		//	Ptr<Point3D> originalPosition = text->definition()->position();

		//	// Verschiebe den Text leicht
		//	Ptr<Point3D> newPosition = Point3D::create(
		//		originalPosition->x() + 0.1, 
		//		originalPosition->y(), 
		//		originalPosition->z()
		//	);
		//	text->definition()->position(newPosition);

		//	// Warte kurz
		//	std::this_thread::sleep_for(std::chrono::milliseconds(300));

		//	// Setze die ursprüngliche Position zurück
		//	text->definition()->position(originalPosition);
		//}

		void SketchTextPanel::focusCameraOnText(const Ptr<SketchText>& sketchText) {
			if (!sketchText) {
				LOG_ERROR("Invalid SketchText");
				return;
			}

			Ptr<Point3D> position = nullptr;
			Ptr<Point3D> minPoint = nullptr;
			Ptr<Point3D> maxPoint = nullptr;

			if (!toolsApp->sketchTextPanel->getTextPoints(sketchText, position, minPoint, maxPoint)) {
				LOG_ERROR("focusCameraOnText: Failed to get text points");
				return;
			}
			if (!position || !minPoint || !maxPoint) {
				LOG_ERROR("focusCameraOnText: Invalid points");
				return;
			}

			// Calculate the diagonal of the bounding box
			double diagonal = sqrt(
				pow(maxPoint->x() - minPoint->x(), 2) +
				pow(maxPoint->y() - minPoint->y(), 2) +
				pow(maxPoint->z() - minPoint->z(), 2)
			);

			double zoomFactor = 0.5; // Zoom closer to the text
			double adjustedDiagonal = diagonal * zoomFactor;

			Ptr<Camera> camera = Camera::create();
			if (!camera) {
				LOG_ERROR("Failed to create camera");
				return;
			}

			// Set the camera position (eye) and the target (centerPoint)
			camera->eye(Point3D::create(position->x(), position->y(), position->z() + adjustedDiagonal));
			camera->target(position);
			camera->upVector(Vector3D::create(0.0, 1.0, 0.0));
			camera->isSmoothTransition(true);

			Ptr<Viewport> viewport = toolsApp->activeViewport();
			if (!viewport) {
				LOG_ERROR("focusCameraOnText: Failed to get active viewport");
				return;
			}

			viewport->camera(camera);
		}

		/// <summary>Adds a highlight graphics.</summary>
		///
		/// <param name="sketchText">The sketch text.</param>
		void SketchTextPanel::addHighlightGraphics(const Ptr<SketchText>& sketchText) {
			if (!sketchText) {
				LOG_ERROR("Invalid SketchText");
				return;
			}

			Ptr<SketchTextDefinition> textDef = sketchText->definition();
			Ptr<MultiLineTextDefinition> multiLineTextDef = textDef;
			if (!multiLineTextDef) {
				LOG_ERROR("No MultiLineTextDefinition found for the sketch text.");
				return;
			}

			std::vector<Ptr<SketchLine>> lines = multiLineTextDef->rectangleLines();
			if (lines.size() != 4) {
				LOG_ERROR("RectangleLines does not have the expected number of lines.");
				return;
			}

			double minX = (std::numeric_limits<double>::max)();
			double minY = (std::numeric_limits<double>::max)();
			double maxX = std::numeric_limits<double>::lowest();
			double maxY = std::numeric_limits<double>::lowest();

			for (const auto& line : lines) {
				if (!line) continue;

				Ptr<Point3D> startPoint = line->startSketchPoint()->geometry();
				Ptr<Point3D> endPoint = line->endSketchPoint()->geometry();

				if (startPoint) {
					minX = (std::min)(minX, startPoint->x());
					minY = (std::min)(minY, startPoint->y());
					maxX = (std::max)(maxX, startPoint->x());
					maxY = (std::max)(maxY, startPoint->y());
				}

				if (endPoint) {
					minX = (std::min)(minX, endPoint->x());
					minY = (std::min)(minY, endPoint->y());
					maxX = (std::max)(maxX, endPoint->x());
					maxY = (std::max)(maxY, endPoint->y());
				}
			}
			if (minX == (std::numeric_limits<double>::max)() || minY == (std::numeric_limits<double>::max)() ||
				maxX == std::numeric_limits<double>::lowest() || maxY == std::numeric_limits<double>::lowest()) {
				LOG_ERROR("No valid points found for the bounding box.");
				return;
			}
			double centerX = (minX + maxX) / 2.0;
			double centerY = (minY + maxY) / 2.0;

			LOG_INFO("BoundingBox Min: (" + std::to_string(minX) + ", " + std::to_string(minY) + ")");
			LOG_INFO("BoundingBox Max: (" + std::to_string(maxX) + ", " + std::to_string(maxY) + ")");
			LOG_INFO("BoundingBox Center: (" + std::to_string(centerX) + ", " + std::to_string(centerY) + ")");

			Ptr<Design> design = toolsApp->activeProduct();
			Ptr<Component> root = design->rootComponent();

			if (root->customGraphicsGroups()->count() > 0) {
				for (size_t i = 0; i < root->customGraphicsGroups()->count(); ++i) {
					Ptr<CustomGraphicsGroup> group = root->customGraphicsGroups()->item(i);
					if (group) {
						group->deleteMe();
					}
				}
				LOG_INFO("Deleted existing graphics.");
				toolsApp->activeViewport()->refresh();
			}

			Ptr<CustomGraphicsGroup> highlightGraphics = root->customGraphicsGroups()->add();
			if (!highlightGraphics) {
				LOG_ERROR("Failed to create CustomGraphicsGroup");
				return;
			}

			std::vector<double> points = {
				minX, minY, 0.0,
				maxX, minY, 0.0,
				maxX, maxY, 0.0,
				minX, maxY, 0.0 
			};

			std::vector<int> indices = {
				0, 1, 1, 2, 2, 3, 3, 0
			};

			Ptr<CustomGraphicsCoordinates> coordinates = CustomGraphicsCoordinates::create(points);
			if (!coordinates) {
				LOG_ERROR("Failed to create CustomGraphicsCoordinates");
				return;
			}

			Ptr<CustomGraphicsLines> linesGraphics = highlightGraphics->addLines(coordinates, indices, false);
			if (!linesGraphics) {
				LOG_ERROR("Failed to add lines");
				return;
			}

			linesGraphics->color(Color::create(255, 0, 0, 128));
			linesGraphics->weight(4.0);
			linesGraphics->isVisible(true);
			linesGraphics->isSelectable(false);

			toolsApp->activeViewport()->refresh();
		}

		//void SketchTextPanel::duplicateAndHighlightText(const Ptr<SketchText>& text) {
		//	if (!text) {
		//		TRACE("duplicateAndHighlightText: Invalid SketchText");
		//		return;
		//	}

		//	// Erstelle eine Kopie des Textes
		//	Ptr<SketchTextInput> textInput = text->parentSketch()->sketchTexts()->createInput(
		//		text->text(), 
		//		text->height(), 
		//		text->definition()->position()
		//	);
		//	Ptr<SketchText> duplicateText = text->parentSketch()->sketchTexts()->add(textInput);

		//	// Ändere den Stil des Duplikats
		//	duplicateText->textStyle(TextStyles::BoldTextStyle | TextStyles::UnderlinedTextStyle);

		//	// Warte kurz
		//	std::this_thread::sleep_for(std::chrono::milliseconds(500));

		//	// Entferne das Duplikat
		//	duplicateText->deleteMe();
		//}

		/// <summary>
		/// <para>The getTextPosition method of the SketchTextPanel class retrieves the center position of a sketch text </para>
		/// <para>by calculating the bounding rectangle defined by its associated lines.</para>
		/// </summary>
		///
		/// <param name="sketchText">
		/// <para>is a constant reference to a smart pointer that manages an instance of the SketchText class, </para>
		/// <para>from the Autodesk Fusion 360 API, allowing for safe and efficient access to sketch text objects.</para>
		/// </param>
		///
		/// <returns>The text position.</returns>
		Ptr<Point3D> SketchTextPanel::getTextPosition(const Ptr<SketchText>& sketchText) {
			if (!sketchText) {
				LOG_ERROR("Invalid SketchText");
				return nullptr;
			}

			Ptr<SketchTextDefinition> textDef = sketchText->definition();
			Ptr<MultiLineTextDefinition> multiLineTextDef = textDef;
			if (!multiLineTextDef) {
				LOG_ERROR("No MultiLineTextDefinition found for the sketch text.");
				return nullptr;
			}

			std::vector<Ptr<SketchLine>> lines = 
				multiLineTextDef->rectangleLines();
			if (lines.size() != 4) {
				LOG_ERROR("rectangleLines does not have the expected number of lines.");
				return nullptr;
			}
			for (size_t i = 0; i < lines.size(); ++i) {
				Ptr<SketchLine> line = lines[i];

				if (line) {
					LOG_INFO("Line " 
						+ std::to_string(i) + ": " 
						+ std::to_string(line->startSketchPoint()->geometry()->x()) + ", " 
						+ std::to_string(line->startSketchPoint()->geometry()->y()));
				}
			}

			double minX = (std::numeric_limits<double>::max)();
			double minY = (std::numeric_limits<double>::max)();
			double maxX = std::numeric_limits<double>::lowest();
			double maxY = std::numeric_limits<double>::lowest();

			for (const auto& line : lines) {
				if (!line) continue;

				Ptr<Point3D> startPoint =
					line->startSketchPoint()->geometry();
				Ptr<Point3D> endPoint = 
					line->endSketchPoint()->geometry();

				if (startPoint) {
					minX = (std::min)(minX, startPoint->x());
					minY = (std::min)(minY, startPoint->y());
					maxX = (std::max)(maxX, startPoint->x());
					maxY = (std::max)(maxY, startPoint->y());
				}

				if (endPoint) {
					minX = (std::min)(minX, endPoint->x());
					minY = (std::min)(minY, endPoint->y());
					maxX = (std::max)(maxX, endPoint->x());
					maxY = (std::max)(maxY, endPoint->y());
				}
			}

			double centerX = (minX + maxX) / 2.0;
			double centerY = (minY + maxY) / 2.0;

			LOG_INFO("Center Point: (" + std::to_string(centerX) + ", " + std::to_string(centerY) + ")");

			Ptr<Point3D> centerPoint = Point3D::create(centerX, centerY, 0.0);

			return centerPoint;
		}
	}
}

//void SketchTextPanelInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
//    std::string inputId = eventArgs->input()->id();
//    TRACE("InputChanged: " + inputId);
//
//    Ptr<CommandInputs> inputs = eventArgs->inputs();
//    if (!inputs) {
//        TRACE("SketchTextPanelInputChangedEventHandler::notify: Invalid inputs");
//        return;
//    }
//
//    // Überprüfe, ob die Änderung in der Tabelle erfolgt ist
//    if (inputId.find("TextID_") != std::string::npos) {
//        size_t id = std::stoul(inputId.substr(7)); // Extrahiere die ID aus "TextID_<id>"
//        toolsApp->sketchTextPanel->blinkTextById(id);
//    }
//}


//Ptr<SketchText> SketchTextPanel::findTextById(size_t id) {
//    Ptr<Sketch> sketch = toolsApp->sketchTextPanel->sketch_;
//    if (!sketch) {
//        TRACE("findTextById: No active sketch");
//        return nullptr;
//    }
//
//    Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
//    if (!sketchTexts) {
//        TRACE("findTextById: No SketchTexts found in the sketch");
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