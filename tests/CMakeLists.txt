#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        21/11/2024
#  Description: CMakeLists.txt file for the tests.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

# Set project name
project(nexoTests VERSION 1.0.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
   
enable_testing()
include(GoogleTest)
# Find gtest package
find_package(GTest CONFIG REQUIRED)

set(TEST_MAIN_FILES
    ${CMAKE_CURRENT_LIST_DIR}/test_main.cpp
)

include(${CMAKE_CURRENT_LIST_DIR}/engine/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/common/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/renderer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/ecs/CMakeLists.txt)

# Add tests
gtest_add_tests(TARGET engine_tests
                TEST_LIST engineTestsList
                
)
gtest_add_tests(TARGET common_tests
                TEST_LIST commonTestsList
)
gtest_add_tests(TARGET renderer_tests
        TEST_LIST rendererTestsList
)
gtest_add_tests(TARGET ecs_tests
        TEST_LIST ecsTestsList
)

# Core engine tests
set_tests_properties(${engineTestsList} PROPERTIES LABELS "engine")
# Common tests
set_tests_properties(${commonTestsList} PROPERTIES LABELS "common")
# Renderer tests
set_tests_properties(${rendererTestsList} PROPERTIES LABELS "renderer")
# Ecs tests
set_tests_properties(${ecsTestsList} PROPERTIES LABELS "ecs")

# Exclude tests from the "ALL" target
message(STATUS "NEXO_BUILD_TESTS: ${NEXO_BUILD_TESTS}")
if(NOT NEXO_BUILD_TESTS)
    message(STATUS "Excluding tests from the 'ALL' target")
    set_target_properties(engine_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(common_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(renderer_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
    set_target_properties(ecs_tests PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message(STATUS "Including tests in the 'ALL' target")
endif()
