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

# Link against nexoCommon library
add_executable(common_tests
    ${TEST_MAIN_FILES}
    ${BASEDIR}/Matrix.test.cpp
    ${BASEDIR}/Path.test.cpp
    ${BASEDIR}/Exceptions.test.cpp
    ${BASEDIR}/Vector.test.cpp
    ${BASEDIR}/Light.test.cpp
    ${BASEDIR}/save/Serialization.test.cpp
    ${BASEDIR}/save/SerializationContext.test.cpp
    ${BASEDIR}/save/SerializationFocused.test.cpp
    ${BASEDIR}/save/SerializationIntegration.test.cpp
    ${BASEDIR}/save/VersionedJson.test.cpp
)

# Add precompiled header (if available and enabled)
if (NEXO_USE_PCH)
    target_precompile_headers(common_tests REUSE_FROM nexoCommon)
endif()

######################
# Target compile definitions
######################
target_compile_definitions(common_tests PRIVATE NX_TESTS)
target_compile_definitions(common_tests PRIVATE NX_TESTS_COMMON)


# Find glm and add its include directories
find_package(glm CONFIG REQUIRED)

# boost-dll
find_package(Boost CONFIG REQUIRED COMPONENTS dll)
target_link_libraries(common_tests PRIVATE Boost::dll)

# Link gtest and engine (renderer) libraries
target_link_libraries(common_tests PRIVATE GTest::gtest nexoCommon)
