#pragma once

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {  
	namespace fusion {
        /// <summary>
        /// <para>SketchTextCommandCreatedEventHandler is a event handler that inherits from CommandCreatedEventHandler</para>
		/// <para> and overrides the notify method to display a message box when a tools command is created.</para>
        /// </summary>
        class SketchTextCommandCreatedEventHandler : public CommandCreatedEventHandler {
        public:
            explicit SketchTextCommandCreatedEventHandler() {}

            /// <summary>The notify method is an overridden virtual function that handles the event when a command is created.</summary>
            ///
            /// <param name="eventArgs">
			/// <para>- The event arguments that contain information about the command creation event.</para>
            /// </param>
            void notify(const Ptr<adsk::core::CommandCreatedEventArgs>& eventArgs) override;
        };

        /// <summary>
        /// <para>SketchTextCommandControl is a derived class of CommandControl that provides an initialize function</para>
        /// <para> to set up command event handling, returning a boolean indicating success or failure.</para>
        /// </summary>
		class SketchTextCommandControl :  
			public CommandControl  
		{  
		public:
            /// <summary>
            /// <para>The initialize function of the SketchTextCommandControl class sets up a command </para>
            /// <para>by adding an event handler for the command creation event, returning true if successful.</para>
            /// </summary>
            ///
            /// <returns>True if it succeeds, false if it fails.</returns>
			bool initialize();
		};  
	}  
}
