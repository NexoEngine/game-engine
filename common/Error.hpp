//// Error.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        26/07/2025
//  Description: Header file for thread-safe error message retrieval
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cerrno>
#include <cstring>
#include <string>

namespace nexo {

    /**
     * @brief Thread-safe wrapper for strerror functionality
     *
     * Provides cross-platform thread-safe error message retrieval
     * for Windows MSVC and Linux systems, handling the GNU/POSIX strerror_r differences.
     */
    class SafeStrerror {
       public:
        /**
         * @brief Get error message for a specific error number (thread-safe strerror)
         * @param errorNumber The error number to get the message for
         * @return String containing the error message
         */
        [[nodiscard]] static std::string getErrorMessage(int errorNumber) noexcept
        {
            try {
                #ifdef _WIN32
                    return getWindowsError(errorNumber);
                #else
                    return getPosixError(errorNumber);
                #endif
            } catch (...) {
                return "Unknown error";
            }
        }

        /**
         * @brief Get error message for the current errno value
         * @return String containing the error message
         */
        [[nodiscard]] static std::string getErrorMessage() noexcept
        {
            return getErrorMessage(errno);
        }

       private:

#ifdef _WIN32
        static std::string getWindowsError(int errorNumber)
        {
            constexpr size_t bufferSize = 256;
            char buffer[bufferSize];

            errno_t result = strerror_s(buffer, bufferSize, errorNumber);
            if (result == 0) {
                return std::string(buffer);
            }

            return "Unknown error";
        }
#else
        static std::string getPosixError(int errorNumber)
        {
            constexpr size_t bufferSize = 256;
            char buffer[bufferSize];

    // The critical fix: handle GNU vs POSIX strerror_r properly
    // We need to detect which version we're using at compile time
    #if defined(_GNU_SOURCE) && defined(__GLIBC__) && !defined(__APPLE__)
            // GNU version: returns char* (may or may not use provided buffer)
            const char* result = strerror_r(errorNumber, buffer, bufferSize);
            return std::string(result); // MUST use return value, not buffer!
    #else
            // POSIX version: returns int, uses provided buffer
            int result = strerror_r(errorNumber, buffer, bufferSize);
            if (result == 0) {
                return std::string(buffer); // Safe to use buffer on success
            }
            return "Unknown error";
    #endif
        }
#endif
    };

    /**
     * @brief Get error message for a specific error number
     * @param errorNumber The error number to get the message for
     * @return String containing the error message
     */
    [[nodiscard]] inline std::string strerror(const int errorNumber) noexcept
    {
        return SafeStrerror::getErrorMessage(errorNumber);
    }

    /**
     * @brief Get error message for the current errno value
     * @return String containing the error message
     *
     * This function is thread-safe and retrieves the error message
     * corresponding to the last error set in the current thread.
     */
    [[nodiscard]] inline std::string strerror() noexcept
    {
        return SafeStrerror::getErrorMessage();
    }

} // namespace nexo
