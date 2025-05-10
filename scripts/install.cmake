#### install.cmake ############################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        24/11/2024
#  Description: CMake to pack the Nexo project and create an installer.
#
###############################################################################

cmake_minimum_required(VERSION 3.28)

# Installation rules
install(TARGETS nexoEditor
        RUNTIME DESTINATION bin
)

# Automatically install runtime dependencies (Windows .dll files, Linux .so files, etc.)
install(TARGETS nexoEditor
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" ".*system32.*" [[.*(\\|/)system32(\\|/).*\.dll]]
        POST_EXCLUDE_REGEXES ".*windows.*" ".*Microsoft.*" [[.*(\\|/)system32(\\|/).*\.dll]]
        RUNTIME DESTINATION bin
        PRIVATE_HEADER DESTINATION include COMPONENT headers
        PUBLIC_HEADER DESTINATION include COMPONENT headers
)

# Install resources, config, and logs directories
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/" DESTINATION resources)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/config/" DESTINATION config)
install(DIRECTORY DESTINATION logs)

# Install licenses
install(DIRECTORY "${CMAKE_SOURCE_DIR}/external/licenses/"
        DESTINATION "external/licenses/"
)
install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_SOURCE_DIR}/COPYRIGHT"
        DESTINATION "."
)

# Component documenation
install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/"
        DESTINATION docs
        COMPONENT documentation
)

# Component headers
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include" # source directory
        COMPONENT headers
        DESTINATION "headers/" # target directory
        FILES_MATCHING # install only matched files
            PATTERN "*.hpp" # select header files
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src" # source directory
        COMPONENT headers
        DESTINATION "headers/" # target directory
        FILES_MATCHING # install only matched files
            PATTERN "*.hpp" # select header files
)

# Generate license files
file(GLOB LICENSE_DIRS "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/*")
set(LICENSE_LIST "" CACHE INTERNAL "")
foreach(LICENSE_DIR ${LICENSE_DIRS})
    if (EXISTS ${LICENSE_DIR}/copyright)
        cmake_path(GET LICENSE_DIR FILENAME LICENSE_LIB_NAME)
        if ("${LICENSE_LIB_NAME}" MATCHES "boost-*")
            set(LICENSE_LIB_NAME "boost")
        endif()
        list(FIND LICENSE_LIST ${LICENSE_LIB_NAME} LICENSE_LIB_NAME_FOUND)
        if (NOT LICENSE_LIB_NAME_FOUND EQUAL -1)
            continue()
        endif()
        install(FILES ${LICENSE_DIR}/copyright DESTINATION "external/licenses" RENAME ${LICENSE_LIB_NAME}
                COMPONENT generate-licenses EXCLUDE_FROM_ALL)
        list(APPEND LICENSE_LIST ${LICENSE_LIB_NAME})
    endif()
endforeach()
list(LENGTH LICENSE_LIST LICENSE_LIST_LENGTH)

# Generate copyright file
install(SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copyright.cmake"
        COMPONENT generate-copyright EXCLUDE_FROM_ALL
)


