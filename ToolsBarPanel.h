#pragma once

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {
    namespace fusion {
		/// <summary>
		/// <para>The LanguageCommandCreatedEventHandler class is a specialized event handler to the creation </para>
		/// <para>of language commands, allowing for custom actions to be performed when such events occur, </para>
		/// <para>including logging the selected language.</para>
		/// </summary>
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

        /// <summary>
        /// <para>The ToolsBarPanel class is a derived class of ToolbarPanel that manages the initialization, </para>
        /// <para>termination, and addition of command controls and language selectors for the user interface.</para>
        /// </summary>
        class ToolsBarPanel :
            public ToolbarPanel
        {
        public:
			/// <summary>
			/// <para>The initialize function sets up the toolbar panel by terminating any existing processes</para>
			/// <para>and adding necessary controls, returning true if successful or false if any additions fail.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
            bool initialize();

			/// <summary>
			/// <para>The terminate function is responsible for cleaning up and deleting specific command definitions</para>
			/// <para>related to region selections and the ImplicateX command from the user interface.</para>
			/// </summary>
            void terminate();

			/// <summary>
			/// <para>The addToolsCommandControl function is responsible for adding a command control </para>
			/// <para>for the Implicate-X tool to the toolbar, ensuring that the necessary command definitions </para>
			/// <para>and controls are successfully created and initialized.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool addToolsCommandControl();

			/// <summary>
			/// <para>addSketchTextCommandControl adds a command control for sketch text functionality to the toolbar, </para>
			/// <para>ensuring proper initialization and logging any failures encountered during the process.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool addSketchTextCommandControl();

			/// <summary>
			/// <para>removeSketchTextCommandControl is for deleting the sketch text command from the user interface </para>
			/// <para>if it exists, returning true upon successful execution.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool removeSketchTextCommandControl();

			/// <summary>
			/// <para>updateSketchTextCommandControlLabel updates the control label of the sketch text command, </para>
			/// <para>using a string loaded from resources, and returns a boolean indicating success.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool updateSketchTextCommandControlLabel();

			/// <summary>
			/// <para>The addLanguageSelector function is responsible for creating a dropdown menu </para>
			/// <para>for selecting countries, populating it with command buttons for each region, </para>
			/// <para>and logging any failures encountered during the process.</para></summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
            bool addLanguageSelector();

			/// <summary>
			/// <para>The removeLanguageSelector function is for safely removing </para>
			/// <para>the language selector command and its associated dropdown control, </para>
			/// <para>logging any failures encountered during the process.</para>
			/// </summary>
			///
			/// <returns>True if it succeeds, false if it fails.</returns>
			bool removeLanguageSelector();
        };
    }
}
