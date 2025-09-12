//// NativeApi.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        06/05/2025
//  Description: Header file for native API functions exposed to managed code
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef NEXO_EXPORT
    #ifdef WIN32
        #define NEXO_API __declspec(dllexport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define NEXO_API __attribute__((visibility("default")))
    #else
        #define NEXO_API
    #endif
#else
    #ifdef WIN32
        #define NEXO_API __declspec(dllimport)
    #else
        #define NEXO_API
    #endif
#endif

#ifdef WIN32 // Set calling convention according to .NET https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.dllimportattribute.callingconvention?view=net-9.0#remarks
    #define NEXO_CALL  __stdcall
#elif defined(__GNUC__) || defined(__clang__)
    #define NEXO_CALL // Empty on GCC/Clang since cdecl is default
#else
    #define NEXO_CALL __cdecl
#endif

#define NEXO_RET(type) NEXO_API type NEXO_CALL

#include "ManagedTypedef.hpp"
#include "components/Transform.hpp"

namespace nexo::scripting {
    struct Field;

    template<typename Sig>
    struct ApiCallback;

    template<typename Ret, typename... Args>
    struct ApiCallback<Ret(Args...)> {
        using Type = Ret (NEXO_CALL *)(Args...);

        // Constructor explicitly accepting function pointers
        explicit ApiCallback(Type f) : func(f) {}

        explicit ApiCallback(nullptr_t) = delete;

        // Delete the default constructor to enforce initialization
        ApiCallback() = delete;

        private:
            Type func;
    };

    extern "C" {
        struct ComponentTypeIds {
            UInt32 Transform;
            UInt32 AmbientLight;
            UInt32 DirectionalLight;
            UInt32 PointLight;
            UInt32 SpotLight;
            UInt32 RenderComponent;
            UInt32 SceneTag;
            UInt32 CameraComponent;
            UInt32 UuidComponent;
            UInt32 PerspectiveCameraController;
            UInt32 PerspectiveCameraTarget;
            UInt32 PhysicsBodyComponent;
        };

        NEXO_RET(void) NxHelloFromNative(void);
        NEXO_RET(Int32) NxAddNumbers(Int32 a, Int32 b);
        NEXO_RET(const char*) NxGetNativeMessage(void);
        NEXO_RET(void) NxLog(UInt32 level, const char *message);

        NEXO_RET(ecs::Entity) NxCreateCube(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color);
        NEXO_RET(components::TransformComponent *) NxGetTransformComponent(ecs::Entity entity);
        NEXO_RET(void *) NxGetComponent(ecs::Entity entity, UInt32 componentTypeId);
        NEXO_RET(void) NxAddComponent(ecs::Entity entity, UInt32 componentTypeId, const void *componentData);
        NEXO_RET(void) NxRemoveComponent(ecs::Entity entity, UInt32 componentTypeId);
        NEXO_RET(void) NxDestroyEntity(ecs::Entity entity);
        NEXO_RET(bool) NxHasComponent(ecs::Entity entity, UInt32 componentTypeId);
        NEXO_RET(Int64) NxRegisterComponent(const char *name, UInt64 componentSize, const Field *fields, UInt64 fieldCount);
        NEXO_RET(ComponentTypeIds) NxGetComponentTypeIds();

        NEXO_RET(ecs::Entity) NxCreateTetrahedron(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color);
        NEXO_RET(ecs::Entity) NxCreatePyramid(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color);
        NEXO_RET(ecs::Entity) NxCreateCylinder(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color, UInt32 nbSegment);
        NEXO_RET(ecs::Entity) NxCreateSphere(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color, UInt32 nbSubdivision);

        NEXO_RET(ecs::Entity) NxCreatePointLight(Vector3 position, Vector4 color, float linear, float quadratic);
        
        // Physics functions
        NEXO_RET(void) NxCreateBodyFromShape(ecs::Entity entity, Vector3 position, Vector3 size, Vector3 rotation, UInt32 shapeType, UInt32 motionType);
        NEXO_RET(void) NxApplyForce(ecs::Entity entity, Vector3 force);

    }

    struct NativeApiCallbacks {
        ApiCallback<void(void)> NxHelloFromNative{&scripting::NxHelloFromNative};
        ApiCallback<Int32(Int32, Int32)> NxAddNumbers{&scripting::NxAddNumbers};
        ApiCallback<const char*()> NxGetNativeMessage{&scripting::NxGetNativeMessage};
        ApiCallback<void(UInt32, const char*)> NxLog{&scripting::NxLog};

        ApiCallback<ecs::Entity(Vector3, Vector3, Vector3, Vector4)> NxCreateCube{&scripting::NxCreateCube};
        ApiCallback<ecs::Entity(Vector3, Vector3, Vector3, Vector4)> NxCreateTetrahedron{&scripting::NxCreateTetrahedron};
        ApiCallback<ecs::Entity(Vector3, Vector3, Vector3, Vector4)> NxCreatePyramid{&scripting::NxCreatePyramid};
        ApiCallback<ecs::Entity(Vector3, Vector3, Vector3, Vector4, UInt32)> NxCreateCylinder{&scripting::NxCreateCylinder};
        ApiCallback<ecs::Entity(Vector3, Vector3, Vector3, Vector4, UInt32)> NxCreateSphere{&scripting::NxCreateSphere};

        ApiCallback<ecs::Entity(Vector3, Vector4, float, float)> NxCreatePointLight{&scripting::NxCreatePointLight};
        
        // Physics callbacks
        ApiCallback<void(ecs::Entity, Vector3, Vector3, Vector3, UInt32, UInt32)> NxCreateBodyFromShape{&scripting::NxCreateBodyFromShape};
        ApiCallback<void(ecs::Entity, Vector3)> NxApplyForce{&scripting::NxApplyForce};
        
        ApiCallback<components::TransformComponent*(ecs::Entity)> NxGetTransformComponent{&scripting::NxGetTransformComponent};
        ApiCallback<void*(ecs::Entity, UInt32)> NxGetComponent{&scripting::NxGetComponent};
        ApiCallback<void(ecs::Entity, UInt32, const void *componentData)> NxAddComponent{&scripting::NxAddComponent};
        ApiCallback<void(ecs::Entity, UInt32)> NxRemoveComponent{&scripting::NxRemoveComponent};
        ApiCallback<void(ecs::Entity)> NxDestroyEntity{&scripting::NxDestroyEntity};
        ApiCallback<bool(ecs::Entity, UInt32)> NxHasComponent{&scripting::NxHasComponent};
        ApiCallback<Int64(const char*, UInt64, const Field *, UInt64)> NxRegisterComponent{&scripting::NxRegisterComponent};
        ApiCallback<ComponentTypeIds()> NxGetComponentTypeIds{&scripting::NxGetComponentTypeIds};
    };

    inline NativeApiCallbacks nativeApiCallbacks;

} // namespace nexo::scripting
