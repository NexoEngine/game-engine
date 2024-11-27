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
set(DEB_CATEGORIES "Development;Graphics;3DGraphics;Building;")

# generate postinst and postrm files using templates
configure_file("${CMAKE_SOURCE_DIR}/scripts/linux/postinst.in"
               "${CMAKE_BINARY_DIR}/postinst" @ONLY
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE
)
configure_file("${CMAKE_SOURCE_DIR}/scripts/linux/postrm.in"
               "${CMAKE_BINARY_DIR}/postrm" @ONLY
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE
)

# Install the desktop entry
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    ${CMAKE_BINARY_DIR}/postinst
    ${CMAKE_BINARY_DIR}/postrm
)
