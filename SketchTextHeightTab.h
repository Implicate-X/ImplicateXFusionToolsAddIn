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

			#pragma region Design
			bool addSketchDropDown(const Ptr<CommandInputs>& inputs, Ptr<DropDownCommandInput>& dropdown);
			bool addTextHeightFilter(const Ptr<CommandInputs>& inputs);
			bool addTextHeightMatchTable(const Ptr<CommandInputs>& inputs);
			bool getTextHeightMatchItems(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts);
			#pragma endregion

			#pragma region Action
			static void dropDownSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightReplaced(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightChanged(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textIdCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textValueCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textHeightCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			static void textToggleCellSelected(const Ptr<InputChangedEventArgs>& eventArgs);
			void localizeText(const Ptr<InputChangedEventArgs>& eventArgs);
			#pragma endregion

			#pragma region Getters
			static SketchTextHeightTab* get();
			Ptr<SketchText> getTextById(const std::string& id) const;
			Ptr<SketchText> getTextById(const int id) const;
			Ptr<SketchText> getSelectedText() const { return selectedText_; }
			const std::string& getPendingTextValue() const { return pendingTextValue_; }
			Ptr<StringValueCommandInput> getTextValueCellInput() const { return textValueCellInput_; }
			std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)>& getActions() { return actions_; }
			#pragma endregion

			#pragma region Setters
			void setSelectedText(const Ptr<SketchText>& text) { selectedText_ = text; }
		    void setPendingTextValue(const std::string& value) { pendingTextValue_ = value; }
			void setTextValueCellInput(const Ptr<StringValueCommandInput>& input) { textValueCellInput_ = input; }
			void setActions(const std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)>& actions) { actions_ = actions; }
			#pragma endregion

			std::unordered_map<std::string, Ptr<SketchText>> idTextMap_;

		private:
			std::string pendingTextValue_;
			Ptr<SketchText> selectedText_;
			Ptr<StringValueCommandInput> textValueCellInput_;
			std::unordered_map<std::string, void(*)(const Ptr<InputChangedEventArgs>& eventArgs)> actions_;
		};
	}
}
