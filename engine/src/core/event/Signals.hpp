//// Signals.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        29/11/2024
//  Description: Header file to define signals utils
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <csignal>
#include <cstring>


namespace nexo::utils {

#ifdef _WIN32
    constexpr const char *strsignal(const int signal)
    {
        switch (signal) {
            case SIGABRT: return "SIGABRT";
            case SIGFPE: return "SIGFPE";
            case SIGILL: return "SIGILL";
            case SIGINT: return "SIGINT";
            case SIGSEGV: return "SIGSEGV";
            case SIGTERM: return "SIGTERM";
            default: return "UNKNOWN";
        }
    }
#else
    inline const char *strsignal(const int signal)
    {
        return ::strsignal(signal);
    }
#endif

}
