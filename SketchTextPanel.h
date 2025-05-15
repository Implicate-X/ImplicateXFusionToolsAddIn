#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region Constants
		constexpr auto IDS_CMD_SKETCH_TEXT_DEFINITIONS = "commandSketchTextDefinitions"; // commandSketchTextDefinitions
		constexpr auto IDS_ITEM_DROPDOWN_SELECT_SKETCH = "dropdownSelectSketch"; // dropdownSelectSketch
		constexpr auto IDS_ITEM_TAB_TEXTSIZE = "textSizeTab"; // textSizeTab
		constexpr auto IDS_ITEM_TEXTSIZE_MIN = "textSizeMin"; // textSizeMin
		constexpr auto IDS_ITEM_TEXTSIZE_MAX = "textSizeMax"; // textSizeMax
		constexpr auto IDS_ITEM_TEXTSIZE_NEW = "textSizeNew"; // textSizeNew
		constexpr auto IDS_ITEM_TEXTSIZE_FILTER = "textSizeFilter"; // textSizeFilter
		constexpr auto IDS_ITEM_TEXTSIZE_MATCH = "textSizeMatch"; // textSizeMatch
		constexpr auto IDS_ITEM_TEXTSIZE_TABLE = "textSizeTable"; // textSizeTable
		constexpr auto IDS_ITEM_TEXTSIZE_REPLACE = "textSizeReplace"; // textSizeReplace
		constexpr auto IDS_ITEM_TEXTSIZE_SEPARATOR = "textSizeSeparator"; // textSizeSeparator
		constexpr auto IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR = "textSizeMatchSeparator"; // textSizeMatchSeparator
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT = "Resources/Sketch/Text"; // Resources/Sketch/Text
		constexpr auto IDS_PATH_ICON_SKETCH_TEXTSIZE = "Resources/Sketch/Text/TextSize"; // Resources/Sketch/Text/TextSize
		constexpr auto IDS_ACTION_TEXT_ID = "textId"; // textId
		constexpr auto IDS_ACTION_TEXT_VALUE = "textValue"; // textValue
		constexpr auto IDS_ACTION_TEXT_HEIGHT = "textHeight"; // textHeight
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

		/// <summary>
		/// <para>The SketchTextPanel class is responsible for managing command definitions</para>
		/// <para>related to sketch text within a user interface,including initializing the palette,</para>
		/// <para>creating new commands, and removing existing ones.</para>
		/// </summary>
		class SketchTextPanel
		{
		public:
			bool initialize();
			bool terminate();
			bool createCommand();
			bool removeCommand();

			#pragma region Design
			bool addTextSizeTab(const Ptr<CommandInputs>& inputs, Ptr<CommandInputs>& tabInputs);
			bool addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown);
			bool addTextSizeFilter(const Ptr<CommandInputs>& inputs);
			bool addTextSizeMatch(const Ptr<CommandInputs>& inputs);
			#pragma endregion

			#pragma region Action
			static void dropDownSelectAction(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textSizeReplaceAction(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textSizeChangeAction(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textIdSelectAction(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textValueChangeAction(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightChangeAction(const Ptr<InputChangedEventArgs>& eventArgs);
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
