#include "pch.h"
#include "resource.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include <Windows.h>

namespace implicatex {
    namespace fusion {
        Ptr<ToolsApp> toolsApp = nullptr;
        Ptr<UserInterface> toolsUI = nullptr;

		std::string toolsLocaleId = "en-US";

        /// <summary>
        /// <para>The implicatex::fusion::run function initializes the application and its user interface,</para>
        /// <para>returning a boolean value indicating success or failure.</para>
        /// </summary>
        ///
        /// <param name="context">
        /// <para>context is a pointer to a constant character string, typically used</para>
        /// <para>to represent a fixed message or information that should not be modified.</para>
        /// </param>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        extern "C" XI_EXPORT bool run(const char* context)
        {
            toolsApp = Application::get();
            if (!toolsApp)
                return false;

            toolsUI = toolsApp->userInterface();
            if (!toolsUI)
                return false;

			LOG_INFO("ToolsBar::initialize");

            if (!toolsApp->initialize())
                return false;

            return true;
        }

        /// <summary>
        /// <para>The stop function in the implicatex::fusion namespace terminates the application</para>
        /// <para>if the global user interface pointer is set, and returns true to indicate successful execution.</para>
        /// </summary>
        ///
        /// <param name="context">
        /// <para>context is a pointer to a constant character string, typically used</para>
        /// <para>to represent a fixed message or information that should not be modified.</para>
        /// </param>
        ///
        /// <returns>True if it succeeds, false if it fails.</returns>
        extern "C" XI_EXPORT bool stop(const char* context)
        {
            if (toolsUI)
            {
                toolsApp->terminate();

                toolsUI = nullptr;
            }

            return true;
        }

        #ifdef XI_WIN

        /// <summary>
        /// <para>The DllMain function is the entry point for a dynamic-link library (DLL)</para>
        /// <para>that handles initialization and cleanup tasks based on the reason for the DLL's invocation,</para>
        /// <para>such as process or thread attachment and detachment.</para>
        /// </summary>
        ///
        /// <param name="hmodule">
        /// <para>HMODULE hmodule is a handle to a module (such as a DLL) in the Windows operating system,</para>
        /// <para>used for managing and accessing the module's resources and functions.</para>
        /// </param>
        /// <param name="reason">
        /// <para>DWORD reason is a variable of type DWORD (a 32-bit unsigned integer)</para>
        /// <para>typically used to store a reason code or status in Windows programming.</para>
        /// </param>
        /// <param name="reserved">
        /// <para>LPVOID reserved is a pointer type in C++ that is typically used to hold a generic pointer</para>
        /// <para>to any data type, often reserved for future use or for compatibility purposes in APIs.</para>
        /// </param>
        ///
        /// <returns>An APIENTRY.</returns>
        BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
        {
            switch (reason)
            {
            case DLL_PROCESS_ATTACH:
            case DLL_THREAD_ATTACH:
            case DLL_THREAD_DETACH:
            case DLL_PROCESS_DETACH:
                break;
            }
            return TRUE;
        }

        #endif // XI_WIN
    }
}
