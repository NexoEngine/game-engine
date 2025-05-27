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
#  Description: CMakeLists.txt file for the engine tests.
#
###############################################################################

cmake_minimum_required(VERSION 3.10)

project(engineTests)

set(BASEDIR ${CMAKE_CURRENT_LIST_DIR})

include_directories("./engine/src")
include_directories("./editor/external")
include_directories("./common")

# Add engine test source files
add_executable(engine_tests
	${TEST_MAIN_FILES}
    ${BASEDIR}/event/EventManager.test.cpp
    ${BASEDIR}/event/WindowEvent.test.cpp
    ${BASEDIR}/exceptions/Exceptions.test.cpp
    ${BASEDIR}/scene/Scene.test.cpp
    ${BASEDIR}/scene/SceneManager.test.cpp
    ${BASEDIR}/components/Camera.test.cpp
    ${BASEDIR}/assets/AssetLocation.test.cpp
    ${BASEDIR}/assets/AssetCatalog.test.cpp
    ${BASEDIR}/assets/AssetName.test.cpp
    ${BASEDIR}/assets/AssetRef.test.cpp
    ${BASEDIR}/assets/AssetImporterContext.test.cpp
    ${BASEDIR}/assets/AssetImporter.test.cpp
    ${BASEDIR}/assets/Assets/Model/ModelImporter.test.cpp
	${BASEDIR}/physics/PhysicsSystem.test.cpp
        # Add other engine test files here
)

# Link gtest and engine (renderer) libraries
target_link_libraries(engine_tests PRIVATE GTest::gtest GTest::gmock nexoRenderer)
