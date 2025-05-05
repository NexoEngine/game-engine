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

namespace nexo::scripting {

    extern "C" {
        NEXO_RET(void) HelloFromNative();
        NEXO_RET(int) AddNumbers(int a, int b);
        NEXO_RET(const char*) GetNativeMessage();
    }

} // namespace nexo::scripting
