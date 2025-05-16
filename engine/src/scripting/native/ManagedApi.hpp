//// ManagedApi.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        06/05/2025
//  Description: Header file for managed API functions exposed to native code
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <coreclr_delegates.h>

#include "Entity.hpp"
#include "Exception.hpp"
#include "ManagedTypedef.hpp"
#include "components/Transform.hpp"

namespace nexo::scripting {

    class InvalidManagedApi final : public Exception {
        public:
            explicit InvalidManagedApi(
                std::string_view message,
                const std::source_location loc = std::source_location::current()
            ) : Exception(std::format("Invalid managed API call: {}", message), loc) {};
    };

    template<typename Sig>
    struct ManagedApiFn;

    template<typename Ret, typename... Args>
    struct ManagedApiFn<Ret(Args...)> {
        using Type = Ret (CORECLR_DELEGATE_CALLTYPE *)(Args...);

        // Constructor explicitly accepting function pointers
        explicit(false) ManagedApiFn(Type f) : func(f)
        {
            if (!func) {
                THROW_EXCEPTION(InvalidManagedApi, std::format("Function pointer is null: {}", typeid(Type).name()));
            }
        }

        Ret operator()(Args... args) const
        {
            assert(func != nullptr && "Called function pointer is null");
            return func(args...);
        }

        explicit ManagedApiFn(nullptr_t) = delete;

        // Delete the default constructor to enforce initialization
        ManagedApiFn() = default;

        private:
            Type func = nullptr;
    };

    struct lib_args {
        const char_t* message;
        int number;
    };

    struct NativeApiCallbacks;

    /**
     * @brief ManagedApi struct to hold function pointers to managed API functions
     *
     * This struct is used to hold function pointers to managed API functions that can be called from native code.
     *
     * @warning At runtime, all functions MUST be initialized to a none nullptr value.
     *          Otherwise, an exception will be thrown by \c HostHandler::checkManagedApi().
     */
    struct ManagedApi {

        struct NativeInteropApi {
            ManagedApiFn<Int32(NativeApiCallbacks *callbacks, Int32 callbackSize)> Initialize;

            ManagedApiFn<void(void)> DemonstrateNativeCalls;

            ManagedApiFn<void(Double deltaTime)> Update;
        } NativeInterop;

        struct LibApi {
            /**
             * @brief Example call from C++ to C#
             *
             * @param args Arguments passed to managed code
             */
            ManagedApiFn<void(lib_args args)> CustomEntryPoint;

            ManagedApiFn<void(lib_args args)> CustomEntryPointUnmanagedCallersOnly;

            ManagedApiFn<void(lib_args *arg, UInt32 argLength)> Hello;

            ManagedApiFn<Int32(Int32 a, Int32 b)> Add;
            ManagedApiFn<Int32(Int32 a, Int32 b, Int32 *result)> AddToPtr;
        } Lib;


    };

} // namespace nexo::scripting
