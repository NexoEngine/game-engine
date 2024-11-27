#### nsis_config.cmake ########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        25/11/2024
#  Description: CMake to configure the NSIS installer for the Nexo Engine.
#
###############################################################################

# Install Icon
set (CPACK_NSIS_MUI_ICON ${NEXO_ICON_PATH})

# Uninstall Icon
set (CPACK_NSIS_MUI_UNIICON ${NEXO_ICON_PATH})

# Start Menu Icon
set(CPACK_NSIS_INSTALLED_ICON_NAME "bin/nexoEditor.exe")

# Icon displayed in installer
# See https://stackoverflow.com/questions/28768417/how-to-set-an-icon-in-nsis-install-cmake
# On linux to convert image to the right bmp format:
#  convert logo.png -background white -flatten BMP3:logo_nexo.bmp
set (CPACK_PACKAGE_ICON
     "${CMAKE_SOURCE_DIR}/assets\\\\nexo_header.bmp")

# Adds shortcuts to the Start Menu
# It will find exe in any subdirectory of the install directory
set(CPACK_PACKAGE_EXECUTABLES "nexoEditor" "NEXO Engine")

set(CPACK_NSIS_MENU_LINKS
    "https://nexoengine.github.io/game-engine/" "NEXO Engine Website"
)
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MODIFY_PATH ON)
