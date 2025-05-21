#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region Constants
		constexpr auto IDS_CMD_SKETCH_TEXT_DEFINITIONS = "commandSketchTextDefinitions"; // commandSketchTextDefinitions
		constexpr auto IDS_ITEM_DROPDOWN_SELECT_SKETCH = "dropdownSelectSketch"; // dropdownSelectSketch
		constexpr auto IDS_ITEM_TAB_SETTINGS = "settingsTab"; // settingsTab
		constexpr auto IDS_ITEM_TAB_TEXT_HEIGHT = "textHeightTab"; // textHeightTab
		constexpr auto IDS_ITEM_TEXT_HEIGHT_MIN = "textHeightMin"; // textHeightMin
		constexpr auto IDS_ITEM_TEXT_HEIGHT_MAX = "textHeightMax"; // textHeightMax
		constexpr auto IDS_ITEM_TEXT_HEIGHT_NEW = "textHeightNew"; // textHeightNew
		constexpr auto IDS_ITEM_TEXT_HEIGHT_FILTER = "textHeightFilter"; // textHeightFilter
		constexpr auto IDS_ITEM_TEXT_HEIGHT_MATCH = "textHeightMatch"; // textHeightMatch
		constexpr auto IDS_ITEM_TEXT_HEIGHT_TABLE = "textHeightTable"; // textHeightTable
		constexpr auto IDS_ITEM_TEXT_HEIGHT_REPLACE = "textHeightReplace"; // textHeightReplace
		constexpr auto IDS_ITEM_TEXT_HEIGHT_SEPARATOR = "textHeightSeparator"; // textHeightSeparator
		constexpr auto IDS_ITEM_TEXT_HEIGHT_MATCH_SEPARATOR = "textHeightMatchSeparator"; // textHeightMatchSeparator
		constexpr auto IDS_ITEM_TEXT_ZOOM_FACTOR = "textZoomFactor"; // textZoomFactor
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT = "Resources/Sketch/Text"; // Resources/Sketch/Text
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT_SETTINGS = "Resources/Sketch/Text/Settings"; // Resources/Sketch/Text/Settings
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT_HEIGHT = "Resources/Sketch/Text/Height"; // Resources/Sketch/Text/Height
		constexpr auto IDS_CELL_TEXT_ID = "textIdCell"; // textIdCell
		constexpr auto IDS_CELL_TEXT_VALUE = "textValueCell"; // textValueCell
		constexpr auto IDS_CELL_TEXT_HEIGHT = "textHeightCell"; // textHeightCell
		constexpr auto IDS_UNIT_MM = "mm"; // Millimeters
		#pragma endregion

		#pragma region DesignEvent
		/// <summary>
		/// <para>SketchTextPanelCommandCreatedEventHandler implements the CommandCreatedEventHandler interface,</para>
		/// <para>providing functionality to populate a dropdown menu with available sketches</para>
		/// </summary>
		class SketchTextPanelCommandCreatedEventHandler : public CommandCreatedEventHandler {
		public:
			void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override;
		};
		#pragma endregion

		class SketchTextPanel
		{
		public:
			#pragma region Common
			bool initialize();
			bool terminate();
			bool createCommand();
			bool removeCommand();
			#pragma endregion

			#pragma region Design
			bool addTextHeightTab(const Ptr<Command>& command);
			bool addSettingsTab(const Ptr<Command>& command);
			#pragma endregion

			#pragma region Operation
			Ptr<Point3D> getTextPosition(const Ptr<SketchText>& sketchText);
			bool getTextPoints(const Ptr<SketchText>& sketchText, Ptr<Point3D>& centerPoint, Ptr<Point3D>& minPoint, Ptr<Point3D>& maxPoint);
			bool getSelectedSketch(const Ptr<DropDownCommandInput>& dropdown, Ptr<Sketch>& sketch);
			bool alignModelToSketchXYPlane(const Ptr<Sketch>& sketch);
			void addHighlightGraphics(const Ptr<SketchText>& text);
			void focusCameraOnText(const Ptr<SketchText>& sketchText);
			#pragma endregion

			std::shared_ptr<SketchTextSettingsTab> settingsTab_;
			std::shared_ptr<SketchTextHeightTab> textHeightTab_;
		};
	}
}
