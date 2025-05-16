#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region Constants
		constexpr auto IDS_CMD_SKETCH_TEXT_DEFINITIONS = "commandSketchTextDefinitions"; // commandSketchTextDefinitions
		constexpr auto IDS_ITEM_DROPDOWN_SELECT_SKETCH = "dropdownSelectSketch"; // dropdownSelectSketch
		constexpr auto IDS_ITEM_TAB_TEXTHEIGHT = "textHeightTab"; // textHeightTab
		constexpr auto IDS_ITEM_TEXTHEIGHT_MIN = "textHeightMin"; // textHeightMin
		constexpr auto IDS_ITEM_TEXTHEIGHT_MAX = "textHeightMax"; // textHeightMax
		constexpr auto IDS_ITEM_TEXTHEIGHT_NEW = "textHeightNew"; // textHeightNew
		constexpr auto IDS_ITEM_TEXTHEIGHT_FILTER = "textHeightFilter"; // textHeightFilter
		constexpr auto IDS_ITEM_TEXTHEIGHT_MATCH = "textHeightMatch"; // textHeightMatch
		constexpr auto IDS_ITEM_TEXTHEIGHT_TABLE = "textHeightTable"; // textHeightTable
		constexpr auto IDS_ITEM_TEXTHEIGHT_REPLACE = "textHeightReplace"; // textHeightReplace
		constexpr auto IDS_ITEM_TEXTHEIGHT_SEPARATOR = "textHeightSeparator"; // textHeightSeparator
		constexpr auto IDS_ITEM_TEXTHEIGHT_MATCH_SEPARATOR = "textHeightMatchSeparator"; // textHeightMatchSeparator
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT = "Resources/Sketch/Text"; // Resources/Sketch/Text
		constexpr auto IDS_PATH_ICON_SKETCH_TEXTHEIGHT = "Resources/Sketch/Text/TextSize"; // Resources/Sketch/Text/TextSize
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

		#pragma region ActionEvent
		/// <summary>
		/// <para>SketchTextPanelInputChangedEventHandler implements the InputChangedEventHandler interface,</para>
		/// <para>providing to handle input change events by the selected sketch from a dropdown menu.</para>
		/// </summary>
		class SketchTextPanelInputChangedEventHandler : public InputChangedEventHandler {
		public:
			void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
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
			bool addTextSizeTab(const Ptr<CommandInputs>& inputs, Ptr<CommandInputs>& tabInputs);
			bool addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown);
			bool addTextSizeFilter(const Ptr<CommandInputs>& inputs);
			bool addTextSizeMatch(const Ptr<CommandInputs>& inputs);
			#pragma endregion

			#pragma region Action
			static void dropDownSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightReplaced(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightChanged(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			#pragma endregion

			#pragma region Operation
			Ptr<Point3D> getTextPosition(const Ptr<SketchText>& sketchText);
			bool getTextPoints(const Ptr<SketchText>& sketchText, Ptr<Point3D>& centerPoint, Ptr<Point3D>& minPoint, Ptr<Point3D>& maxPoint);
			bool getSelectedSketch(const Ptr<DropDownCommandInput>& dropdown, Ptr<Sketch>& sketch);
			bool getTextSizeMatch(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts);
			bool alignModelToSketchXYPlane(const Ptr<Sketch>& sketch);
			void addHighlightGraphics(const Ptr<SketchText>& text);
			void focusCameraOnText(const Ptr<SketchText>& sketchText);
			#pragma endregion

			std::unordered_map<std::string, Ptr<SketchText>> sketchTextMap_;
			std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)> actionHandlers_;
		};
	}
}
