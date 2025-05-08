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
    #else
        #define NEXO_API __attribute__((visibility("default")))
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
#else
    #define NEXO_CALL __cdecl
#endif

#define NEXO_RET(type) NEXO_API type NEXO_CALL

#include <cstdint>
#include <type_traits>

#include "ManagedTypedef.hpp"

namespace nexo::scripting {

    template<typename Sig>
    struct ApiCallback;

    template<typename Ret, typename... Args>
    struct ApiCallback<Ret(Args...)> {
        using Type = Ret(NEXO_CALL *)(Args...);

        // Constructor explicitly accepting function pointers
        explicit ApiCallback(Type f) : func(f) {}

        // Delete the default constructor to enforce initialization
        ApiCallback() = delete;

        private:
            Type func;
    };

    extern "C" {

        NEXO_RET(void) HelloFromNative(void);
        NEXO_RET(Int32) AddNumbers(Int32 a, Int32 b);
        NEXO_RET(const char*) GetNativeMessage(void);
        NEXO_RET(void) NxLog(UInt32 level, const char *message);

        NEXO_RET(void) CreateCube(void);



    }

    struct NativeApiCallbacks {
        ApiCallback<void(void)> HelloFromNative{&scripting::HelloFromNative};
        ApiCallback<Int32(Int32, Int32)> AddNumbers{&scripting::AddNumbers};
        ApiCallback<const char*()> GetNativeMessage{&scripting::GetNativeMessage};
        ApiCallback<void(UInt32, const char*)> NxLog{&scripting::NxLog};

        ApiCallback<void()> CreateCube{&scripting::CreateCube};
    };

    inline NativeApiCallbacks nativeApiCallbacks;

} // namespace nexo::scripting
