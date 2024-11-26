#### deb_config.cmake #########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        25/11/2024
#  Description: CMake to configure the DEB package for the Nexo Engine.
#
###############################################################################

# Set the executable path
set(DEB_EXECUTABLE_PATH "/usr/share/nexo-engine/bin/nexoEditor")
set(DEB_ICON_PATH "/usr/share/nexo-engine/assets/nexo.png")

# Set the desktop entry name (e.g. nexo-engine for nexo-engine.desktop)
set(DEB_DESKTOP_ENTRY_NAME "nexo-engine")

# Set the categories
set(DEB_CATEGORIES "Game;Development;")

include("${CMAKE_SOURCE_DIR}/scripts/linux/generate_postinst.cmake")
include("${CMAKE_SOURCE_DIR}/scripts/linux/generate_postrm.cmake")

# Install the desktop entry
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/postinst")
