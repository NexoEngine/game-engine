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
    #define NEXO_CALL __attribute__((cdecl))
#else
    #define NEXO_CALL __cdecl
#endif

#define NEXO_RET(type) NEXO_API type NEXO_CALL

#include "Entity.hpp"
#include "ManagedTypedef.hpp"
#include "components/Transform.hpp"

namespace nexo::scripting {
    struct Field;

    template<typename Sig>
    struct ApiCallback;

    template<typename Ret, typename... Args>
    struct ApiCallback<Ret(Args...)> {
        using Type = Ret (NEXO_CALL *)(Args...);

        // Default constructor
        ApiCallback() : func(nullptr) {}

        // Constructor explicitly accepting function pointers
        explicit ApiCallback(Type f) : func(f) {}

        // Assignment operator for initialization
        ApiCallback& operator=(Type f) {
            func = f;
            return *this;
        }

        explicit ApiCallback(nullptr_t) = delete;

        private:
            Type func;
    };

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

    // Forward declare the extern "C" functions within the namespace
    extern "C" {
        NEXO_RET(void) NxHelloFromNative(void);
        NEXO_RET(Int32) NxAddNumbers(Int32 a, Int32 b);
        NEXO_RET(const char*) NxGetNativeMessage(void);
        NEXO_RET(void) NxLog(UInt32 level, const char *message);

        NEXO_RET(ecs::Entity) NxCreateCube(Vector3 pos, Vector3 size, Vector3 rotation, Vector4 color);
        NEXO_RET(components::TransformComponent *) NxGetTransformComponent(ecs::Entity entity);
        NEXO_RET(void *) NxGetComponent(ecs::Entity entity, UInt32 componentTypeId);
        NEXO_RET(void) NxAddComponent(ecs::Entity entity, UInt32 typeId, const void *componentData);
        NEXO_RET(bool) NxHasComponent(ecs::Entity entity, UInt32 typeId);
        NEXO_RET(Int64) NxRegisterComponent(const char *name, UInt64 componentSize, const Field *fields, UInt64 fieldCount);
        NEXO_RET(ComponentTypeIds) NxGetComponentTypeIds();

        NEXO_RET(bool) NxIsKeyPressed(Int32 keycode);
        NEXO_RET(bool) NxIsKeyReleased(Int32 keycode);
        NEXO_RET(bool) NxIsAnyKeyPressed(void);
        NEXO_RET(bool) NxIsMouseDown(Int32 button);
        NEXO_RET(bool) NxIsMouseReleased(Int32 button);
        NEXO_RET(void) NxGetMousePosition(Vector2 *position);
    }

    struct NativeApiCallbacks {
        ApiCallback<void(void)> NxHelloFromNative;
        ApiCallback<Int32(Int32, Int32)> NxAddNumbers;
        ApiCallback<const char*()> NxGetNativeMessage;
        ApiCallback<void(UInt32, const char*)> NxLog;

        ApiCallback<UInt32(Vector3, Vector3, Vector3, Vector4)> NxCreateCube;
        ApiCallback<components::TransformComponent*(ecs::Entity)> NxGetTransformComponent;
        ApiCallback<void*(ecs::Entity, UInt32)> NxGetComponent;
        ApiCallback<void(ecs::Entity, UInt32, const void *componentData)> NxAddComponent;
        ApiCallback<bool(ecs::Entity, UInt32)> NxHasComponent;
        ApiCallback<Int64(const char*, UInt64, const Field *, UInt64)> NxRegisterComponent;
        ApiCallback<ComponentTypeIds()> NxGetComponentTypeIds;
        ApiCallback<bool(Int32)> NxIsKeyPressed;
        ApiCallback<bool(Int32)> NxIsKeyReleased;
        ApiCallback<bool()> NxIsAnyKeyPressed;
        ApiCallback<bool(Int32)> NxIsMouseDown;
        ApiCallback<bool(Int32)> NxIsMouseReleased;
        ApiCallback<void(Vector2*)> NxGetMousePosition;
    };

    extern NativeApiCallbacks nativeApiCallbacks;

    void initializeNativeApiCallbacks();

} // namespace nexo::scripting
