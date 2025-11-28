#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        27/11/2025
#  Description: CMakeLists.txt file for the common library.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

# Set project name
project(nexoCommon VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(COMMON_SOURCES
        common/Exception.cpp
        common/math/Vector.cpp
        common/math/Projection.cpp
        common/math/Bounds.cpp
        common/math/Light.cpp
        common/math/Matrix.cpp
        common/Path.cpp
)

# Create the library
add_library(nexoCommon STATIC ${COMMON_SOURCES})

# Add include directories for this target
target_include_directories(nexoCommon PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# loguru for common utilities
find_package(loguru CONFIG REQUIRED)
target_link_libraries(nexoCommon PUBLIC loguru::loguru)

# glm for math
find_package(glm CONFIG REQUIRED)
target_link_libraries(nexoCommon PUBLIC glm::glm)


