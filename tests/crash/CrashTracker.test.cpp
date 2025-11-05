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

        EXPECT_FALSE(tracker->hasUserConsent());

        tracker->setUserConsent(true);
        EXPECT_TRUE(tracker->hasUserConsent());

        tracker->setUserConsent(false);
        EXPECT_FALSE(tracker->hasUserConsent());
    }

    #if defined(NEXO_SENTRY_ENABLED) || defined(NEXO_SENTRY_DEBUG_MODE)
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
    #endif

}
