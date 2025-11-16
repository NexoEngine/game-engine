#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        16/11/2025
#  Description: CMakeLists.txt file for the common library.
#
###############################################################################

cmake_minimum_required(VERSION 3.28)

project(nexoCommon VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all common source files
set(COMMON_SOURCES
    Exception.cpp
    Path.cpp
    math/Vector.cpp
    math/Matrix.cpp
    math/Projection.cpp
    math/Bounds.cpp
    math/Light.cpp
)

# Create the static library
add_library(nexoCommon STATIC ${COMMON_SOURCES})

# Add C++20 module support for serialization
target_sources(nexoCommon
    PUBLIC
        FILE_SET CXX_MODULES FILES
            Json.cppm
            save/Serializer.cppm
            save/SerializationContext.cppm
            save/SerializationException.cppm
            save/SerializationConcepts.cppm
            save/Serialization.cppm
            save/glm/glm.cppm
)

# Find required dependencies
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Set up include directories
target_include_directories(nexoCommon
    PUBLIC
        ${CMAKE_SOURCE_DIR}/common
        ${CMAKE_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(nexoCommon
    PUBLIC
        glm::glm
        nlohmann_json::nlohmann_json
)

# Export symbols if needed
target_compile_definitions(nexoCommon PUBLIC NEXO_COMMON_EXPORT)

