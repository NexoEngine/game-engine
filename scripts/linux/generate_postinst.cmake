#### generate_postinst.cmake ##################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        24/11/2024
#  Description: CMake to generate postinst for DEB package.
#               This postinst creates a desktop entry for the Nexo Engine.
#
###############################################################################

# Write the desktop entry to a file
file(WRITE "${CMAKE_BINARY_DIR}/postinst"
"#!/bin/sh
set -e

if [ ! -s /usr/share/applications/${DESKTOP_ENTRY_NAME}.desktop ]; then
    cat >/usr/share/applications/${DESKTOP_ENTRY_NAME}.desktop<<-END
[Desktop Entry]
Name=${CPACK_PACKAGE_NAME}
Comment=${CPACK_PACKAGE_DESCRIPTION_SUMMARY}
Exec=${DEB_EXECUTABLE_PATH}
Icon=assets/nexo.ico
Terminal=false
Type=Application
Categories=${DEB_CATEGORIES}
END
")

if (UNIX)
    # Set the permissions
    file(CHMOD "${CMAKE_BINARY_DIR}/postinst" OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE)
endif (UNIX)
