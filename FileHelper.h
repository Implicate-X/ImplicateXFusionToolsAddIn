#pragma once

namespace implicatex {
    namespace fusion {
#ifdef _WIN32
        std::string getEnvVar(const char* name);
#endif
        std::string getUserSettingsPath();
#if __cplusplus >= 201703L
        void ensureUserSettingsDirectoryExists();
#endif
    }
}
