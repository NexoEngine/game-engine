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
#include <hostfxr.h>
#include <boost/dll.hpp>

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
    bool load_hostfxr(const char_t *assembly_path);
    load_assembly_and_get_function_pointer_fn get_dotnet_load_assembly(const char_t *assembly);

    int run_component_example(const string_t& root_path);
    int run_app_example(const string_t& root_path);

} // namespace nexo::scripting
