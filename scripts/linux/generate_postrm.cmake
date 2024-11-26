#### generate_postrm.cmake ####################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        24/11/2024
#  Description: CMake to generate postrm for DEB package.
#               This postrm removes the desktop entry for the Nexo Engine.
#
###############################################################################

cmake_minimum_required(VERSION 3.19)

# Write the desktop entry to a file
file(WRITE "${CMAKE_BINARY_DIR}/postrm"
"#!/bin/sh
set -e

echo \"Removing the desktop entry...\"
if [ -f /usr/share/applications/${DEB_DESKTOP_ENTRY_NAME}.desktop ]; then
    rm /usr/share/applications/${DEB_DESKTOP_ENTRY_NAME}.desktop
fi
")

# Set the permissions
file(CHMOD "${CMAKE_BINARY_DIR}/postrm"
     PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE)
