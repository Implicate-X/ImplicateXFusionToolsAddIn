#pragma once

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
    namespace fusion {
		class LanguageCommandCreatedEventHandler : public adsk::core::CommandCreatedEventHandler {
		public:
			explicit LanguageCommandCreatedEventHandler(const std::string& localeId) : localeId_(localeId) {}

			/// <summary>
			/// <para>The notify method in the LanguageCommandCreatedEventHandler class is an overridden virtual function</para>
			/// <para>that handles the event when a language command is created, logging a message with the selected language.</para>
			/// </summary>
			///
			/// <param name="eventArgs">The event arguments.</param>
			void notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) override;

		private:
			std::string localeId_;
		};

        class ToolsBarPanel :
            public ToolbarPanel
        {
        public:
            bool initialize();
            void terminate();
			bool terminateLanguageSelector();
			bool AddImplicateXCommandControl();
            bool AddLanguageSelector();
        };
    }
}
