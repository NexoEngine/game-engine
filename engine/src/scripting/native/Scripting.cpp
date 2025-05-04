//// Scripting.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        30/04/2025
//  Description: Source file for the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#include <nethost.h>
#include <filesystem>
#include <hostfxr.h>

#include "Scripting.hpp"
#include "HostString.hpp"
#include "Logger.hpp"

namespace nexo::scripting {

    HostHandler::~HostHandler()
    {
        if (m_dll_handle) {
            if (m_hostfxr_fn.close) {
                m_hostfxr_fn.close(nullptr);
            }
        }
    }

    HostHandler::Status HostHandler::initialize(Parameters parameters)
    {
        if (m_status == SUCCESS)
            return m_status;

        m_params = std::move(parameters);

        if (loadHostfxr() != SUCCESS)
            return m_status;

        currentErrorCallback = m_params.errorCallback;
        m_hostfxr_fn.set_error_writer([](const char_t* message) {
            currentErrorCallback(message);
        });

        if (initRuntime() != SUCCESS)
            return m_status;

        if (getRuntimeDelegates() != SUCCESS)
            return m_status;

        if (loadManagedAssembly() != SUCCESS)
            return m_status;

        return m_status = SUCCESS;
    }

    void HostHandler::defaultErrorCallback(const HostString& message)
    {
        std::cerr << "[Scripting] Error: " << message.to_utf8() << std::endl;
    }

    HostHandler::Status HostHandler::loadHostfxr()
    {
        auto assemblyPath = HostString(m_params.assemblyPath.c_str());
        auto dotnetRoot = HostString(m_params.dotnetRoot.c_str());

        get_hostfxr_parameters params {
            sizeof(get_hostfxr_parameters),
            assemblyPath.empty() ? nullptr : assemblyPath.c_str(),
            dotnetRoot.empty() ? nullptr : dotnetRoot.c_str()
        };
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        if (unsigned int rc = get_hostfxr_path(buffer, &buffer_size, &params)) {
            m_params.errorCallback(std::format("Failed to get hostfxr path. Error code 0x{:X}.", rc));
            return m_status = HOSTFXR_NOT_FOUND;
        }

        m_dll_handle = std::make_shared<boost::dll::shared_library>(buffer);
        if (m_dll_handle == nullptr)
        {
            m_params.errorCallback(std::format("Failed to load hostfxr library from path: {}", HostString(buffer).to_utf8()));
            return m_status = HOSTFXR_LOAD_ERROR;
        }

        m_hostfxr_fn.set_error_writer = m_dll_handle->get<std::remove_pointer_t<hostfxr_set_error_writer_fn>>("hostfxr_set_error_writer");
        m_hostfxr_fn.init_for_cmd_line = m_dll_handle->get<std::remove_pointer_t<hostfxr_initialize_for_dotnet_command_line_fn>>("hostfxr_initialize_for_dotnet_command_line");
        m_hostfxr_fn.init_for_config = m_dll_handle->get<std::remove_pointer_t<hostfxr_initialize_for_runtime_config_fn>>("hostfxr_initialize_for_runtime_config");
        m_hostfxr_fn.get_delegate = m_dll_handle->get<std::remove_pointer_t<hostfxr_get_runtime_delegate_fn>>("hostfxr_get_runtime_delegate");
        m_hostfxr_fn.run_app = m_dll_handle->get<std::remove_pointer_t<hostfxr_run_app_fn>>("hostfxr_run_app");
        m_hostfxr_fn.close = m_dll_handle->get<std::remove_pointer_t<hostfxr_close_fn>>("hostfxr_close");

        if (not (m_hostfxr_fn.set_error_writer && m_hostfxr_fn.init_for_cmd_line && m_hostfxr_fn.init_for_config
                && m_hostfxr_fn.get_delegate && m_hostfxr_fn.run_app && m_hostfxr_fn.close)) {
            m_params.errorCallback(std::format("Failed to load hostfxr functions from path: {}", HostString(buffer).to_utf8()));
            return m_status = HOSTFXR_LOAD_ERROR;
        }

        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::initRuntime()
    {
        const std::filesystem::path runtimeConfigPath =
            m_params.nexoManagedPath / NEXO_RUNTIMECONFIG_FILENAME;

        if (!std::filesystem::exists(runtimeConfigPath))
        {
            m_params.errorCallback(std::format("Nexo runtime config file not found: {}", runtimeConfigPath.string()));
            return m_status = RUNTIME_CONFIG_NOT_FOUND;
        }

        const HostString configPath = runtimeConfigPath.c_str();

        // Load .NET Core
        unsigned int rc = m_hostfxr_fn.init_for_config(configPath.c_str(), nullptr, &m_host_ctx);
        if (rc != 0 || m_host_ctx == nullptr) {
            m_params.errorCallback(std::format("Init failed: 0x{:X}", rc));
            m_hostfxr_fn.close(m_host_ctx);
            return m_status = INIT_DOTNET_RUNTIME_ERROR;
        }
        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::getRuntimeDelegates()
    {
        unsigned int rc = 0;

        rc = m_hostfxr_fn.get_delegate(m_host_ctx, hdt_load_assembly, reinterpret_cast<void **>(&m_delegates.load_assembly));
        if (rc != 0 || m_delegates.load_assembly == nullptr) {
            m_params.errorCallback(std::format("Failed to get 'load_assembly' delegate: 0x{:X}", rc));
            return m_status = GET_DELEGATES_ERROR;
        }

        rc = m_hostfxr_fn.get_delegate(m_host_ctx, hdt_load_assembly_and_get_function_pointer,
            reinterpret_cast<void **>(&m_delegates.load_assembly_and_get_function_pointer));
        if (rc != 0 || m_delegates.load_assembly_and_get_function_pointer == nullptr) {
            m_params.errorCallback(std::format("Failed to get 'load_assembly_and_get_function_pointer' delegate: 0x{:X}", rc));
            return m_status = GET_DELEGATES_ERROR;
        }

        rc = m_hostfxr_fn.get_delegate(m_host_ctx, hdt_get_function_pointer,
            reinterpret_cast<void **>(&m_delegates.get_function_pointer));
        if (rc != 0 || m_delegates.get_function_pointer == nullptr) {
            m_params.errorCallback(std::format("Failed to get 'get_function_pointer' delegate: 0x{:X}", rc));
            return m_status = GET_DELEGATES_ERROR;
        }
        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::loadManagedAssembly()
    {
        unsigned int rc = 0;

        const std::filesystem::path assemblyPath =
            m_params.nexoManagedPath / NEXO_ASSEMBLY_FILENAME;

        if (!std::filesystem::exists(assemblyPath)) {
            m_params.errorCallback(std::format("Nexo assembly file not found: {}", assemblyPath.string()));
            return m_status = ASSEMBLY_NOT_FOUND;
        }

        const HostString assemblyPathStr = assemblyPath.c_str();

        rc = m_delegates.load_assembly(assemblyPathStr.c_str(), nullptr, nullptr);
        if (rc != 0) {
            m_params.errorCallback(std::format("Failed to load assembly at {}: 0x{:X}", assemblyPathStr.to_utf8(), rc));
            return m_status = LOAD_ASSEMBLY_ERROR;
        }
        m_assembly_path = assemblyPathStr;
        return m_status = SUCCESS;
    }

    int runScriptExample(const HostHandler::Parameters& params)
    {
        // Get the instance of the HostHandler singleton
        HostHandler& host = HostHandler::getInstance();

        // Initialize the host
        HostHandler::Status status = host.initialize(params);
        if (status != HostHandler::SUCCESS) {
            return EXIT_FAILURE;
        }

        // Get function pointers to managed methods
        // Regular method
        component_entry_point_fn hello = host.getManagedFptr<component_entry_point_fn>(
            STR("Nexo.Lib, Nexo"),
            STR("Hello"),
            nullptr
        );

        if (hello == nullptr) {
            return EXIT_FAILURE;
        }

        // Run managed code
        struct lib_args {
            const char_t* message;
            int number;
        };

        // Call the Hello method multiple times
        for (int i = 0; i < 3; ++i) {
            lib_args args {
                STR("from host!"),
                i
            };

            hello(&args, sizeof(args));
        }

        // Get function pointer for UnmanagedCallersOnly method
        typedef void (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(lib_args args);
        custom_entry_point_fn custom_unmanaged = host.getManagedFptr<custom_entry_point_fn>(
            STR("Nexo.Lib, Nexo"),
            STR("CustomEntryPointUnmanagedCallersOnly"),
            UNMANAGEDCALLERSONLY_METHOD
        );

        if (custom_unmanaged == nullptr) {
            return EXIT_FAILURE;
        }

        // Call UnmanagedCallersOnly method
        lib_args args_unmanaged {
            STR("from host!"),
            -1
        };
        custom_unmanaged(args_unmanaged);

        // Get function pointer for custom delegate type method
        custom_entry_point_fn custom = host.getManagedFptr<custom_entry_point_fn>(
            STR("Nexo.Lib, Nexo"),
            STR("CustomEntryPoint"),
            STR("Nexo.Lib+CustomEntryPointDelegate, Nexo")
        );

        if (custom == nullptr) {
            return EXIT_FAILURE;
        }

        // Call custom delegate type method
        custom(args_unmanaged);

        return EXIT_SUCCESS;
    }

}
