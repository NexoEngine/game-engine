cmake_minimum_required(VERSION 3.10)

# Set project name
project(nexoApi VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(COMMON_SOURCES
        engine/src/scripting/native/NativeApi.cpp
)

# Create the library
add_library(nexoApi SHARED ${COMMON_SOURCES})

# Add include directories for this target
target_include_directories(nexoApi PUBLIC
                           ${CMAKE_SOURCE_DIR}/engine/include
                           ${CMAKE_SOURCE_DIR}/engine/src
                           ${CMAKE_SOURCE_DIR}/common)

target_link_libraries(nexoApi PRIVATE nexoRenderer)
target_compile_definitions(nexoApi PRIVATE NEXO_EXPORT)
