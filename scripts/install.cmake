#### install.cmake ############################################################
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

# Installation rules
install(TARGETS nexoEditor
        RUNTIME DESTINATION bin
)

# Automatically install runtime dependencies (Windows .dll files, Linux .so files, etc.)
install(TARGETS nexoEditor
        RUNTIME_DEPENDENCIES
        PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" ".*system32.*" [[.*(\\|/)system32(\\|/).*\.dll]]
        POST_EXCLUDE_REGEXES ".*windows.*" ".*Microsoft.*" [[.*(\\|/)system32(\\|/).*\.dll]]
        RUNTIME DESTINATION bin
        PRIVATE_HEADER DESTINATION include COMPONENT headers
        PUBLIC_HEADER DESTINATION include COMPONENT headers
)

# Install resources, config, and logs directories
install(DIRECTORY "${CMAKE_SOURCE_DIR}/resources/" DESTINATION resources)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/config/" DESTINATION config)
install(DIRECTORY DESTINATION logs)

# Component documenation
install(DIRECTORY "${CMAKE_SOURCE_DIR}/docs/"
        DESTINATION docs
        COMPONENT documentation
)

# Component headers
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include" # source directory
        COMPONENT headers
        DESTINATION "headers/" # target directory
        FILES_MATCHING # install only matched files
            PATTERN "*.hpp" # select header files
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src" # source directory
        COMPONENT headers
        DESTINATION "headers/" # target directory
        FILES_MATCHING # install only matched files
            PATTERN "*.hpp" # select header files
)
