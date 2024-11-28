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

# Set the categories
set(DEB_CATEGORIES "Development;Graphics;3DGraphics;Building;")

# generate postinst, postrm and nexo-engine.desktop files using templates
configure_file("${CMAKE_SOURCE_DIR}/scripts/linux/postinst.in"
               "${CMAKE_BINARY_DIR}/postinst" @ONLY
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
configure_file("${CMAKE_SOURCE_DIR}/scripts/linux/postrm.in"
               "${CMAKE_BINARY_DIR}/postrm" @ONLY
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
configure_file(${CMAKE_SOURCE_DIR}/scripts/linux/nexo-engine.desktop.in
               ${CMAKE_BINARY_DIR}/nexo-engine.desktop @ONLY
               FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

# Create symlink for the executable
set(DEB_TEMP_GENERATED_SYMLINK_PATH "${CMAKE_BINARY_DIR}/nexoEditor-symlink")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "../share/nexo-engine/bin/nexoEditor" ${DEB_TEMP_GENERATED_SYMLINK_PATH})

# Install the desktop entry
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    ${CMAKE_BINARY_DIR}/postinst
    ${CMAKE_BINARY_DIR}/postrm
)

# DEB package generator specific settings
# Component deb-symlink
install(FILES ${DEB_TEMP_GENERATED_SYMLINK_PATH}
        DESTINATION /usr/bin  RENAME "nexoEditor"
        COMPONENT deb-symlink EXCLUDE_FROM_ALL # EXCLUDE_FROM_ALL is important to avoid installing the symlink in the default component
)
# Component deb-icon
install(FILES "${CMAKE_SOURCE_DIR}/assets/nexo.png"
        DESTINATION /usr/share/icons/hicolor/256x256/apps
        COMPONENT deb-icon EXCLUDE_FROM_ALL
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
# Component deb-desktop-entry
install(FILES ${CMAKE_BINARY_DIR}/nexo-engine.desktop
        DESTINATION /usr/share/applications
        COMPONENT deb-desktop-entry EXCLUDE_FROM_ALL
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)
