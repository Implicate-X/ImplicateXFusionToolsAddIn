#ifdef _WIN32
#include <cstdlib>
#include <memory>
#endif
#include "pch.h"
#include "resource.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "FileHelper.h"

#if __cplusplus >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#endif

#ifdef _WIN32
namespace implicatex {
    namespace fusion {
        std::string getEnvVar(const char* name) {
            char* buffer = nullptr;
            size_t size = 0;
            if (_dupenv_s(&buffer, &size, name) == 0 && buffer) {
                std::string value(buffer);
                free(buffer);
                return value;
            }
            return {};
        }
    }
}
#endif

namespace implicatex {
    namespace fusion {
        std::string getUserSettingsPath() {
#ifdef _WIN32
            std::string appData = getEnvVar("APPDATA");
            if (!appData.empty())
                return std::string(appData) + "\\Autodesk\\FusionAddins\\ImplicateX\\Settings.json";
#else
            const char* home = std::getenv("HOME");
            if (home)
                return std::string(home) + "/.config/Autodesk/FusionAddins/ImplicateX/Settings.json";
#endif
            // Fallback: Current Directory
            return "Settings.json";
        }

#if __cplusplus >= 201703L
        void ensureUserSettingsDirectoryExists() {
            fs::path settingsPath(getUserSettingsPath());
            fs::create_directories(settingsPath.parent_path());
        }
#endif
    }
}