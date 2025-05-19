#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region ActionEvent
		/// <summary>
		/// <para>SketchTextPanelInputChangedEventHandler implements the InputChangedEventHandler interface,</para>
		/// <para>providing to handle input change events by the selected sketch from a dropdown menu.</para>
		/// </summary>
		class SketchTextHeightTabInputChangedEventHandler : public InputChangedEventHandler {
		public:
			void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
		};
		#pragma endregion

		class SketchTextHeightTab
		{
		public:
			bool initialize(Ptr<Command> command, const Ptr<TabCommandInput>& tabInput);
			bool addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown);
			bool addTextHeightFilter(const Ptr<CommandInputs>& inputs);
			bool addTextHeightMatch(const Ptr<CommandInputs>& inputs);
			bool getTextHeightMatch(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts);

			#pragma region Action
			static void dropDownSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightReplaced(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightChanged(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			#pragma endregion

			std::unordered_map<std::string, Ptr<SketchText>> idTextMap_;
			std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)> actions_;
		};
	}
}
