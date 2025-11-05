//// CrashTracker.cpp //////////////////////////////////////////////////////////
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: Sentry crash tracking implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "CrashTracker.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <csignal>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#else
#include <sys/utsname.h>
#include <unistd.h>
#endif

namespace nexo::crash {

    std::shared_ptr<CrashTracker> CrashTracker::getInstance() {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (!s_instance) {
            s_instance = std::shared_ptr<CrashTracker>(new CrashTracker());
        }
        return s_instance;
    }

    CrashTracker::~CrashTracker() {
        shutdown();
    }

    void CrashTracker::initialize(const std::string& dsn) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (m_initialized) {
            return;
        }

        m_userConsent = readUserConsent();
        if (!m_userConsent) {
            return;
        }

        sentry_options_t* options = sentry_options_new();

#ifdef NEXO_SENTRY_ENABLED
        std::string sentryDsn = dsn;
        if (sentryDsn.empty()) {
            const char* envDsn = std::getenv("SENTRY_DSN");
            if (envDsn) {
                sentryDsn = envDsn;
            } else {
                sentryDsn = "https://d8b6a2e6dba9385c2322bec13c2eed2f@o4508817940873216.ingest.de.sentry.io/4508829070327888";
            }
        }

        if (!sentryDsn.empty()) {
            sentry_options_set_dsn(options, sentryDsn.c_str());
            m_enabled = true;
        }
#endif

#ifdef NEXO_SENTRY_DEBUG_MODE
        std::filesystem::create_directories(".nexo/crash_reports");
        sentry_options_set_database_path(options, ".nexo/crash_reports");
        m_enabled = true;
#endif

#ifdef NEXO_SENTRY_RELEASE
        sentry_options_set_release(options, NEXO_SENTRY_RELEASE);
#endif

        sentry_options_set_environment(options,
#ifdef NDEBUG
            "production"
#else
            "development"
#endif
        );

        sentry_options_set_auto_session_tracking(options, 1);

        if (sentry_init(options) == 0) {
            m_initialized = true;
            m_userId = loadOrCreateUserId();
            setUser(m_userId);
            collectSystemInfo();
            setupCrashHandlers();
            setupTerminateHandler();
        }
#endif
    }

    void CrashTracker::shutdown() {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (m_initialized) {
            if (s_previousTerminateHandler) {
                std::set_terminate(s_previousTerminateHandler);
                s_previousTerminateHandler = nullptr;
            }
            sentry_close();
            m_initialized = false;
            m_enabled = false;
        }
#endif
    }

    void CrashTracker::addBreadcrumb(const std::string& message, const std::string& category, const std::string& level) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;

        sentry_value_t crumb = sentry_value_new_breadcrumb(nullptr, scrubPII(message).c_str());
        sentry_value_set_by_key(crumb, "category", sentry_value_new_string(category.c_str()));
        sentry_value_set_by_key(crumb, "level", sentry_value_new_string(level.c_str()));
        sentry_add_breadcrumb(crumb);
#endif
    }

    void CrashTracker::setTag(const std::string& key, const std::string& value) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;
        sentry_set_tag(key.c_str(), scrubPII(value).c_str());
#endif
    }

    void CrashTracker::setContext(const std::string& key, const std::string& value) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;
        sentry_value_t context = sentry_value_new_object();
        sentry_value_set_by_key(context, "value", sentry_value_new_string(scrubPII(value).c_str()));
        sentry_set_context(key.c_str(), context);
#endif
    }

    void CrashTracker::setUser(const std::string& id) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;
        sentry_value_t user = sentry_value_new_object();
        sentry_value_set_by_key(user, "id", sentry_value_new_string(id.c_str()));
        sentry_set_user(user);
#endif
    }

    void CrashTracker::captureMessage(const std::string& message, const std::string& level) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;

        sentry_level_t sentryLevel = SENTRY_LEVEL_INFO;
        if (level == "debug") sentryLevel = SENTRY_LEVEL_DEBUG;
        else if (level == "warning") sentryLevel = SENTRY_LEVEL_WARNING;
        else if (level == "error") sentryLevel = SENTRY_LEVEL_ERROR;
        else if (level == "fatal") sentryLevel = SENTRY_LEVEL_FATAL;

        sentry_capture_event(sentry_value_new_message_event(sentryLevel, nullptr, scrubPII(message).c_str()));
#endif
    }

    void CrashTracker::captureException(const std::string& type, const std::string& message,
                                        const char* file, unsigned int line, const char* function) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;

        sentry_value_t event = sentry_value_new_event();
        sentry_value_t exc = sentry_value_new_exception(type.c_str(), scrubPII(message).c_str());

        sentry_value_t stacktrace = sentry_value_new_object();
        sentry_value_t frames = sentry_value_new_list();

        sentry_value_t frame = sentry_value_new_object();
        sentry_value_set_by_key(frame, "filename", sentry_value_new_string(anonymizePath(file).c_str()));
        sentry_value_set_by_key(frame, "function", sentry_value_new_string(function));
        sentry_value_set_by_key(frame, "lineno", sentry_value_new_int32(line));

        sentry_value_append(frames, frame);
        sentry_value_set_by_key(stacktrace, "frames", frames);
        sentry_value_set_by_key(exc, "stacktrace", stacktrace);

        sentry_value_t exceptions = sentry_value_new_list();
        sentry_value_append(exceptions, exc);
        sentry_value_set_by_key(event, "exception", exceptions);

        sentry_capture_event(event);
#endif
    }

    void CrashTracker::captureSignal(int signal) {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        if (!m_initialized) return;

        std::string signalName;
        switch (signal) {
            case SIGSEGV: signalName = "SIGSEGV"; break;
            case SIGTERM: signalName = "SIGTERM"; break;
            case SIGINT: signalName = "SIGINT"; break;
            default: signalName = "SIGNAL_" + std::to_string(signal); break;
        }

        addBreadcrumb("Signal received: " + signalName, "signal", "fatal");
        captureMessage("Received signal: " + signalName, "fatal");
#endif
    }

    void CrashTracker::addScrubCallback(std::function<std::string(const std::string&)> callback) {
        std::lock_guard<std::mutex> lock(m_callbackMutex);
        m_scrubCallbacks.push_back(callback);
    }

    bool CrashTracker::hasUserConsent() {
        return m_userConsent.load();
    }

    void CrashTracker::setUserConsent(bool consent) {
        m_userConsent = consent;
        saveUserConsent(consent);

        if (consent && !m_initialized) {
            initialize();
        } else if (!consent && m_initialized) {
            shutdown();
        }
    }

    void CrashTracker::collectSystemInfo() {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        setTag("platform",
#ifdef _WIN32
            "windows"
#elif defined(__APPLE__)
            "macos"
#else
            "linux"
#endif
        );

        setTag("graphics_api", "opengl");
        setTag("build_type",
#ifdef NDEBUG
            "release"
#else
            "debug"
#endif
        );

#ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        setContext("cpu_architecture", std::to_string(sysInfo.wProcessorArchitecture));

        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        setContext("total_memory", std::to_string(memInfo.ullTotalPhys / (1024 * 1024)) + " MB");
#else
        struct utsname unameData;
        if (uname(&unameData) == 0) {
            setContext("os_version", std::string(unameData.sysname) + " " + unameData.release);
            setContext("cpu_architecture", unameData.machine);
        }

        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        if (pages > 0 && page_size > 0) {
            setContext("total_memory", std::to_string((pages * page_size) / (1024 * 1024)) + " MB");
        }
#endif
#endif
    }

    void CrashTracker::setupCrashHandlers() {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        std::signal(SIGSEGV, signalHandlerCallback);
#ifndef _WIN32
        std::signal(SIGBUS, signalHandlerCallback);
        std::signal(SIGILL, signalHandlerCallback);
        std::signal(SIGFPE, signalHandlerCallback);
        std::signal(SIGABRT, signalHandlerCallback);
#endif
#endif
    }

    void CrashTracker::setupTerminateHandler() {
#if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        s_previousTerminateHandler = std::set_terminate([]() {
            if (auto tracker = getInstance(); tracker->isInitialized()) {
                try {
                    auto exc = std::current_exception();
                    if (exc) {
                        std::rethrow_exception(exc);
                    }
                } catch (const std::exception& e) {
                    tracker->captureMessage(std::string("Uncaught exception: ") + e.what(), "fatal");
                } catch (...) {
                    tracker->captureMessage("Uncaught unknown exception", "fatal");
                }
            }
            if (s_previousTerminateHandler) {
                s_previousTerminateHandler();
            } else {
                std::abort();
            }
        });
#endif
    }

    void CrashTracker::signalHandlerCallback(int signal) {
        if (auto tracker = getInstance(); tracker->isInitialized()) {
            tracker->captureSignal(signal);
        }
        std::signal(signal, SIG_DFL);
        std::raise(signal);
    }

    std::string CrashTracker::scrubPII(const std::string& text) {
        std::string result = text;

        std::lock_guard<std::mutex> lock(m_callbackMutex);
        for (const auto& callback : m_scrubCallbacks) {
            result = callback(result);
        }

        static const std::regex emailRegex(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b)");
        result = std::regex_replace(result, emailRegex, "[EMAIL_REDACTED]");

        static const std::regex tokenRegex(R"(\b[A-Za-z0-9_-]{20,}\b)");
        result = std::regex_replace(result, tokenRegex, "[TOKEN_REDACTED]");

        return result;
    }

    std::string CrashTracker::anonymizePath(const std::string& path) {
        std::string result = path;

#ifdef _WIN32
        const char* userProfile = std::getenv("USERPROFILE");
        if (userProfile) {
            size_t pos = result.find(userProfile);
            if (pos != std::string::npos) {
                result.replace(pos, std::strlen(userProfile), "[USER_HOME]");
            }
        }
#else
        const char* home = std::getenv("HOME");
        if (home) {
            size_t pos = result.find(home);
            if (pos != std::string::npos) {
                result.replace(pos, std::strlen(home), "[USER_HOME]");
            }
        }
#endif

        return result;
    }

    std::string CrashTracker::generateAnonymousUserId() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return boost::uuids::to_string(uuid);
    }

    std::string CrashTracker::loadOrCreateUserId() {
        std::filesystem::path configPath;

#ifdef _WIN32
        const char* appData = std::getenv("APPDATA");
        if (appData) {
            configPath = std::filesystem::path(appData) / "NexoEngine" / "nexo_config.ini";
        }
#else
        const char* home = std::getenv("HOME");
        if (home) {
            configPath = std::filesystem::path(home) / ".config" / "NexoEngine" / "nexo_config.ini";
        }
#endif

        if (configPath.empty()) {
            return generateAnonymousUserId();
        }

        std::filesystem::create_directories(configPath.parent_path());

        if (std::filesystem::exists(configPath)) {
            std::ifstream file(configPath);
            std::string line;
            bool inPrivacySection = false;

            while (std::getline(file, line)) {
                if (line == "[Privacy]") {
                    inPrivacySection = true;
                } else if (!line.empty() && line[0] == '[') {
                    inPrivacySection = false;
                } else if (inPrivacySection && line.find("user_id=") == 0) {
                    return line.substr(8);
                }
            }
        }

        std::string newId = generateAnonymousUserId();
        std::ofstream file(configPath, std::ios::app);
        if (file.is_open()) {
            file << "\n[Privacy]\n";
            file << "user_id=" << newId << "\n";
        }
        return newId;
    }

    bool CrashTracker::readUserConsent() {
        std::filesystem::path configPath;

#ifdef _WIN32
        const char* appData = std::getenv("APPDATA");
        if (appData) {
            configPath = std::filesystem::path(appData) / "NexoEngine" / "nexo_config.ini";
        }
#else
        const char* home = std::getenv("HOME");
        if (home) {
            configPath = std::filesystem::path(home) / ".config" / "NexoEngine" / "nexo_config.ini";
        }
#endif

        if (configPath.empty() || !std::filesystem::exists(configPath)) {
            return false;
        }

        std::ifstream file(configPath);
        std::string line;
        bool inPrivacySection = false;

        while (std::getline(file, line)) {
            if (line == "[Privacy]") {
                inPrivacySection = true;
            } else if (!line.empty() && line[0] == '[') {
                inPrivacySection = false;
            } else if (inPrivacySection && line.find("crash_tracking_consent=") == 0) {
                std::string value = line.substr(23);
                return (value == "true" || value == "1" || value == "yes");
            }
        }

        return false;
    }

    void CrashTracker::saveUserConsent(bool consent) {
        std::filesystem::path configPath;

#ifdef _WIN32
        const char* appData = std::getenv("APPDATA");
        if (appData) {
            configPath = std::filesystem::path(appData) / "NexoEngine" / "nexo_config.ini";
        }
#else
        const char* home = std::getenv("HOME");
        if (home) {
            configPath = std::filesystem::path(home) / ".config" / "NexoEngine" / "nexo_config.ini";
        }
#endif

        if (configPath.empty()) {
            return;
        }

        std::filesystem::create_directories(configPath.parent_path());

        std::vector<std::string> lines;
        bool inPrivacySection = false;
        bool consentUpdated = false;

        if (std::filesystem::exists(configPath)) {
            std::ifstream file(configPath);
            std::string line;
            while (std::getline(file, line)) {
                if (line == "[Privacy]") {
                    inPrivacySection = true;
                    lines.push_back(line);
                } else if (!line.empty() && line[0] == '[') {
                    if (inPrivacySection && !consentUpdated) {
                        lines.push_back("crash_tracking_consent=" + std::string(consent ? "true" : "false"));
                        consentUpdated = true;
                    }
                    inPrivacySection = false;
                    lines.push_back(line);
                } else if (inPrivacySection && line.find("crash_tracking_consent=") == 0) {
                    lines.push_back("crash_tracking_consent=" + std::string(consent ? "true" : "false"));
                    consentUpdated = true;
                } else {
                    lines.push_back(line);
                }
            }
        }

        if (!consentUpdated) {
            if (!inPrivacySection) {
                lines.push_back("[Privacy]");
            }
            lines.push_back("crash_tracking_consent=" + std::string(consent ? "true" : "false"));
        }

        std::ofstream file(configPath);
        for (const auto& line : lines) {
            file << line << "\n";
        }
    }

}
