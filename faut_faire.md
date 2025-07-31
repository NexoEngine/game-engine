# Tests Failures - Engine Issues to Fix

## Failing Tests Due to Engine Implementation Problems

- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.LightContextDefaultConstruction` - LightContext constructor not initializing ambientLight to zero
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.LightContextAddLights` - LightContext light management functions not properly implemented
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.LightContextMaxLimits` - LightContext max light limits not enforced
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.AmbientLightCopySemantics` - AmbientLightComponent copy operations have uninitialized memory
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.DirectionalLightCopySemantics` - DirectionalLightComponent copy operations have uninitialized memory
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.HDRLightingValues` - HDR lighting value handling not implemented
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.ZeroIntensityLights` - Zero intensity light handling incorrect
- `/home/jean/Epitech/game-engine/tests/engine/components/Light.test.cpp.ExtremeAttenuationValues` - Extreme attenuation value validation missing

## Disabled Tests (API Mismatches)

- `/home/jean/Epitech/game-engine/tests/engine/factories/EntityFactory3D.test.cpp` - Missing nexo::math::decomposeTransformQuat function in engine
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetLocation.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetCatalog.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetName.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetRef.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetImporterContext.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/AssetImporter.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/assets/Assets/Model/ModelImporter.test.cpp` - Asset system API evolution requires engine updates
- `/home/jean/Epitech/game-engine/tests/engine/physics/PhysicsSystem.test.cpp` - Complex physics dependencies require Jolt Physics integration updates
- `/home/jean/Epitech/game-engine/tests/engine/systems/TransformMatrixSystem.test.cpp` - ECS API changes require system signature updates
- `/home/jean/Epitech/game-engine/tests/engine/systems/TransformHierarchySystem.test.cpp` - ECS API changes require system signature updates
- `/home/jean/Epitech/game-engine/tests/engine/systems/CameraSystem.test.cpp` - ECS API changes require system signature updates
- `/home/jean/Epitech/game-engine/tests/engine/systems/LightSystem.test.cpp` - ECS API changes require system signature updates
- `/home/jean/Epitech/game-engine/tests/engine/integration/Integration.test.cpp` - Missing SceneManager dependencies in engine