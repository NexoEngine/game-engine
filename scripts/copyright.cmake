#### copyright.cmake ##########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        15/04/2025
#  Description: CMake to generate the copyright file for the NEXO project.
#
###############################################################################

cmake_minimum_required(VERSION 3.28)

message(STATUS "Generating copyright file...")

set(UPSTREAM_NAME "NEXO Engine")
set(UPSTREAM_CONTACT "NEXO Engine Team <nexo.engine@gmail.com>")
set(UPSTREAM_SOURCE "https://github.com/NexoEngine/game-engine")

set(COPYRIGHT_LIST "")

# find copyright line with regex
function(extract_copyright_line file outvar)
    file(READ ${file} file_content)
    string(REGEX MATCH "Copyright[^\n\r]*" regex_match ${file_content})
    string(REGEX REPLACE "(Copyright[\\t\\n\\r ]*(\\(c\\))?[\\t\\n\\r ]*)" "" regex_match "${regex_match}")
    set(${outvar} ${regex_match} PARENT_SCOPE)
endfunction()

file(GLOB LICENSE_PATHS "${CMAKE_INSTALL_PREFIX}/external/licenses/*")

foreach(LICENSE_PATH ${LICENSE_PATHS})
    cmake_path(GET LICENSE_PATH FILENAME LICENSE_LIB_NAME)
    message(STATUS "Processing license: ${LICENSE_PATH}")
    extract_copyright_line("${LICENSE_PATH}" COPYRIGHT_LINE)
    if (NOT COPYRIGHT_LINE OR COPYRIGHT_LINE STREQUAL "")
        set(COPYRIGHT_LINE "${LICENSE_LIB_NAME} TODO: copyright line not found")
    endif()
    set(COPYRIGHT_ENTRY_OUTPUT "

Files: *
Copyright: ${COPYRIGHT_LINE}
License: TODO: license to determine
Comment:
 ${LICENSE_LIB_NAME} full license in /external/licenses/${LICENSE_LIB_NAME}")

    set(COPYRIGHT_LIST "${COPYRIGHT_LIST}${COPYRIGHT_ENTRY_OUTPUT}")
endforeach()

string(STRIP "${COPYRIGHT_LIST}" COPYRIGHT_LIST)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/COPYRIGHT.in"
        "${CMAKE_INSTALL_PREFIX}/COPYRIGHT_generated"
        @ONLY
)

message(STATUS "Copyright file generated at ${CMAKE_INSTALL_PREFIX}/COPYRIGHT_generated")
