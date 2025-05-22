#pragma once
using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
	namespace fusion {
		#pragma region ActionEvent
		/// <summary>
		/// <para>SketchTextSettingsTabInputChangedEventHandler implements </para>
		/// <para>the InputChangedEventHandler interface.</para>
		/// </summary>
		class SketchTextSettingsTabInputChangedEventHandler : public InputChangedEventHandler {
		public:
			void notify(const Ptr<InputChangedEventArgs>& eventArgs) override;
		};
		#pragma endregion

		class SketchTextSettingsTab
		{
		public:
			bool initialize(Ptr<Command> command, const Ptr<TabCommandInput>& tabInput);

			void save();
			double load();

			double getZoomFactor() const { return zoomFactor_; }
			void setZoomFactor(double zoomFactor) { zoomFactor_ = zoomFactor; }

		private:
			double zoomFactor_;
		};
	}
}
