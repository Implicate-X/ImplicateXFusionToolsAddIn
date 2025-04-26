#pragma once
#include "ToolsApp.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;
using namespace icu;

namespace implicatex {
    namespace fusion {
        extern Ptr<ToolsApp> gApp;
        extern Ptr<UserInterface> gUi;

        /// <summary>LocaleID holds the default locale identifier, initialized to "en-US" for English (Unites States).</summary>
        extern std::string gLocaleId;

		const std::string IDS_SUBDIR_RESOURCES = "Resources/"; // Resources/
		const std::string IDS_ID_SOLID_ENV = "FusionSolidEnvironment"; // FusionSolidEnvironment
		const std::string IDS_ID_TOOLS_BAR = "ImplicateXFusionToolsAddInBar"; // ImplicateXFusionToolsAddInBar
		const std::string IDS_ID_TOOLS_RES = "Resources/logo"; // Resources/logo
		const std::string IDS_NAME_TOOLS_BAR = "Implicate-X"; // Implicate-X
		const std::string IDS_CMD_IMPLICATEX = "ImplicateXFusionToolsCommand"; // ImplicateXFusionToolsCommand
		const std::string IDS_ID_LANG_SELECTOR = "LanguageDropDownControl"; // LanguageDropDownControl
		const std::string IDS_CMD_LANG_SELECTOR = "LanguageDropDownCommand"; // LanguageDropDownCommand

    }
}
