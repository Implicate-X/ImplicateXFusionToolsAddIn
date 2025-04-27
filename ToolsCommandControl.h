#pragma once

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

namespace implicatex {  
namespace fusion {
    /// <summary>
    /// <para>ToolsCommandCreatedEventHandler is a event handler that inherits from CommandCreatedEventHandler</para>
	/// <para> and overrides the notify method to display a message box when a tools command is created.</para>
    /// </summary>
	class ToolsCommandControl :  
		public CommandControl
	{  
	public:
        /// <summary>
        /// <para>The initialize function of the ToolsCommandControl class sets up a command </para>
        /// <para>by adding an event handler for the command creation event, returning true if successful.</para>
        /// </summary>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
		bool initialize();
	};  
}  
}
