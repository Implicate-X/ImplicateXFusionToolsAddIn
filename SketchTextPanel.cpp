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
		/// <summary>The sketch text panel sketch.</summary>
		Ptr<Sketch> SketchTextPanel::sketch_ = nullptr;

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

		/// <summary>
		/// <para>The initialize function in the SketchTextPanel class is responsible</para>
		/// <para>for setting up the palette by invoking the createCommand method.</para>
		/// </summary>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
        bool SketchTextPanel::initialize() {  
			idHandlers_.insert({ std::string(IDS_ITEM_DROPDOWN_SELECT_SKETCH), &SketchTextPanel::handleDropDownSelect});
			idHandlers_.insert({ std::string(IDS_ITEM_TEXTSIZE_REPLACE), &SketchTextPanel::handleTextSizeReplace});

			
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
			Ptr<ListItem> selectedItem = dropdown->selectedItem();
			std::string selectedSketchName = selectedItem->name();
			Application::log("Selected Sketch: " + selectedSketchName);
			if (selectedItem) {
				Ptr<Design> design = toolsApp->activeProduct();
				SketchTextPanel::sketch_ = design->rootComponent()->sketches()->itemByName(selectedSketchName);
				if (!SketchTextPanel::sketch_) {
					Application::log("handleDropDownSelect: Sketch not found");
					return;
				}
				toolsApp->sketchTextPanel->alignModelToSketchXYPlane(SketchTextPanel::sketch_);
			}
			else {
				Application::get()->log("handleDropDownSelect: No sketch selected");
			}
		}

		/// <summary>Handles the text size replace described by eventArgs.</summary>
		///
		/// <param name="eventArgs">The event arguments.</param>
		void SketchTextPanel::handleTextSizeReplace(const Ptr<InputChangedEventArgs>& eventArgs) {
			Application::log("handleTextSizeReplace");
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
		}

		/// <summary>
		/// <para>The notify method is an overridden virtual function that handles input change events.</para>
		/// </summary>
		/// <param name="eventArgs">
		/// <para>is a constant reference to a pointer of type InputChangedEventArgs, </para>
		/// <para>which is typically used to handle input change events.</para>
		/// </param>
		void SketchTextPanelInputChangedEventHandler::notify(const Ptr<InputChangedEventArgs>& eventArgs) {
			Ptr<CommandInputs> inputs = eventArgs->inputs();
			Ptr<DropDownCommandInput> dropdown = inputs->itemById(IDS_ITEM_DROPDOWN_SELECT_SKETCH);
			Ptr<ListItem> selectedItem = dropdown->selectedItem();
			Ptr<ValueCommandInput> minTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MIN);
			Ptr<ValueCommandInput> maxTextHeight = inputs->itemById(IDS_ITEM_TEXTSIZE_MAX);
			Ptr<TextBoxCommandInput> matchesTextHeightInput = inputs->itemById(IDS_ITEM_TEXTSIZE_MATCH);

			std::string inputId = eventArgs->input()->id();
			Application::log("InputChanged: " + inputId);

			if (toolsApp->sketchTextPanel->idHandlers_.find(inputId) != toolsApp->sketchTextPanel->idHandlers_.end()) {
				toolsApp->sketchTextPanel->idHandlers_[inputId](eventArgs);
			} else {
				Application::log("Unknown ID: " + inputId);
			}
			return;
		}

//				double minHeightValue = minTextHeight->value();
//				double maxHeightValue = maxTextHeight->value();
				//auto filteredTexts = 
				//	toolsApp->sketchTextPanel->filterTextDefinitionsByHeight(sketch, minHeightValue, maxHeightValue);

				//size_t textHeightMatchCount = filteredTexts.size();

				//matchesTextHeightInput->text(std::to_string(textHeightMatchCount));

				//for (const auto& text : filteredTexts) {
				//	Application::log("Filtered Text: " + text->text());
				//}

		/// <summary>
		/// <para>The filterTextDefinitionsByHeight function filters and returns a vector of SketchText pointers </para> 
		/// <para>from a given Sketch, based on specified minimum and maximum height values.</para>
		/// </summary>
		///
		/// <param name="sketch">		 The sketch.</param>
		/// <param name="minHeightValue">The minimum height value.</param>
		/// <param name="maxHeightValue">The maximum height value.</param>
		///
		/// <returns>A std::vector&lt;Ptr&lt;SketchText&gt;&gt;</returns>
		std::vector<Ptr<SketchText>> SketchTextPanel::filterTextDefinitionsByHeight(
			const Ptr<Sketch>& sketch,
			double minHeightValue,
			double maxHeightValue)
		{
			std::vector<Ptr<SketchText>> filteredTexts;

			if (!sketch) {
				Application::log("Invalid sketch provided.");
				return filteredTexts;
			}

			Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
			if (!sketchTexts) {
				Application::log("No SketchTexts found in the sketch.");
				return filteredTexts;
			}

			//std::for_each(sketchTexts->begin(), sketchTexts->end(), [&](const Ptr<SketchText>& text) {
			//	if (text) {
			//		double textHeight = text->height();
			//		if (textHeight >= minHeightValue && textHeight <= maxHeightValue) {
			//			filteredTexts.push_back(text);
			//		}
			//	}
			//});
			for (size_t i = 0; i < sketchTexts->count(); ++i) {
				Ptr<SketchText> text = sketchTexts->item(i);
				if (text) {
					double textHeight = text->height();
					if (textHeight >= minHeightValue && textHeight <= maxHeightValue) {
						filteredTexts.push_back(text);
					}
				}
			}

			return filteredTexts;
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
	}
}
