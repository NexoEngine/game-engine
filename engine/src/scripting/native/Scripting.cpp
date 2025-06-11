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

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif


#include "Scripting.hpp"
#include "HostString.hpp"
#include "Logger.hpp"
#include "ManagedApi.hpp"
#include "NativeApi.hpp"
#include "ManagedTypedef.hpp"
#include "core/event/SignalEvent.hpp"

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

        // Take over signal handling because the CoreCLR library overrides them??
        event::SignalHandler::getInstance()->initSignals();

        if (initManagedApi() != SUCCESS)
            return m_status;

        if (initCallbacks() != SUCCESS)
            return m_status;

        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::update(Double deltaTime)
    {
        m_managedApi.NativeInterop.Update(deltaTime);
        return m_status;
    }

    void *HostHandler::getManagedFptrVoid(const char_t* typeName, const char_t* methodName,
        const char_t* delegateTypeName) const
    {
        if (m_status != SUCCESS) {
            m_params.errorCallback("getManagedFptr: HostHandler not initialized");
            return nullptr;
        }

        void *fptr = nullptr;
        unsigned int rc = m_delegates.load_assembly_and_get_function_pointer(
            m_assembly_path.c_str(), typeName, methodName, delegateTypeName, nullptr, &fptr);
        if (rc != 0 || fptr == nullptr) {
            m_params.errorCallback(std::format("Failed to get function pointer Type({}) Method({}): 0x{:X}",
                typeName ? HostString(typeName).to_utf8() : "",
                methodName ? HostString(methodName).to_utf8() : "",
                rc)
            );
            return nullptr;
        }
        return fptr;
    }

    void HostHandler::defaultErrorCallback(const HostString& message)
    {
        std::cerr << "[Scripting] Error: " << message.to_utf8() << std::endl;
    }

    HostHandler::Status HostHandler::loadHostfxr()
    {
        const auto assemblyPath = HostString(m_params.assemblyPath.c_str());
        const auto dotnetRoot = HostString(m_params.dotnetRoot.c_str());

        get_hostfxr_parameters params {
            sizeof(get_hostfxr_parameters),
            assemblyPath.empty() ? nullptr : assemblyPath.c_str(),
            dotnetRoot.empty() ? nullptr : dotnetRoot.c_str()
        };
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH] = {0};
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        if (unsigned int rc = get_hostfxr_path(buffer, &buffer_size, &params)) {
            m_params.errorCallback(std::format("Failed to get hostfxr path. Error code 0x{:X}.", rc));
            return m_status = HOSTFXR_NOT_FOUND;
        }

        m_dll_handle = std::make_shared<boost::dll::shared_library>(buffer, boost::dll::load_mode::default_mode);
        if (m_dll_handle == nullptr || !m_dll_handle->is_loaded()) {
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


    HostHandler::Status HostHandler::initManagedApi()
    try {
        m_managedApi.NativeInterop = {
            .Initialize = getManagedFptr<Int32(*)(NativeApiCallbacks*, Int32)>(
                "Nexo.NativeInterop, Nexo",
                "Initialize",
                UNMANAGEDCALLERSONLY
            ),
            .DemonstrateNativeCalls = getManagedFptr<void(*)()>(
                "Nexo.NativeInterop, Nexo",
                "DemonstrateNativeCalls",
                UNMANAGEDCALLERSONLY
            ),
            .Update = getManagedFptr<void(*)(Double)>(
                "Nexo.NativeInterop, Nexo",
                "Update",
                UNMANAGEDCALLERSONLY
            )
        };

        m_managedApi.Lib = {
            .CustomEntryPoint = getManagedFptr<void(*)(lib_args)>(
                "Nexo.Lib, Nexo",
                "CustomEntryPoint",
                "Nexo.Lib+CustomEntryPointDelegate, Nexo"
            ),
            .CustomEntryPointUnmanagedCallersOnly = getManagedFptr<void(*)(lib_args)>(
                "Nexo.Lib, Nexo",
                "CustomEntryPointUnmanagedCallersOnly",
                UNMANAGEDCALLERSONLY
            ),
            .Hello = getManagedFptr<void(*)(lib_args*, UInt32)>(
                "Nexo.Lib, Nexo",
                "Hello"
            ),
            .Add = getManagedFptr<Int32(*)(Int32, Int32)>(
                "Nexo.Lib, Nexo",
                "Add",
                UNMANAGEDCALLERSONLY
            ),
            .AddToPtr = getManagedFptr<Int32(*)(Int32, Int32, Int32*)>(
                "Nexo.Lib, Nexo",
                "AddToPtr",
                UNMANAGEDCALLERSONLY
            )
        };

        m_managedApi.ObjectFactory = {
            .CreateInstance = getManagedFptr<IntPtr(*)(const char *typeName, const char **parametersType, const void **parameters, UInt32 parametersCount)>(
                "Nexo.ObjectFactory, Nexo",
                "CreateInstance",
                UNMANAGEDCALLERSONLY
            )
        };

        return m_status = checkManagedApi();

    } catch (const std::exception& e) {
        m_params.errorCallback(std::format("Failed to initialize managed API: {}", e.what()));
        return m_status = INIT_MANAGED_API_ERROR;
    }

    HostHandler::Status HostHandler::checkManagedApi()
    {
        // Assert that the ManagedApiFn struct is properly defined
        static_assert(sizeof(ManagedApiFn<void()>) == sizeof(nullptr), "ManagedApiFn: struct size is not a pointer size");
        // Assert that the ManagedApi struct is properly defined (check that every field is void *)
        if constexpr (sizeof(ManagedApi) % sizeof(nullptr) != 0) {
            m_params.errorCallback("ManagedApi: struct size is not a multiple of pointer size");
        }

        // Check all fields of ManagedApi for nullptr
        constexpr size_t nbFields = sizeof(ManagedApi) / sizeof(void*);

        for (size_t i = 0; i < nbFields; ++i) {
            void **fieldPtr = reinterpret_cast<void**>(&m_managedApi) + i;
            if (*fieldPtr == nullptr) {
                m_params.errorCallback(std::format("ManagedApi: ManagedApiFn function pointer number {} is null in the struct", i));
                return m_status = INIT_MANAGED_API_ERROR;
            }
        }

        return m_status = SUCCESS;
    }

    HostHandler::Status HostHandler::initCallbacks()
    {
        // Initialize callbacks
        if (m_managedApi.NativeInterop.Initialize(&nativeApiCallbacks, sizeof(nativeApiCallbacks))) {
            m_params.errorCallback("Failed to initialize native API callbacks");
            return m_status = INIT_CALLBACKS_ERROR;
        }
        return m_status = SUCCESS;
    }

    int HostHandler::runScriptExample()
    {
        // Run managed code
        // Call the Hello method multiple times
        for (int i = 0; i < 3; ++i) {
            lib_args args {
                STR("from host!"),
                i
            };

            m_managedApi.Lib.Hello(&args, sizeof(args));
        }


        // Call UnmanagedCallersOnly method
        lib_args args_unmanaged {
            STR("from host!"),
            -1
        };
        m_managedApi.Lib.CustomEntryPointUnmanagedCallersOnly(args_unmanaged);


        // Call custom delegate type method
        m_managedApi.Lib.CustomEntryPoint(args_unmanaged);

        std::cout << "Testing Add(30, -10) = " << m_managedApi.Lib.Add(30, -10) << std::endl;

        int32_t result = 0;
        if (m_managedApi.Lib.AddToPtr(1000, 234, &result)) {
            std::cout << "addToPtr returned an error" << std::endl;
        } else {
            std::cout << "Testing AddToPtr(1000, 234, ptr), *ptr = " << result << std::endl;
        }

        // Demonstrate C# calling C++ (managed to native)
        // Call the method to demonstrate calling C++ from C#
        std::cout << "\nDemonstrating calling C++ functions from C#:" << std::endl;
        m_managedApi.NativeInterop.DemonstrateNativeCalls();

        // Demonstrate creating a managed object from native code
        std::cout << "\nDemonstrating creating a managed object from native code:" << std::endl;
        const char *typeName = "Nexo.ExampleClass";
        const char *parametersType[] = { "System.Int32" };
        // Create an instance of UInt32 with value 42
        UInt32 param1 = 42;
        const void *parameters[] = { &param1 };
        IntPtr instance = m_managedApi.ObjectFactory.CreateInstance(typeName, parametersType, parameters, 1);
        if (instance == nullptr) {
            m_params.errorCallback("Failed to create instance of " + std::string(typeName));
            return EXIT_FAILURE;
        }
        std::cout << "Created instance of " << typeName << " with address: " << instance << std::endl;

        return EXIT_SUCCESS;
    }

}
