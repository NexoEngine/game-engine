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
    ${BASEDIR}/event/Event.test.cpp
    ${BASEDIR}/event/EventManager.test.cpp
    ${BASEDIR}/event/WindowEvent.test.cpp
    ${BASEDIR}/exceptions/Exceptions.test.cpp
    ${BASEDIR}/scene/Scene.test.cpp
    ${BASEDIR}/scene/SceneManager.test.cpp
    ${BASEDIR}/scene/SceneSerializer.test.cpp
    ${BASEDIR}/components/Camera.test.cpp
    ${BASEDIR}/components/Transform.test.cpp
    ${BASEDIR}/components/Light.test.cpp
    ${BASEDIR}/components/LightContext.test.cpp
    ${BASEDIR}/components/Name.test.cpp
    ${BASEDIR}/components/Uuid.test.cpp
    ${BASEDIR}/components/Parent.test.cpp
    ${BASEDIR}/components/Render.test.cpp
    ${BASEDIR}/components/SceneTag.test.cpp
    ${BASEDIR}/components/Editor.test.cpp
    ${BASEDIR}/assets/AssetLocation.test.cpp
    ${BASEDIR}/assets/AssetCatalog.test.cpp
    ${BASEDIR}/assets/AssetName.test.cpp
    ${BASEDIR}/assets/AssetRef.test.cpp
    ${BASEDIR}/assets/AssetImporterContext.test.cpp
    ${BASEDIR}/assets/AssetImporter.test.cpp
    ${BASEDIR}/assets/AssetImporterInput.test.cpp
    ${BASEDIR}/assets/AssetPackName.test.cpp
    ${BASEDIR}/assets/Assets/Model/ModelImporter.test.cpp
    ${BASEDIR}/assets/FilenameValidator.test.cpp
    ${BASEDIR}/assets/AssetType.test.cpp
    ${BASEDIR}/assets/TextureParameters.test.cpp
    ${BASEDIR}/assets/ModelParameters.test.cpp
    ${BASEDIR}/assets/ValidatedName.test.cpp
    ${BASEDIR}/components/BillboardComponent.test.cpp
    ${BASEDIR}/renderer/FramebufferSpecs.test.cpp
    ${BASEDIR}/renderer/ShaderMetadata.test.cpp
    ${BASEDIR}/renderer/Buffer.test.cpp
    ${BASEDIR}/renderer/TextureFormat.test.cpp
    ${BASEDIR}/renderer/DrawCommand.test.cpp
    ${BASEDIR}/renderer/RendererExceptions.test.cpp
    ${BASEDIR}/renderer/RendererAPIEnums.test.cpp
    ${BASEDIR}/renderer/TransparentStringHasher.test.cpp
    ${BASEDIR}/renderer/Attributes.test.cpp
    ${BASEDIR}/ecs/ComponentArray.test.cpp
    ${BASEDIR}/ecs/EntityManager.test.cpp
    ${BASEDIR}/ecs/SingletonComponent.test.cpp
    ${BASEDIR}/ecs/Coordinator.test.cpp
    ${BASEDIR}/ecs/Access.test.cpp
	${BASEDIR}/physics/PhysicsSystem.test.cpp
    ${BASEDIR}/ecs/Definitions.test.cpp
    ${BASEDIR}/ecs/ECSExceptions.test.cpp
    ${BASEDIR}/ecs/TypeErasedComponent/TypeErasedComponent.test.cpp
    ${BASEDIR}/ecs/FieldType.test.cpp
    ${BASEDIR}/ecs/Field.test.cpp
    ${BASEDIR}/renderPasses/Masks.test.cpp
    ${BASEDIR}/renderPasses/Passes.test.cpp
    ${BASEDIR}/Types.test.cpp
    ${BASEDIR}/../crash/CrashTracker.test.cpp
    ${BASEDIR}/scripting/FieldType.test.cpp
    ${BASEDIR}/scripting/Field.test.cpp
    ${BASEDIR}/scripting/ManagedTypedef.test.cpp
    ${BASEDIR}/scripting/HostString.test.cpp
    ${BASEDIR}/components/Material.test.cpp
    ${BASEDIR}/components/RenderContext.test.cpp
    ${BASEDIR}/components/StaticMesh.test.cpp
    ${BASEDIR}/components/MaterialComponent.test.cpp
    ${BASEDIR}/components/Model.test.cpp
    ${BASEDIR}/components/PhysicsBodyComponent.test.cpp
    ${BASEDIR}/core/Signals.test.cpp
    ${BASEDIR}/core/KeyCodes.test.cpp
    ${BASEDIR}/systems/TransformMatrixSystem.test.cpp
    # Disabled due to test fixture issues - needs investigation
    # ${BASEDIR}/systems/TransformHierarchySystem.test.cpp
    # ${BASEDIR}/systems/LightSystems.test.cpp
    ${BASEDIR}/renderer/PixelConversion.test.cpp
    # ${BASEDIR}/EntityFactory3D.test.cpp
    # ${BASEDIR}/Factories.test.cpp
        # Add other engine test files here
)

# Link gtest and engine (renderer) libraries
target_link_libraries(engine_tests PRIVATE GTest::gtest GTest::gmock nexoRenderer)
