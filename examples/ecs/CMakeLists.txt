cmake_minimum_required(VERSION 3.17)

# Set project name
project(ecsExample)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRCS
        examples/ecs/exampleBasic.cpp
)

add_executable(ecsExample ${SRCS})

set_target_properties(ecsExample
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/"
)

include_directories("./engine/src")
include_directories("./common")

#if(WIN32)
#    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/include")
#    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-windows/bin")
#    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows/include")
#elseif(UNIX AND NOT APPLE)
#    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-linux/include")
#elseif(APPLE)
#    include_directories("${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-osx/include")
#endif()

if(APPLE)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
endif()

include_directories(include)
target_link_libraries(ecsExample PRIVATE nexoRenderer)

# Set the output directory for the executable (prevents generator from creating Debug/Release folders)
set_target_properties(ecsExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<0:>)

