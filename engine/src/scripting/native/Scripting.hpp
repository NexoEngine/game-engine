//// Scripting.hpp ////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Guillaume HEIN
//  Date:        30/04/2025
//  Description: Header file for the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <format>

#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>
#include <boost/dll.hpp>

#include "HostString.hpp"
#include "ManagedApi.hpp"
#include "Path.hpp"

#ifdef WIN32
    #define STR(s) L ## s
    #define CH(c) L ## c
    #define DIR_SEPARATOR L'\\'
#else
    #include <dlfcn.h>

    #define STR(s) s
    #define CH(c) c
    #define DIR_SEPARATOR '/'
    #define MAX_PATH PATH_MAX
#endif

namespace nexo::scripting {

    struct HostfxrFn {
        hostfxr_set_error_writer_fn set_error_writer;
        hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line;
        hostfxr_initialize_for_runtime_config_fn init_for_config;
        hostfxr_get_runtime_delegate_fn get_delegate;
        hostfxr_run_app_fn run_app;
        hostfxr_close_fn close;
    };

    struct CoreclrDelegate {
        load_assembly_fn load_assembly;
        load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer;
        get_function_pointer_fn get_function_pointer;
    };

    class ScriptingBackendInitFailed final : public Exception {
        public:
            explicit ScriptingBackendInitFailed(const std::string &message,
                const std::source_location loc = std::source_location::current())
                    : Exception("Couldn't load scripting backend: " + message, loc) {}
    };

    class HostHandler {
        protected:
            static void defaultErrorCallback(const HostString& message);
        public:
            using ErrorCallBackFn = std::function<void(const HostString& message)>;

            // Globals
            static inline const std::filesystem::path DEFAULT_NEXO_MANAGED_PATH =
                Path::resolvePathRelativeToExe("."); // TODO: Change it later for packing
            static inline const std::string     NEXO_RUNTIMECONFIG_FILENAME  = "Nexo.runtimeconfig.json";
            static inline const std::string     NEXO_ASSEMBLY_FILENAME       = "Nexo.dll";
            static inline const ErrorCallBackFn DEFAULT_ERROR_CALLBACK       = HostHandler::defaultErrorCallback;

        protected:
            // Singleton: protected constructor and destructor
            HostHandler() = default;
            ~HostHandler();

        public:
            // Singleton: Meyers' Singleton Pattern
            static HostHandler& getInstance()
            {
                static HostHandler s_instance;
                return s_instance;
            }

            // Singleton: delete copy constructor and assignment operator
            HostHandler(HostHandler const&)   = delete;
            void operator=(HostHandler const&) = delete;

            enum Status {
                SUCCESS,
                UNINITIALIZED,

                HOSTFXR_NOT_FOUND,
                HOSTFXR_LOAD_ERROR,

                RUNTIME_CONFIG_NOT_FOUND,
                INIT_DOTNET_RUNTIME_ERROR,

                GET_DELEGATES_ERROR,

                ASSEMBLY_NOT_FOUND,
                LOAD_ASSEMBLY_ERROR,

                INIT_MANAGED_API_ERROR,

                INIT_CALLBACKS_ERROR,

            };


            struct Parameters {
                // Hostfxr parameters, see https://github.com/dotnet/runtime/blob/main/docs/design/features/native-hosting.md#initialize-host-context
                std::filesystem::path assemblyPath;
                std::filesystem::path dotnetRoot;

                // Nexo
                std::filesystem::path nexoManagedPath = DEFAULT_NEXO_MANAGED_PATH;

                ErrorCallBackFn errorCallback = DEFAULT_ERROR_CALLBACK;
            };
            static inline ErrorCallBackFn currentErrorCallback = nullptr;

            Status initialize(Parameters parameters);

            [[nodiscard]] const ManagedApi& getManagedApi() const
            {
                return m_managedApi;
            }

            inline void *getManagedFptrVoid(const char_t *typeName, const char_t *methodName, const char_t *delegateTypeName) const;

            // activate if you want to use the function pointer directly
            template <typename T>
            inline T getManagedFptr(const HostString& typeName, const HostString& methodName, const HostString& delegateTypeName)
            {
                return reinterpret_cast<T>(getManagedFptrVoid(typeName.c_str(), methodName.c_str(), delegateTypeName.c_str()));
            }

            enum ManagedFptrFlags {
                NONE = 0,
                UNMANAGEDCALLERSONLY = 1 << 0,
            };

            template <typename T>
            inline T getManagedFptr(const HostString& typeName, const HostString& methodName, const ManagedFptrFlags flags = NONE)
            {
                const char_t *delegateTypeName = flags & UNMANAGEDCALLERSONLY ? UNMANAGEDCALLERSONLY_METHOD : nullptr;
                return reinterpret_cast<T>(getManagedFptrVoid(typeName.c_str(), methodName.c_str(), delegateTypeName));
            }


            int runScriptExample() const;

        protected:

            Status loadHostfxr();
            Status initRuntime();
            Status getRuntimeDelegates();
            Status loadManagedAssembly();
            Status initManagedApi();
            Status checkManagedApi();
            Status initCallbacks();



        protected:
            Status m_status = UNINITIALIZED;
            Parameters m_params{};
            HostString m_assembly_path{};

            HostfxrFn m_hostfxr_fn = {};
            CoreclrDelegate m_delegates = {};
            ManagedApi m_managedApi = {};

            std::shared_ptr<boost::dll::shared_library> m_dll_handle = nullptr;
            hostfxr_handle m_host_ctx = nullptr;
    };


    int runScriptExample(const HostHandler::Parameters& params);

    // Function to register native callback functions
    void registerNativeFunctions();
    
} // namespace nexo::scripting
