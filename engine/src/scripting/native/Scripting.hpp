//// Scripting.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        30/04/2025
//  Description: Header file for the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <nethost.h>
#include <coreclr_delegates.h>
#include <filesystem>
#include <hostfxr.h>
#include <boost/dll.hpp>

#include "HostString.hpp"
#include "Path.hpp"

#ifdef WIN32
    #include <Windows.h>

    #define STR(s) L ## s
    #define CH(c) L ## c
    #define DIR_SEPARATOR L'\\'

    #define string_compare wcscmp

#else
    #include <dlfcn.h>
    #include <limits.h>

    #define STR(s) s
    #define CH(c) c
    #define DIR_SEPARATOR '/'
    #define MAX_PATH PATH_MAX

    #define string_compare strcmp

#endif

namespace nexo::scripting {

    using string_t = std::basic_string<char_t>;

    extern std::shared_ptr<boost::dll::shared_library> shared_library_handle;

    // Function pointers for hostfxr functions
    extern hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr;
    extern hostfxr_initialize_for_runtime_config_fn init_for_config_fptr;
    extern hostfxr_get_runtime_delegate_fn get_delegate_fptr;
    extern hostfxr_run_app_fn run_app_fptr;
    extern hostfxr_close_fn close_fptr;

    // Function to load the hostfxr library and retrieve function pointers
    bool load_hostfxr(const HostString& assembly_path = "");
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const HostString& assembly);

    int run_component_example(const HostString& root_path);

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

    class HostHandler {
        protected:
            static void defaultErrorCallback(const HostString& message);
        public:
            typedef void(*ErrorCallBackFn)(const HostString& message);

            // Globals
            static inline const std::filesystem::path DEFAULT_NEXO_MANAGED_PATH =
                Path::resolvePathRelativeToExe("../engine/src/scripting/managed/bin/Debug/net9.0/"); // TODO: Change it later for packing
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

            template <typename T>
            T getManagedFptr(const char_t *typeName, const char_t *methodName, const char_t *delegateTypeName)
            {
                if (m_status != SUCCESS) {
                    m_params.errorCallback("getManagedFptr: HostHandler not initialized");
                    return nullptr;
                }
                void *fptr = nullptr;
                unsigned int rc = m_delegates.get_function_pointer(typeName, methodName, delegateTypeName, m_host_ctx, nullptr, &fptr);
                if (rc != 0 || fptr == nullptr) {
                    m_params.errorCallback(std::format("Failed to get function pointer Type({}) Method({}): 0x{:X}",
                        typeName ? HostString(typeName).to_utf8() : "",
                        methodName ? HostString(methodName).to_utf8() : "",
                        rc)
                    );
                    return nullptr;
                }
                return reinterpret_cast<T>(fptr);
            }

        protected:

            Status loadHostfxr();
            Status initRuntime();
            Status getRuntimeDelegates();
            Status loadManagedAssembly();



        protected:
            Status m_status = UNINITIALIZED;
            Parameters m_params;

            HostfxrFn m_hostfxr_fn = {};
            CoreclrDelegate m_delegates = {};

            std::shared_ptr<boost::dll::shared_library> m_dll_handle = nullptr;
            hostfxr_handle m_host_ctx = nullptr;
    };

} // namespace nexo::scripting
