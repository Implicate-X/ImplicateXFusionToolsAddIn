#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		constexpr auto IDS_CMD_SKETCH_TEXT_DEFINITIONS = "commandSketchTextDefinitions"; // commandSketchTextDefinitions
		constexpr auto IDS_ITEM_DROPDOWN_SELECT_SKETCH = "dropdownSelectSketch"; // dropdownSelectSketch
		constexpr auto IDS_ITEM_TAB_TEXTSIZE = "textSizeTab"; // textSizeTab
		constexpr auto IDS_ITEM_TEXTSIZE_MIN = "textSizeMin"; // textSizeMin
		constexpr auto IDS_ITEM_TEXTSIZE_MAX = "textSizeMax"; // textSizeMax
		constexpr auto IDS_ITEM_TEXTSIZE_NEW = "textSizeNew"; // textSizeNew
		constexpr auto IDS_ITEM_TEXTSIZE_FILTER = "textSizeFilter"; // textSizeFilter
		constexpr auto IDS_ITEM_TEXTSIZE_MATCH = "textSizeMatch"; // textSizeMatch
		constexpr auto IDS_ITEM_TEXTSIZE_SEPARATOR = "textSizeSeparator"; // textSizeSeparator
		constexpr auto IDS_ITEM_TEXTSIZE_MATCH_SEPARATOR = "textSizeMatchSeparator"; // textSizeMatchSeparator
		constexpr auto IDS_PATH_ICON_SKETCH_TEXT = "Resources/Sketch/Text"; // Resources/Sketch/Text
		constexpr auto IDS_PATH_ICON_SKETCH_TEXTSIZE = "Resources/Sketch/Text/TextSize"; // Resources/Sketch/Text/TextSize
		constexpr auto IDS_UNIT_MM = "mm"; // Millimeters

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
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool initialize();

			/// <summary>
			/// <para>The terminate function in the SketchTextPanel class is a boolean method </para>
			/// <para>that calls and returns the result of the removeCommand function.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool terminate();

			/// <summary>
			/// <para>The createCommand function in the SketchTextPanel class is responsible</para>
			/// <para>for creating and executing a new command definition for sketch text definitions.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool createCommand();

			/// <summary>
			/// <para>The removeCommand function in the SketchTextPanel class is responsible for removing</para>
			/// <para>a command definition from the user interface if it exists, </para>
			/// <para>effectively cleaning up resources associated with that command.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool removeCommand();

			/// <summary>Filter text definitions by height.</summary>
			///
			/// <param name="sketch">		 The sketch.</param>
			/// <param name="minHeightValue">The minimum height value.</param>
			/// <param name="maxHeightValue">The maximum height value.</param>
			///
			/// <returns>A std::vector&lt;Ptr&lt;SketchText&gt;&gt;</returns>
			std::vector<Ptr<SketchText>> filterTextDefinitionsByHeight(
				const Ptr<Sketch>& sketch,
				double minHeightValue,
				double maxHeightValue);

			/// <summary>Align model to sketch xy plane.</summary>
			///
			/// <param name="sketch">The sketch.</param>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool alignModelToSketchXYPlane(const Ptr<Sketch>& sketch);
		};
	}
}
