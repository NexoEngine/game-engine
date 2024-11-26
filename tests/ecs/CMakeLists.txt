#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Mehdy MORVAN
#  Date:        26/11/2024
#  Description: CMakeLists.txt file for the ecs tests.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

project(ecsTests)

set(BASEDIR ${CMAKE_CURRENT_LIST_DIR})

include_directories("./common")
include_directories("./engine/src/ecs")

# TODO: make common a library and link it to the tests
set(COMMON_SOURCES
        common/Exception.cpp
)

# TODO: Make an ecs library
set(ECS_SOURCES
        engine/src/ecs/Components.cpp
        engine/src/ecs/Coordinator.cpp
        engine/src/ecs/Entity.cpp
        engine/src/ecs/System.cpp
)

add_executable(ecs_tests
        ${COMMON_SOURCES}
        ${ECS_SOURCES}
        ${BASEDIR}/Components.test.cpp
)

# Find glm and add its include directories
find_package(glm CONFIG REQUIRED)
target_include_directories(ecs_tests PUBLIC ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include)

# boost-dll
find_package(Boost CONFIG REQUIRED COMPONENTS dll)
target_link_libraries(ecs_tests PRIVATE Boost::dll)

# Link gtest and engine (renderer) libraries
target_link_libraries(ecs_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock)
