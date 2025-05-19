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
		/// <para>getSelectedSketch retrieves a sketch from a dropdown command input in the SketchTextPanel, </para>
		/// <para>returning a boolean indicating success or failure while logging errors for various failure conditions.</para>
		/// </summary>
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
					LOG_INFO("Line World = "
						+ std::to_string(i) + ": "
						+ std::to_string(line->startSketchPoint()->worldGeometry()->x()) + ", "
						+ std::to_string(line->startSketchPoint()->worldGeometry()->y()) + ", "
						+ std::to_string(line->startSketchPoint()->worldGeometry()->z()));
					LOG_INFO("Line Sketch = "
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
					line->startSketchPoint()->worldGeometry();
				Ptr<Point3D> endPoint =
					line->endSketchPoint()->worldGeometry();

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

			Ptr<Point3D> startPoint = nullptr;
			Ptr<Point3D> endPoint = nullptr;

			for (const auto& line : lines) {
				if (!line) continue;

				Ptr<Point3D> startPoint = line->startSketchPoint()->worldGeometry();
				Ptr<Point3D> endPoint = line->endSketchPoint()->worldGeometry();

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
			double centerZ = 0.0;
			
			if (startPoint) {
				centerZ = startPoint->z();
			}

			minPoint = Point3D::create(minX, minY, centerZ);
			maxPoint = Point3D::create(maxX, maxY, centerZ);
			centerPoint = Point3D::create(centerX, centerY, centerZ);

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
			}
			else {
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
		/// <para>The focusCameraOnText function adjusts the camera's position and target to focus on a SketchText, </para>
		/// <para>ensuring that the camera is properly set up to view the text with a calculated zoom level.</para>
		/// </summary>
		///
		/// <param name="sketchText">
		/// <para>is a constant reference to a smart pointer that manages an instance of the SketchText class, </para>
		/// <para>allowing for safe and efficient access to sketch text objects.</para>
		/// </param>
		void SketchTextPanel::focusCameraOnText(const Ptr<SketchText>& sketchText) {
			if (!sketchText) {
				LOG_ERROR("Invalid SketchText");
				return;
			}

			Ptr<Point3D> position = nullptr;
			Ptr<Point3D> minPoint = nullptr;
			Ptr<Point3D> maxPoint = nullptr;

			if (!toolsApp->sketchTextPanel->getTextPoints(sketchText, position, minPoint, maxPoint)) {
				LOG_ERROR("Failed to get text points");
				return;
			}
			if (!position || !minPoint || !maxPoint) {
				LOG_ERROR("Invalid points");
				return;
			}

			// Calculate the diagonal of the bounding box
			double diagonal = sqrt(
				pow(maxPoint->x() - minPoint->x(), 2) +
				pow(maxPoint->y() - minPoint->y(), 2) +
				pow(maxPoint->z() - minPoint->z(), 2)
			);

			double zoomFactor = 1.0; // Zoom closer to the text

			Ptr<Viewport> viewport = toolsApp->activeViewport();
			if (!viewport) {
				LOG_ERROR("Failed to get active viewport");
				return;
			}

			Ptr<Camera> viewportCamera = viewport->camera();
			CameraTypes viewportCameraType = viewportCamera->cameraType();

			Ptr<Camera> sketchTextCamera = Camera::create();
			if (!sketchTextCamera) {
				LOG_ERROR("Failed to create camera");
				return;
			}

			switch (viewportCameraType)
			{
			case CameraTypes::OrthographicCameraType:
				sketchTextCamera->cameraType(CameraTypes::OrthographicCameraType);
				sketchTextCamera->eye(Point3D::create(position->x(), position->y(), position->z() + 1.0));
				sketchTextCamera->setExtents(diagonal * zoomFactor, diagonal * zoomFactor);
				break;

			case CameraTypes::PerspectiveCameraType:
				sketchTextCamera->cameraType(CameraTypes::PerspectiveCameraType);

			case CameraTypes::PerspectiveWithOrthoFacesCameraType:
				sketchTextCamera->cameraType(CameraTypes::PerspectiveWithOrthoFacesCameraType);
				sketchTextCamera->eye(Point3D::create(position->x(), position->y(), position->z() + diagonal * 1.0));
				sketchTextCamera->perspectiveAngle(zoomFactor);
				break;
			}
			
			// Set the camera position (eye) and the target (centerPoint)
			sketchTextCamera->target(position);
			sketchTextCamera->upVector(Vector3D::create(0.0, 1.0, 0.0));
			sketchTextCamera->isSmoothTransition(true);

			viewport->camera(sketchTextCamera);
		}

		/// <summary>
		/// <para>addHighlightGraphics adds graphical highlights around a specified SketchText object by calculating </para>
		/// <para>its bounding box and creating custom graphics to visually represent it in the application.</para>
		/// </summary>
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

			Ptr<Point3D> startPoint;
			Ptr<Point3D> endPoint;

			for (const auto& line : lines) {
				if (!line) continue;

				startPoint = line->startSketchPoint()->worldGeometry();
				endPoint = line->endSketchPoint()->worldGeometry();

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
			double centerZ = 0.0;
			
			if (startPoint) {
				centerZ = startPoint->z();
			}

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
				minX, minY, centerZ,
				maxX, minY, centerZ,
				maxX, maxY, centerZ,
				minX, maxY, centerZ
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

			auto colorEffect = CustomGraphicsSolidColorEffect::create(Color::create(0x00, 0xFF, 0xFF, 0xFF));
			linesGraphics->color(colorEffect);
			linesGraphics->weight(1.5);
			linesGraphics->isVisible(true);
			linesGraphics->isSelectable(true);
			linesGraphics->setOpacity(1.0, true);

			toolsApp->activeViewport()->refresh();
		}
	}
}
