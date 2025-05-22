#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region ActionEvent
		/// <summary>
		/// <para>SketchTextHeightTabInputChangedEventHandler implements </para>
		/// <para>the InputChangedEventHandler interface.</para>
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

			#pragma region Getters
			Ptr<SketchText> getTextById(const std::string& id) const;
			Ptr<SketchText> getSelectedText() const { return selectedText_; }
			#pragma endregion

			#pragma region Setters
			void setSelectedText(const Ptr<SketchText>& text) { selectedText_ = text; }
			#pragma endregion

			std::unordered_map<std::string, Ptr<SketchText>> idTextMap_;
			std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)> actions_;

		private:
			Ptr<SketchText> selectedText_;
		};
	}
}
