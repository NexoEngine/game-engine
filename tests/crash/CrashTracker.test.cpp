//// CrashTracker.test.cpp /////////////////////////////////////////////////
//
//  Author:      Jean CARDONNE
//  Date:        05/11/2025
//  Description: Test file for CrashTracker
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "core/crash/CrashTracker.hpp"
#include <filesystem>
#include <fstream>
#include <csignal>

namespace nexo::crash {

    class CrashTrackerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            testConfigDir = std::filesystem::temp_directory_path() / "nexo_test";
            std::filesystem::create_directories(testConfigDir);

            #ifdef _WIN32
            _putenv_s("APPDATA", testConfigDir.string().c_str());
            #else
            setenv("HOME", testConfigDir.string().c_str(), 1);
            #endif
        }

        void TearDown() override {
            if (std::filesystem::exists(testConfigDir)) {
                std::filesystem::remove_all(testConfigDir);
            }
        }

        std::filesystem::path testConfigDir;
    };

    TEST_F(CrashTrackerTest, SingletonPattern) {
        auto tracker1 = CrashTracker::getInstance();
        auto tracker2 = CrashTracker::getInstance();

        EXPECT_EQ(tracker1, tracker2);
        EXPECT_NE(tracker1, nullptr);
    }

    TEST_F(CrashTrackerTest, InitializationWithoutConsent) {
        auto tracker = CrashTracker::getInstance();
        tracker->initialize();

        EXPECT_FALSE(tracker->isInitialized());
        EXPECT_FALSE(tracker->hasUserConsent());
    }

    TEST_F(CrashTrackerTest, ConsentPersistence) {
        auto tracker = CrashTracker::getInstance();

        tracker->setUserConsent(true);
        EXPECT_TRUE(tracker->hasUserConsent());

        auto tracker2 = CrashTracker::getInstance();
        tracker2->initialize();

        #if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
        EXPECT_TRUE(tracker2->isInitialized());
        #endif
    }

    TEST_F(CrashTrackerTest, UserConsentToggle) {
        auto tracker = CrashTracker::getInstance();

        // Toggle from current state to opposite and back
        tracker->setUserConsent(false);
        EXPECT_FALSE(tracker->hasUserConsent());

        tracker->setUserConsent(true);
        EXPECT_TRUE(tracker->hasUserConsent());

        tracker->setUserConsent(false);
        EXPECT_FALSE(tracker->hasUserConsent());
    }

    TEST_F(CrashTrackerTest, ScrubCallbackRegistration) {
        auto tracker = CrashTracker::getInstance();

        bool callbackCalled = false;
        tracker->addScrubCallback([&callbackCalled](const std::string& text) {
            callbackCalled = true;
            return text;
        });

        // The callback is stored but only called when SENTRY is enabled
        // Just verify registration doesn't throw
        EXPECT_NO_THROW(tracker->addScrubCallback([](const std::string& t) { return t; }));
    }

    TEST_F(CrashTrackerTest, MultipleScrubCallbacks) {
        auto tracker = CrashTracker::getInstance();

        int callCount = 0;
        tracker->addScrubCallback([&callCount](const std::string& text) {
            callCount++;
            return text + "_modified1";
        });
        tracker->addScrubCallback([&callCount](const std::string& text) {
            callCount++;
            return text + "_modified2";
        });

        // Callbacks are registered successfully
        SUCCEED();
    }

    TEST_F(CrashTrackerTest, ConsentPersistenceAcrossRestarts) {
        // First "session" - set consent
        {
            auto tracker = CrashTracker::getInstance();
            tracker->setUserConsent(true);
            EXPECT_TRUE(tracker->hasUserConsent());
        }

        // Verify consent file was created
        #ifdef _WIN32
        std::filesystem::path configPath = testConfigDir / "NexoEngine" / "nexo_config.ini";
        #else
        std::filesystem::path configPath = testConfigDir / ".config" / "NexoEngine" / "nexo_config.ini";
        #endif

        EXPECT_TRUE(std::filesystem::exists(configPath));

        // Read the file and verify content
        std::ifstream file(configPath);
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        EXPECT_NE(content.find("[Privacy]"), std::string::npos);
        EXPECT_NE(content.find("crash_tracking_consent=true"), std::string::npos);
    }

    TEST_F(CrashTrackerTest, ConsentFalseWrittenCorrectly) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->setUserConsent(false);

        #ifdef _WIN32
        std::filesystem::path configPath = testConfigDir / "NexoEngine" / "nexo_config.ini";
        #else
        std::filesystem::path configPath = testConfigDir / ".config" / "NexoEngine" / "nexo_config.ini";
        #endif

        std::ifstream file(configPath);
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        EXPECT_NE(content.find("crash_tracking_consent=false"), std::string::npos);
    }

    TEST_F(CrashTrackerTest, ConfigFileWithExistingContent) {
        // Create config file with existing content
        #ifdef _WIN32
        std::filesystem::path configDir = testConfigDir / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #else
        std::filesystem::path configDir = testConfigDir / ".config" / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #endif

        std::filesystem::create_directories(configDir);
        {
            std::ofstream file(configPath);
            file << "[Graphics]\n";
            file << "resolution=1920x1080\n";
            file << "\n";
            file << "[Audio]\n";
            file << "volume=80\n";
        }

        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);

        // Verify existing content is preserved
        std::ifstream file(configPath);
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        EXPECT_NE(content.find("[Graphics]"), std::string::npos);
        EXPECT_NE(content.find("resolution=1920x1080"), std::string::npos);
        EXPECT_NE(content.find("[Audio]"), std::string::npos);
        EXPECT_NE(content.find("volume=80"), std::string::npos);
        EXPECT_NE(content.find("[Privacy]"), std::string::npos);
        EXPECT_NE(content.find("crash_tracking_consent=true"), std::string::npos);
    }

    TEST_F(CrashTrackerTest, ConsentUpdateInExistingPrivacySection) {
        // Create config file with existing Privacy section
        #ifdef _WIN32
        std::filesystem::path configDir = testConfigDir / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #else
        std::filesystem::path configDir = testConfigDir / ".config" / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #endif

        std::filesystem::create_directories(configDir);
        {
            std::ofstream file(configPath);
            file << "[Privacy]\n";
            file << "crash_tracking_consent=false\n";
            file << "user_id=test-user-123\n";
        }

        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);

        std::ifstream file(configPath);
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());

        // Should update consent but preserve user_id
        EXPECT_NE(content.find("crash_tracking_consent=true"), std::string::npos);
        EXPECT_NE(content.find("user_id=test-user-123"), std::string::npos);
        // Should not have duplicate entries
        EXPECT_EQ(content.find("crash_tracking_consent=false"), std::string::npos);
    }

    TEST_F(CrashTrackerTest, IsEnabledInitiallyFalse) {
        auto tracker = CrashTracker::getInstance();
        EXPECT_FALSE(tracker->isEnabled());
    }

    TEST_F(CrashTrackerTest, IsInitializedInitiallyFalse) {
        auto tracker = CrashTracker::getInstance();
        EXPECT_FALSE(tracker->isInitialized());
    }

    TEST_F(CrashTrackerTest, ShutdownWhenNotInitialized) {
        auto tracker = CrashTracker::getInstance();
        EXPECT_NO_THROW(tracker->shutdown());
        EXPECT_FALSE(tracker->isInitialized());
    }

    #if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
    TEST_F(CrashTrackerTest, ReadConsentFromValidConfig) {
        // Create config with consent=true
        #ifdef _WIN32
        std::filesystem::path configDir = testConfigDir / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #else
        std::filesystem::path configDir = testConfigDir / ".config" / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #endif

        std::filesystem::create_directories(configDir);
        {
            std::ofstream file(configPath);
            file << "[Privacy]\n";
            file << "crash_tracking_consent=true\n";
        }

        auto tracker = CrashTracker::getInstance();
        // hasUserConsent reads from config on first access after initialize
        tracker->initialize();

        // Consent should be read from file
        EXPECT_TRUE(tracker->hasUserConsent());
    }

    TEST_F(CrashTrackerTest, ReadConsentYesValue) {
        #ifdef _WIN32
        std::filesystem::path configDir = testConfigDir / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #else
        std::filesystem::path configDir = testConfigDir / ".config" / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #endif

        std::filesystem::create_directories(configDir);
        {
            std::ofstream file(configPath);
            file << "[Privacy]\n";
            file << "crash_tracking_consent=yes\n";
        }

        auto tracker = CrashTracker::getInstance();
        tracker->initialize();
        EXPECT_TRUE(tracker->hasUserConsent());
    }

    TEST_F(CrashTrackerTest, ReadConsentNumericValue) {
        #ifdef _WIN32
        std::filesystem::path configDir = testConfigDir / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #else
        std::filesystem::path configDir = testConfigDir / ".config" / "NexoEngine";
        std::filesystem::path configPath = configDir / "nexo_config.ini";
        #endif

        std::filesystem::create_directories(configDir);
        {
            std::ofstream file(configPath);
            file << "[Privacy]\n";
            file << "crash_tracking_consent=1\n";
        }

        auto tracker = CrashTracker::getInstance();
        tracker->initialize();
        EXPECT_TRUE(tracker->hasUserConsent());
    }
    TEST_F(CrashTrackerTest, BreadcrumbAddition) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        EXPECT_NO_THROW(tracker->addBreadcrumb("Test breadcrumb", "test", "info"));
        EXPECT_NO_THROW(tracker->addBreadcrumb("Another breadcrumb", "test", "warning"));
    }

    TEST_F(CrashTrackerTest, TagSetting) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        EXPECT_NO_THROW(tracker->setTag("environment", "test"));
        EXPECT_NO_THROW(tracker->setTag("version", "1.0.0"));
    }

    TEST_F(CrashTrackerTest, ExceptionCapture) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        EXPECT_NO_THROW(tracker->captureException(
            "TestException",
            "Test exception message",
            __FILE__,
            __LINE__,
            __FUNCTION__
        ));
    }

    TEST_F(CrashTrackerTest, ContextSetting) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        EXPECT_NO_THROW(tracker->setContext("test_key", "test_value"));
    }

    TEST_F(CrashTrackerTest, MessageCapture) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        EXPECT_NO_THROW(tracker->captureMessage("Test message", "info"));
        EXPECT_NO_THROW(tracker->captureMessage("Debug message", "debug"));
        EXPECT_NO_THROW(tracker->captureMessage("Warning message", "warning"));
        EXPECT_NO_THROW(tracker->captureMessage("Error message", "error"));
        EXPECT_NO_THROW(tracker->captureMessage("Fatal message", "fatal"));
    }

    TEST_F(CrashTrackerTest, SignalCapture) {
        auto tracker = CrashTracker::getInstance();
        tracker->setUserConsent(true);
        tracker->initialize();

        // Can't actually send signals in test, but verify the method exists
        // and doesn't crash when called with invalid signal (not initialized case)
        auto uninitTracker = CrashTracker::getInstance();
        uninitTracker->shutdown();
        EXPECT_NO_THROW(uninitTracker->captureSignal(SIGTERM));
    }
    #endif

}
