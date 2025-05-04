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

#include <codecvt>

#include "Scripting.hpp"
#include "HostString.hpp"
#include "Logger.hpp"


// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

// Standard headers
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// Provided by the AppHost NuGet package and installed as an SDK pack
#include <nethost.h>

// Header files copied from https://github.com/dotnet/core-setup
#include <coreclr_delegates.h>
#include <filesystem>
#include <hostfxr.h>

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

using string_t = std::basic_string<char_t>;

namespace nexo::scripting {
    std::shared_ptr<boost::dll::shared_library> shared_library_handle = nullptr;

    hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr = nullptr;
    hostfxr_initialize_for_runtime_config_fn init_for_config_fptr = nullptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr = nullptr;
    hostfxr_run_app_fn run_app_fptr = nullptr;
    hostfxr_close_fn close_fptr = nullptr;

    int run_component_example(const HostString& root_path)
    {
        //
        // STEP 1: Load HostFxr and get exported hosting functions
        //
        if (!load_hostfxr())
        {
            assert(false && "Failure: load_hostfxr()");
            return EXIT_FAILURE;
        }

        //
        // STEP 2: Initialize and start the .NET Core runtime
        //
        const HostString config_path = root_path + STR("Nexo.runtimeconfig.json");
        load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
        load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
        assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

        //
        // STEP 3: Load managed assembly and get function pointer to a managed method
        //
        const HostString dotnetlib_path = root_path + STR("Nexo.dll");
        const char_t *dotnet_type = STR("Nexo.Lib, Nexo");
        const char_t *dotnet_type_method = STR("Hello");
        // <SnippetLoadAndGet>
        // Function pointer to managed delegate
        component_entry_point_fn hello = nullptr;
        int rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            dotnet_type_method,
            nullptr /*delegate_type_name*/,
            nullptr,
            (void**)&hello);
        // </SnippetLoadAndGet>
        assert(rc == 0 && hello != nullptr && "Failure: load_assembly_and_get_function_pointer()");

        //
        // STEP 4: Run managed code
        //
        struct lib_args
        {
            const char_t *message;
            int number;
        };
        for (int i = 0; i < 3; ++i)
        {
            // <SnippetCallManaged>
            lib_args args
            {
                STR("from host!"),
                i
            };

            hello(&args, sizeof(args));
            // </SnippetCallManaged>
        }

        // Function pointer to managed delegate with non-default signature
        typedef void (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(lib_args args);
        custom_entry_point_fn custom = nullptr;
        lib_args args
        {
            STR("from host!"),
            -1
        };

        // UnmanagedCallersOnly
        rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            STR("CustomEntryPointUnmanagedCallersOnly") /*method_name*/,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            (void**)&custom);
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        // Custom delegate type
        rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            STR("CustomEntryPoint") /*method_name*/,
            STR("Nexo.Lib+CustomEntryPointDelegate, Nexo") /*delegate_type_name*/,
            nullptr,
            (void**)&custom);
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        return EXIT_SUCCESS;
    }

    HostHandler::~HostHandler()
    {
        if (shared_library_handle) {
            if (m_hostfxr_fn.close) {
                close_fptr(nullptr);
            }
        }
    }

    HostHandler::Status HostHandler::initialize(Parameters parameters)
    {
        if (m_status == SUCCESS)
            return m_status;

        m_params = std::move(parameters);

        if (!m_params.errorCallback)
            m_params.errorCallback = DEFAULT_ERROR_CALLBACK;
        if (m_params.nexoManagedPath.empty())
            m_params.nexoManagedPath = DEFAULT_NEXO_MANAGED_PATH;

        if (loadHostfxr() != SUCCESS)
            return m_status;

        currentErrorCallback = m_params.errorCallback;
        m_hostfxr_fn.set_error_writer([](const char_t* message) {
            currentErrorCallback(message);
        });

        if (initRuntime() != SUCCESS)
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
        if (int rc = get_hostfxr_path(buffer, &buffer_size, &params)) {
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
        int rc = init_for_config_fptr(configPath.c_str(), nullptr, &m_host_ctx);
        if (rc != 0 || m_host_ctx == nullptr)
        {
            m_params.errorCallback(std::format("Init failed: 0x{:X}", rc));
            close_fptr(m_host_ctx);
            return m_status = INIT_DOTNET_RUNTIME_ERROR;
        }
        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::getRuntimeDelegates()
    {
        int rc = 0;

        rc = get_delegate_fptr(m_host_ctx, hdt_load_assembly, reinterpret_cast<void **>(&m_delegates.load_assembly));
        if (rc != 0 || m_delegates.load_assembly == nullptr) {
            m_params.errorCallback(std::format("Failed to get 'load_assembly' delegate: 0x{:X}", rc));
            return m_status = GET_DELEGATES_ERROR;
        }

        rc = get_delegate_fptr(m_host_ctx, hdt_get_function_pointer, reinterpret_cast<void **>(&m_delegates.get_function_pointer));
        if (rc != 0 || m_delegates.get_function_pointer == nullptr) {
            m_params.errorCallback(std::format("Failed to get 'get_function_pointer' delegate: 0x{:X}", rc));
            return m_status = GET_DELEGATES_ERROR;
        }
        return m_status = SUCCESS;
    }

/********************************************************************************************
 * Function used to load and activate .NET Core
 ********************************************************************************************/
    // <SnippetLoadHostFxr>
    // Using the nethost library, discover the location of hostfxr and get exports
    bool load_hostfxr(const HostString& assembly_path /* = "" */)
    {
        get_hostfxr_parameters params {
            sizeof(get_hostfxr_parameters),
            assembly_path.empty() ? nullptr : assembly_path.c_str(),
            nullptr };
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        int rc = get_hostfxr_path(buffer, &buffer_size, &params);
        if (rc != 0)
            return false;

        // Load hostfxr and get desired exports
        // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
        // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
        shared_library_handle = std::make_shared<boost::dll::shared_library>(buffer);
        if (shared_library_handle == nullptr)
        {
            std::cerr << "Failed to load hostfxr library." << std::endl;
            return false;
        }

        init_for_cmd_line_fptr = shared_library_handle->get<std::remove_pointer_t<hostfxr_initialize_for_dotnet_command_line_fn>>("hostfxr_initialize_for_dotnet_command_line");
        init_for_config_fptr = shared_library_handle->get<std::remove_pointer_t<hostfxr_initialize_for_runtime_config_fn>>("hostfxr_initialize_for_runtime_config");
        get_delegate_fptr = shared_library_handle->get<std::remove_pointer_t<hostfxr_get_runtime_delegate_fn>>("hostfxr_get_runtime_delegate");
        run_app_fptr = shared_library_handle->get<std::remove_pointer_t<hostfxr_run_app_fn>>("hostfxr_run_app");
        close_fptr = shared_library_handle->get<std::remove_pointer_t<hostfxr_close_fn>>("hostfxr_close");

        return (init_for_cmd_line_fptr && init_for_config_fptr && get_delegate_fptr && run_app_fptr && close_fptr);
    }
    // </SnippetLoadHostFxr>

    // <SnippetInitialize>
    // Load and initialize .NET Core and get desired function pointer for scenario
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const HostString& config_path)
    {
        // Load .NET Core
        void *load_assembly_and_get_function_pointer = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = init_for_config_fptr(config_path.c_str(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
            close_fptr(cxt);
            return nullptr;
        }

        // Get the load assembly function pointer
        rc = get_delegate_fptr(
            cxt,
            hdt_load_assembly_and_get_function_pointer,
            &load_assembly_and_get_function_pointer);
        if (rc != 0 || load_assembly_and_get_function_pointer == nullptr)
            std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

        close_fptr(cxt);
        return (load_assembly_and_get_function_pointer_fn)load_assembly_and_get_function_pointer;
    }
    // </SnippetInitialize>
}
