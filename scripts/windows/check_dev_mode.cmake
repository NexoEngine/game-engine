function(is_win_developer_mode_enabled result)
    if (WIN32)
        cmake_host_system_information(RESULT result_var
                                      QUERY WINDOWS_REGISTRY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppModelUnlock"
                                      VALUE "AllowDevelopmentWithoutDevLicense"
                                      ERROR_VARIABLE error_var)

        if (${result_var})
            set(${result} TRUE PARENT_SCOPE)
        else ()
            message(DEBUG "Developer mode is not enabled on Windows: ${error_var}")
            set(${result} FALSE PARENT_SCOPE)
        endif ()
    else ()
        message(WARNING "Developer mode is only supported on Windows")
        set(${result} FALSE PARENT_SCOPE)
    endif ()
endfunction()
