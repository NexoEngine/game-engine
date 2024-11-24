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
set(PROJECT_NAME "NEXO Engine")

# CPack specific variables
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "NEXO")
set(CPACK_PACKAGE_CONTACT "nexo.engine@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "NEXO Engine - A simple C++ OpenGL game engine featuring a scene editor, console logging, and asset importing.")
set(CPACK_PACKAGE_VERSION "${NEXO_VERSION_MAJOR}.${NEXO_VERSION_MINOR}.${NEXO_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${NEXO_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${NEXO_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${NEXO_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")

# Icon setup
# Install Icon
set (CPACK_NSIS_MUI_ICON
     "${CMAKE_SOURCE_DIR}/assets/nexo.ico")
# Uninstall Icon
set (CPACK_NSIS_MUI_UNIICON
     "${CMAKE_SOURCE_DIR}/assets/nexo.ico")
# Start Menu Icon
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/nexoEditor.exe")
# Icon displayed in installer
# See https://stackoverflow.com/questions/28768417/how-to-set-an-icon-in-nsis-install-cmake
# On linux to convert image to the right bmp format:
#  convert logo.png -background white -flatten BMP3:logo_nexo.bmp
set (CPACK_PACKAGE_ICON
     "${CMAKE_SOURCE_DIR}/assets/textures\\\\logo_nexo.bmp")

# Adds shortcuts to the Start Menu
# It will find exe in any subdirectory of the install directory
set(CPACK_PACKAGE_EXECUTABLES "nexoEditor" "NEXO Engine")

# This must always be after all CPACK\_\* variables are defined
include(CPack)
