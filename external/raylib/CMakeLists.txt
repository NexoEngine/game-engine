if (UNIX)
    find_package(raylib 5.0 QUIET)
    if (NOT raylib_FOUND)
        include(FetchContent)
        FetchContent_Declare(
                raylib
                DOWNLOAD_EXTRACT_TIMESTAMP OFF
                URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
        )
        FetchContent_GetProperties(raylib)
        if (NOT raylib_POPULATED)
            set(FETCHCONTENT_QUIET NO)
            FetchContent_Populate(raylib)
            set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
            
            add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
                target_compile_options(raylib PUBLIC -w)
            elseif(MSVC)
                target_compile_options(raylib PUBLIC /W0)
            endif()
        endif()
    endif()
endif()
