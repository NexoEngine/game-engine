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
#  Description: CMakeLists.txt file for the common tests.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

project(commonTests)

set(BASEDIR ${CMAKE_CURRENT_LIST_DIR})

include_directories("./common")

# TODO: make common a library and link it to the tests
set(COMMON_SOURCES
    common/Exception.cpp
    common/math/Matrix.cpp
)

add_executable(common_tests
    ${COMMON_SOURCES}
    ${BASEDIR}/Matrix.test.cpp
)

# Find glm and add its include directories
find_package(glm CONFIG REQUIRED)

# Link gtest and engine (renderer) libraries
target_link_libraries(common_tests GTest::gtest GTest::gtest_main)
