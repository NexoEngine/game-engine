//// CrashTracker.hpp //////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: Sentry crash tracking singleton for NexoEngine
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <string>
#include <mutex>
#include <functional>
#include <atomic>

#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
#include <sentry.h>
#endif

namespace nexo::crash {

    class CrashTracker {
    public:
        ~CrashTracker();

        CrashTracker(const CrashTracker&) = delete;
        CrashTracker& operator=(const CrashTracker&) = delete;

        static std::shared_ptr<CrashTracker> getInstance();

        void initialize(const std::string& dsn = "");

        void shutdown();

        void addBreadcrumb(const std::string& message, const std::string& category = "default", const std::string& level = "info");

        void setTag(const std::string& key, const std::string& value);

        void setContext(const std::string& key, const std::string& value);

        void setUser(const std::string& id);

        void captureMessage(const std::string& message, const std::string& level = "info");

        void captureException(const std::string& type, const std::string& message, const char* file, unsigned int line, const char* function);

        void captureSignal(int signal);

        void addScrubCallback(std::function<std::string(const std::string&)> callback);

        bool hasUserConsent();

        void setUserConsent(bool crashReporting, bool performanceMonitoring = false);

        [[nodiscard]] bool isInitialized() const { return m_initialized; }

        [[nodiscard]] bool isEnabled() const { return m_enabled; }

    private:
        CrashTracker() = default;

        void collectSystemInfo();

        void setupCrashHandlers();

        void setupTerminateHandler();

        std::string scrubPII(const std::string& text);

        std::string anonymizePath(const std::string& path);

        std::string generateAnonymousUserId();

        std::string loadOrCreateUserId();

        bool readUserConsent();

        void saveUserConsent(bool consent);

        static void signalHandlerCallback(int signal);

        static inline std::shared_ptr<CrashTracker> s_instance = nullptr;
        static inline std::mutex s_mutex;

        bool m_initialized = false;
        bool m_enabled = false;
        std::atomic<bool> m_userConsent{false};
        std::string m_userId;
        std::vector<std::function<std::string(const std::string&)>> m_scrubCallbacks;
        std::mutex m_callbackMutex;

        static inline std::terminate_handler s_previousTerminateHandler = nullptr;
    };

}

#define SENTRY_CAPTURE_EXCEPTION(ExceptionType, ...) \
    do { \
        if (auto tracker = nexo::crash::CrashTracker::getInstance(); tracker->isInitialized()) { \
            try { \
                THROW_EXCEPTION(ExceptionType, __VA_ARGS__); \
            } catch (const nexo::Exception& e) { \
                tracker->captureException(#ExceptionType, e.getMessage(), e.getFile(), e.getLine(), e.getFunction()); \
                throw; \
            } \
        } else { \
            THROW_EXCEPTION(ExceptionType, __VA_ARGS__); \
        } \
    } while(0)
