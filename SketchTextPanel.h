#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		const std::string IDS_CMD_SKETCH_TEXT_DEFINITIONS = "commandSketchTextDefinitions"; // commandSketchTextDefinitions
		const std::string IDS_ITEM_DROPDOWN_SELECT_SKETCH = "dropdownSelectSketch"; // dropdownSelectSketch
		const std::string IDS_ITEM_TAB_TEXTSIZE = "textSizeTab"; // textSizeTab
		const std::string IDS_ITEM_TEXTSIZE_MIN = "textSizeMin"; // textSizeMin
		const std::string IDS_ITEM_TEXTSIZE_MAX = "textSizeMax"; // textSizeMax
		const std::string IDS_ITEM_TEXTSIZE_NEW = "textSizeNew"; // textSizeNew
		const std::string IDS_ITEM_TEXTSIZE_FILTER = "textSizeFilter"; // textSizeFilter
		const std::string IDS_ITEM_TEXTSIZE_MATCH = "textSizeMatch"; // textSizeMatch
		const std::string IDS_ITEM_TEXTSIZE_SEPARATOR = "textSizeSeparator"; // textSizeSeparator
		const std::string IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR = "textSizeMatchSeparator"; // textSizeMatchSeparator
		const std::string IDS_PATH_ICON_SKETCH_TEXT = "Resources/Sketch/Text"; // Resources/Sketch/Text
		const std::string IDS_PATH_ICON_SKETCH_TEXTSIZE = "Resources/Sketch/Text/TextSize"; // Resources/Sketch/Text/TextSize
		const std::string IDS_UNIT_MM = "mm"; // Millimeters

		/// <summary>
		/// <para>SketchTextPanelCommandCreatedEventHandler implements the CommandCreatedEventHandler interface,</para>
		/// <para>providing functionality to populate a dropdown menu with available sketches</para>
		/// </summary>
		class SketchTextPanelCommandCreatedEventHandler : public CommandCreatedEventHandler {
		public:
			void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override;
		};

		/// <summary>
		/// <para>SketchTextPanelInputChangedEventHandler implements the InputChangedEventHandler interface,</para>
		/// <para>providing to handle input change events by the selected sketch from a dropdown menu.</para>
		/// </summary>
		class SketchTextPanelInputChangedEventHandler : public InputChangedEventHandler {
		public:
			void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
		};

		/// <summary>
		/// <para>The SketchTextPanel class is responsible for managing command definitions</para>
		/// <para>related to sketch text within a user interface,including initializing the palette,</para>
		/// <para>creating new commands, and removing existing ones.</para>
		/// </summary>
		class SketchTextPanel
		{
		public:
			/// <summary>
			/// <para>The initialize function in the SketchTextPanel class is responsible</para>
			/// <para>for setting up the palette by invoking the createCommand method.</para>
			/// </summary>
			bool initialize();

			/// <summary>
			/// <para></para>
			/// </summary>
			bool terminate();

			/// <summary>
			/// <para>The createCommand function in the SketchTextDefinitionsPanel class is responsible</para>
			/// <para>for creating and executing a new command definition for sketch text definitions</para>
			/// <para>within the user interface of an application.</para>
			/// </summary>
			bool createCommand();

			/// <summary>
			/// <para>The removeCommand function in the SketchTextDefinitionsPanel class is responsible for removing</para>
			/// <para>a command definition from the user interface if it exists, effectively cleaning up resources associated with that command.</para></summary>
			bool removeCommand();
		};
	}
}
