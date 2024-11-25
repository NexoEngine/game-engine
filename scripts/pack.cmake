#### pack.cmake ###############################################################
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

project(pack-nexo)

# Set the version number
set(NEXO_VERSION_MAJOR 1)
set(NEXO_VERSION_MINOR 0)
set(NEXO_VERSION_PATCH 0)

# Set the project name
set(PROJECT_NAME "NEXO-Engine")

# CPack specific variables
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "NEXO Engine")
set(CPACK_PACKAGE_CONTACT "nexo.engine@gmail.com")
set(CPACK_STRIP_FILES YES)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "NEXO Engine - A C++ OpenGL game engine usable in virtual reality.")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
    "Features a scene editor, console logging, and asset importing."
)
set(CPACK_PACKAGE_VERSION "${NEXO_VERSION_MAJOR}.${NEXO_VERSION_MINOR}.${NEXO_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${NEXO_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NEXO_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${NEXO_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "NexoEngine")

# Set the icon path
set(NEXO_ICON_PATH "${CMAKE_SOURCE_DIR}/assets/nexo.ico")

include("${CMAKE_SOURCE_DIR}/scripts/linux/deb_config.cmake")
include("${CMAKE_SOURCE_DIR}/scripts/windows/nsis_config.cmake")


# Configure each CPack generator
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_BINARY_DIR}/scripts/generator_config.cmake")
# Replace @VARS@ with CMake variables in the template file
configure_file("${PROJECT_SOURCE_DIR}/scripts/CMakeCPackOptions.cmake.in"
               "${PROJECT_BINARY_DIR}/CMakeCPackOptions.cmake" @ONLY)
# Use the final generated file as config
set(CPACK_PROJECT_CONFIG_FILE "${PROJECT_BINARY_DIR}/CMakeCPackOptions.cmake")

# This must always be after all CPACK\_\* variables are defined
include(CPack)
