cmake_minimum_required(VERSION 3.14)

project(SystemsLibraries)

set(CMAKE_CXX_STANDARD 17)
set(ROOT "../../..")

include_directories(${ROOT}/include)

add_library(controls STATIC systems/Controls.cpp)
add_library(draw STATIC systems/Draw.cpp)
add_library(physics STATIC systems/Physics.cpp)

add_custom_target(build_controls
    COMMAND ${CMAKE_COMMAND} --build . --target controls
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(build_draw
    COMMAND ${CMAKE_COMMAND} --build . --target draw
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(build_physics
    COMMAND ${CMAKE_COMMAND} --build . --target physics
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
