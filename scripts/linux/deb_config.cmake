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

# Check if the symlink can be created
if(WIN32)
    include(${CMAKE_SOURCE_DIR}/scripts/windows/check_dev_mode.cmake)
    is_win_developer_mode_enabled(WINDOWS_DEVELOPER_MODE_ENABLED)
    set(CAN_CREATE_SYMLINK ${WINDOWS_DEVELOPER_MODE_ENABLED})
    message(VERBOSE "Windows Developer Mode enabled: ${WINDOWS_DEVELOPER_MODE_ENABLED}")
else()
    set(CAN_CREATE_SYMLINK ON)
endif()

if(CAN_CREATE_SYMLINK)
    # Create symlink for the executable
    set(DEB_TEMP_GENERATED_SYMLINK_PATH "${CMAKE_BINARY_DIR}/nexoEditor-symlink")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "../share/nexo-engine/bin/nexoEditor" ${DEB_TEMP_GENERATED_SYMLINK_PATH})
else ()
    message(VERBOSE "NEXO Engine symlink couldn't be created. Required for the DEB package generator. See README's troubleshooting section.")
endif()

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
        OPTIONAL # OPTIONAL if symlink couldn't be created
)

# Check if the symlink couldn't be created, display a warning on install time
if(NOT CAN_CREATE_SYMLINK)
    # Create an install code to fatal error if the symlink couldn't be created
    # for component deb-symlink
    install(CODE "message(FATAL_ERROR \"NEXO Engine symlink couldn't be created. Required for the DEB package generator. See README's troubleshooting section.\")"
            COMPONENT deb-symlink EXCLUDE_FROM_ALL
    )
endif()

# Component deb-icon
install(FILES "${CMAKE_SOURCE_DIR}/resources/nexo.png"
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
