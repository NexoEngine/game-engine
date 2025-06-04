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

#include "Entity.hpp"
#include "ManagedTypedef.hpp"
#include "components/Transform.hpp"

namespace nexo::scripting {

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
        };

        NEXO_RET(void) HelloFromNative(void);
        NEXO_RET(Int32) AddNumbers(Int32 a, Int32 b);
        NEXO_RET(const char*) GetNativeMessage(void);
        NEXO_RET(void) NxLog(UInt32 level, const char *message);

        NEXO_RET(ecs::Entity) CreateCube(Vector3 pos, Vector3 size, Vector3 rotation, Vector4 color);
        NEXO_RET(components::TransformComponent *) GetTransformComponent(ecs::Entity entity);
        NEXO_RET(void *) GetComponent(UInt32 typeId, ecs::Entity entity);
        NEXO_RET(ComponentTypeIds) GetComponentTypeIds();
        NEXO_RET(void) AddComponent(UInt32 typeId, ecs::Entity entity);

    }

    struct NativeApiCallbacks {
        ApiCallback<void(void)> HelloFromNative{&scripting::HelloFromNative};
        ApiCallback<Int32(Int32, Int32)> AddNumbers{&scripting::AddNumbers};
        ApiCallback<const char*()> GetNativeMessage{&scripting::GetNativeMessage};
        ApiCallback<void(UInt32, const char*)> NxLog{&scripting::NxLog};

        ApiCallback<UInt32(Vector3, Vector3, Vector3, Vector4)> CreateCube{&scripting::CreateCube};
        ApiCallback<components::TransformComponent*(ecs::Entity)> GetTransformComponent{&scripting::GetTransformComponent};
        ApiCallback<void*(UInt32, ecs::Entity)> GetComponent{&scripting::GetComponent};
        ApiCallback<ComponentTypeIds()> GetComponentTypeIds{&scripting::GetComponentTypeIds};
        ApiCallback<void(UInt32, ecs::Entity)> AddComponent{&scripting::AddComponent};

    };

    inline NativeApiCallbacks nativeApiCallbacks;

} // namespace nexo::scripting
