#pragma once
#include "ToolsApp.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace icu;

namespace implicatex {
    namespace fusion {
        /// <summary>The gApp symbol is a pointer to the ToolsApp class, which serves as the main application object.</summary>
        extern Ptr<ToolsApp> toolsApp;

		/// <summary>gUi symbol is a pointer to the UserInterface class, which provides access to the user interface elements.</summary>
        extern Ptr<UserInterface> toolsUI;

        /// <summary>LocaleID holds the default locale identifier, initialized to "en-US" for English (Unites States).</summary>
        extern std::string toolsLocaleId;
    }
}
