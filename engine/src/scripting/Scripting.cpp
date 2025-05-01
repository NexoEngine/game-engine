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

namespace nexo::scripting
{
    std::shared_ptr<boost::dll::shared_library> shared_library_handle = nullptr;

    hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr = nullptr;
    hostfxr_initialize_for_runtime_config_fn init_for_config_fptr = nullptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr = nullptr;
    hostfxr_run_app_fn run_app_fptr = nullptr;
    hostfxr_close_fn close_fptr = nullptr;

    int run_component_example(const string_t& root_path)
    {
        //
        // STEP 1: Load HostFxr and get exported hosting functions
        //
        if (!load_hostfxr(nullptr))
        {
            assert(false && "Failure: load_hostfxr()");
            return EXIT_FAILURE;
        }

        //
        // STEP 2: Initialize and start the .NET Core runtime
        //
        const string_t config_path = root_path + STR("DotNetLib.runtimeconfig.json");
        load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
        load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
        assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

        //
        // STEP 3: Load managed assembly and get function pointer to a managed method
        //
        const string_t dotnetlib_path = root_path + STR("DotNetLib.dll");
        const char_t *dotnet_type = STR("DotNetLib.Lib, DotNetLib");
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
            STR("DotNetLib.Lib+CustomEntryPointDelegate, DotNetLib") /*delegate_type_name*/,
            nullptr,
            (void**)&custom);
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        return EXIT_SUCCESS;
    }

    int run_app_example(const string_t& root_path)
    {
        const string_t app_path = root_path + STR("App.dll");

        if (!load_hostfxr(app_path.c_str()))
        {
            assert(false && "Failure: load_hostfxr()");
            return EXIT_FAILURE;
        }

        // Load .NET Core
        hostfxr_handle cxt = nullptr;
        std::vector<const char_t*> args { app_path.c_str(), STR("app_arg_1"), STR("app_arg_2") };
        int rc = init_for_cmd_line_fptr(args.size(), args.data(), nullptr, &cxt);
        if (rc != 0 || cxt == nullptr)
        {
            std::cerr << "Init failed: " << std::hex << std::showbase << rc << std::endl;
            close_fptr(cxt);
            return EXIT_FAILURE;
        }

        // Get the function pointer to get function pointers
        get_function_pointer_fn get_function_pointer;
        rc = get_delegate_fptr(
            cxt,
            hdt_get_function_pointer,
            (void**)&get_function_pointer);
        if (rc != 0 || get_function_pointer == nullptr)
            std::cerr << "Get delegate failed: " << std::hex << std::showbase << rc << std::endl;

        // Function pointer to App.IsWaiting
        typedef unsigned char (CORECLR_DELEGATE_CALLTYPE* is_waiting_fn)();
        is_waiting_fn is_waiting;
        rc = get_function_pointer(
            STR("App, App"),
            STR("IsWaiting"),
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr, nullptr, (void**)&is_waiting);
        assert(rc == 0 && is_waiting != nullptr && "Failure: get_function_pointer()");

        // Function pointer to App.Hello
        typedef void (CORECLR_DELEGATE_CALLTYPE* hello_fn)(const char*);
        hello_fn hello;
        rc = get_function_pointer(
            STR("App, App"),
            STR("Hello"),
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr, nullptr, (void**)&hello);
        assert(rc == 0 && hello != nullptr && "Failure: get_function_pointer()");

        // Invoke the functions in a different thread from the main app
        std::thread t([&]
        {
            while (is_waiting() != 1)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

            for (int i = 0; i < 3; ++i)
                hello("from host!");
        });

        // Run the app
        run_app_fptr(cxt);
        t.join();

        close_fptr(cxt);
        return EXIT_SUCCESS;
    }
}


/********************************************************************************************
 * Function used to load and activate .NET Core
 ********************************************************************************************/

namespace nexo::scripting
{
    // Forward declarations
    void *load_library(const char_t *);
    void *get_export(void *, const char *);

#ifdef WIN32
    void *load_library(const char_t *path)
    {
        HMODULE h = ::LoadLibraryW(path);
        assert(h != nullptr);
        return (void*)h;
    }
    void *get_export(void *h, const char *name)
    {
        void *f = ::GetProcAddress((HMODULE)h, name);
        assert(f != nullptr);
        return f;
    }
#else
    void *load_library(const char_t *path)
    {
        void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
        assert(h != nullptr);
        return h;
    }
    void *get_export(void *h, const char *name)
    {
        void *f = dlsym(h, name);
        assert(f != nullptr);
        return f;
    }
#endif

    // <SnippetLoadHostFxr>
    // Using the nethost library, discover the location of hostfxr and get exports
    bool load_hostfxr(const char_t *assembly_path)
    {
        get_hostfxr_parameters params { sizeof(get_hostfxr_parameters), assembly_path, nullptr };
        // Pre-allocate a large buffer for the path to hostfxr
        char_t buffer[MAX_PATH];
        size_t buffer_size = sizeof(buffer) / sizeof(char_t);
        int rc = get_hostfxr_path(buffer, &buffer_size, &params);
        if (rc != 0)
            return false;

        // Load hostfxr and get desired exports
        // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
        // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
        void *lib = load_library(buffer);
        init_for_cmd_line_fptr = (hostfxr_initialize_for_dotnet_command_line_fn)get_export(lib, "hostfxr_initialize_for_dotnet_command_line");
        init_for_config_fptr = (hostfxr_initialize_for_runtime_config_fn)get_export(lib, "hostfxr_initialize_for_runtime_config");
        get_delegate_fptr = (hostfxr_get_runtime_delegate_fn)get_export(lib, "hostfxr_get_runtime_delegate");
        run_app_fptr = (hostfxr_run_app_fn)get_export(lib, "hostfxr_run_app");
        close_fptr = (hostfxr_close_fn)get_export(lib, "hostfxr_close");

        return (init_for_config_fptr && get_delegate_fptr && close_fptr);
    }
    // </SnippetLoadHostFxr>

    // <SnippetInitialize>
    // Load and initialize .NET Core and get desired function pointer for scenario
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path)
    {
        // Load .NET Core
        void *load_assembly_and_get_function_pointer = nullptr;
        hostfxr_handle cxt = nullptr;
        int rc = init_for_config_fptr(config_path, nullptr, &cxt);
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

/*
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

    // init globals
    std::shared_ptr<boost::dll::shared_library> shared_library_handle = nullptr;

    // Function pointers for hostfxr functions
    hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr = nullptr;
    hostfxr_initialize_for_runtime_config_fn init_for_config_fptr = nullptr;
    hostfxr_get_runtime_delegate_fn get_delegate_fptr = nullptr;
    hostfxr_run_app_fn run_app_fptr = nullptr;
    hostfxr_close_fn close_fptr = nullptr;



    std::string convert_wstring(std::wstring& wstr)
    {
        std::string str(wstr.length(), 0);
        std::ranges::transform(wstr, str.begin(), [] (const wchar_t c) {
            return static_cast<char>(c);
        });
        return str;
    }

    int load_hostfxr()
    {
        int rc = EXIT_SUCCESS;
        size_t buffer_size = 0;
        // Get the required path length according to nethost API
        get_hostfxr_path(nullptr, &buffer_size, nullptr);
        // Allocate the buffer
        std::wstring buffer(buffer_size + 1, '\0');
        // Get the path to hostfxr
        rc = get_hostfxr_path(buffer.data(), &buffer_size, nullptr);
        if (rc != 0)
            return rc;

        LOG(NEXO_INFO, "Found hostfxr path: {}", convert_wstring(buffer));

        // Load hostfxr and get desired exports
        // NOTE: The .NET Runtime does not support unloading any of its native libraries. Running
        // dlclose/FreeLibrary on any .NET libraries produces undefined behavior.
        LOG(NEXO_INFO, "AHHHH1");

        std::shared_ptr<boost::dll::shared_library> lib = std::make_shared<boost::dll::shared_library>(buffer);
        LOG(NEXO_INFO, "AHHHH2");

        if (lib->has("hostfxr_initialize_for_dotnet_command_line") == false ||
            lib->has("hostfxr_initialize_for_runtime_config") == false ||
            lib->has("hostfxr_get_runtime_delegate") == false ||
            lib->has("hostfxr_run_app") == false ||
            lib->has("hostfxr_close") == false)
        {
            std::cerr << "Failed to load hostfxr functions." << std::endl;
            return EXIT_FAILURE;
        }

        // Get function pointers using boost::dll
        init_for_cmd_line_fptr = lib->get<hostfxr_initialize_for_dotnet_command_line_fn>("hostfxr_initialize_for_dotnet_command_line");
        init_for_config_fptr = lib->get<hostfxr_initialize_for_runtime_config_fn>("hostfxr_initialize_for_runtime_config");
        get_delegate_fptr = lib->get<hostfxr_get_runtime_delegate_fn>("hostfxr_get_runtime_delegate");
        run_app_fptr = lib->get<hostfxr_run_app_fn>("hostfxr_run_app");
        close_fptr = lib->get<hostfxr_close_fn>("hostfxr_close");

        // Store the library handle to keep it loaded
        shared_library_handle = lib;

        LOG(NEXO_INFO, "ptr: {}", (uint64_t)init_for_config_fptr);
        LOG(NEXO_INFO, "AHHHH3");
        std::cout << "adzad" << std::flush;

        // Verify all functions were loaded successfully
        return (!init_for_cmd_line_fptr || !init_for_config_fptr || !get_delegate_fptr || !run_app_fptr || !close_fptr);
    }

    // Load and initialize .NET Core and get desired function pointer for scenario
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *config_path)
    {
        // Load .NET Core
        void *load_assembly_and_get_function_pointer = nullptr;
        hostfxr_handle cxt = nullptr;
        LOG(NEXO_INFO, "AHHHH");
        LOG(NEXO_INFO, "ptr: {}", (uint64_t)init_for_config_fptr);
        assert(init_for_config_fptr != nullptr && "Failure: init_for_config_fptr()");
        void *test = init_for_config_fptr;
        int rc = init_for_config_fptr(config_path, nullptr, &cxt);
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

    int run_component_example(const string_t& root_path)
    {
        //
        // STEP 1: Load HostFxr and get exported hosting functions
        //
        if (load_hostfxr())
        {
            assert(false && "Failure: load_hostfxr()");
            return EXIT_FAILURE;
        }

        //
        // STEP 2: Initialize and start the .NET Core runtime
        //
        const string_t config_path = root_path + STR("DotNetLib.runtimeconfig.json");
        load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer = nullptr;
        load_assembly_and_get_function_pointer = get_dotnet_load_assembly(config_path.c_str());
        assert(load_assembly_and_get_function_pointer != nullptr && "Failure: get_dotnet_load_assembly()");

        //
        // STEP 3: Load managed assembly and get function pointer to a managed method
        //
        const string_t dotnetlib_path = root_path + STR("DotNetLib.dll");
        const char_t *dotnet_type = STR("DotNetLib.Lib, DotNetLib");
        const char_t *dotnet_type_method = STR("Hello");
        // <SnippetLoadAndGet>
        // Function pointer to managed delegate
        component_entry_point_fn hello = nullptr;
        int rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            dotnet_type_method,
            nullptr /*delegate_type_name#1#,
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
            STR("CustomEntryPointUnmanagedCallersOnly") /*method_name#1#,
            UNMANAGEDCALLERSONLY_METHOD,
            nullptr,
            (void**)&custom);
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        // Custom delegate type
        rc = load_assembly_and_get_function_pointer(
            dotnetlib_path.c_str(),
            dotnet_type,
            STR("CustomEntryPoint") /*method_name#1#,
            STR("DotNetLib.Lib+CustomEntryPointDelegate, DotNetLib") /*delegate_type_name#1#,
            nullptr,
            (void**)&custom);
        assert(rc == 0 && custom != nullptr && "Failure: load_assembly_and_get_function_pointer()");
        custom(args);

        return EXIT_SUCCESS;
    }
}
*/
