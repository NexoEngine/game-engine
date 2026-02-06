//// OnceRegistry.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for OnceRegistry singleton class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Logger.hpp"
#include <thread>
#include <vector>
#include <atomic>

namespace nexo {

// =============================================================================
// Singleton Tests
// =============================================================================

class OnceRegistrySingletonTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistrySingletonTest, InstanceReturnsReference) {
    OnceRegistry& registry = OnceRegistry::instance();
    // Just verify we can get a reference
    EXPECT_TRUE(&registry != nullptr);
}

TEST_F(OnceRegistrySingletonTest, SameInstanceReturned) {
    OnceRegistry& registry1 = OnceRegistry::instance();
    OnceRegistry& registry2 = OnceRegistry::instance();
    EXPECT_EQ(&registry1, &registry2);
}

// =============================================================================
// shouldLog Tests
// =============================================================================

class OnceRegistryShouldLogTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryShouldLogTest, FirstCallReturnsTrue) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryShouldLogTest, SecondCallReturnsFalse) {
    OnceRegistry::instance().shouldLog("test_key");
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryShouldLogTest, ThirdCallReturnsFalse) {
    OnceRegistry::instance().shouldLog("test_key");
    OnceRegistry::instance().shouldLog("test_key");
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryShouldLogTest, DifferentKeysAreIndependent) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key2"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key3"));
}

TEST_F(OnceRegistryShouldLogTest, SameKeyMultipleCallsOnlyFirstTrue) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("same_key"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("same_key"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("same_key"));
}

TEST_F(OnceRegistryShouldLogTest, EmptyKeyWorks) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog(""));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog(""));
}

TEST_F(OnceRegistryShouldLogTest, LongKeyWorks) {
    std::string longKey(1000, 'x');
    EXPECT_TRUE(OnceRegistry::instance().shouldLog(longKey));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog(longKey));
}

TEST_F(OnceRegistryShouldLogTest, SpecialCharactersInKey) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key:with:colons"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key/with/slashes"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key\nwith\nnewlines"));
}

// =============================================================================
// reset Tests
// =============================================================================

class OnceRegistryResetTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryResetTest, ResetAllowsLoggingAgain) {
    OnceRegistry::instance().shouldLog("test_key");
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("test_key"));

    OnceRegistry::instance().reset("test_key");
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryResetTest, ResetOnlyAffectsSpecifiedKey) {
    OnceRegistry::instance().shouldLog("key1");
    OnceRegistry::instance().shouldLog("key2");

    OnceRegistry::instance().reset("key1");

    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("key2"));
}

TEST_F(OnceRegistryResetTest, ResetNonExistentKeyIsHarmless) {
    // Should not throw
    EXPECT_NO_THROW(OnceRegistry::instance().reset("never_logged"));
}

TEST_F(OnceRegistryResetTest, MultipleResetsWork) {
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(OnceRegistry::instance().shouldLog("key"));
        OnceRegistry::instance().reset("key");
    }
}

// =============================================================================
// resetAll Tests
// =============================================================================

class OnceRegistryResetAllTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryResetAllTest, ClearsAllKeys) {
    OnceRegistry::instance().shouldLog("key1");
    OnceRegistry::instance().shouldLog("key2");
    OnceRegistry::instance().shouldLog("key3");

    OnceRegistry::instance().resetAll();

    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key2"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key3"));
}

TEST_F(OnceRegistryResetAllTest, ResetAllOnEmptyRegistryIsHarmless) {
    EXPECT_NO_THROW(OnceRegistry::instance().resetAll());
}

TEST_F(OnceRegistryResetAllTest, MultipleResetAllsWork) {
    OnceRegistry::instance().shouldLog("key");
    OnceRegistry::instance().resetAll();
    OnceRegistry::instance().resetAll();
    OnceRegistry::instance().resetAll();
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key"));
}

// =============================================================================
// Thread Safety Tests
// =============================================================================

class OnceRegistryThreadSafetyTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryThreadSafetyTest, ConcurrentShouldLogOnSameKey) {
    std::atomic<int> trueCount{0};
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&trueCount]() {
            if (OnceRegistry::instance().shouldLog("concurrent_key")) {
                trueCount++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Only one thread should have gotten true
    EXPECT_EQ(trueCount.load(), 1);
}

TEST_F(OnceRegistryThreadSafetyTest, ConcurrentDifferentKeys) {
    std::atomic<int> trueCount{0};
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.emplace_back([i, &trueCount]() {
            std::string key = "key_" + std::to_string(i);
            if (OnceRegistry::instance().shouldLog(key)) {
                trueCount++;
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // All threads should have gotten true (different keys)
    EXPECT_EQ(trueCount.load(), 10);
}

TEST_F(OnceRegistryThreadSafetyTest, ConcurrentResetAndShouldLog) {
    // This test verifies no crashes occur with concurrent operations
    std::vector<std::thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back([]() {
            for (int j = 0; j < 100; j++) {
                OnceRegistry::instance().shouldLog("test_key");
            }
        });
        threads.emplace_back([]() {
            for (int j = 0; j < 100; j++) {
                OnceRegistry::instance().reset("test_key");
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // If we get here without crashes, the test passes
    SUCCEED();
}

// =============================================================================
// Use Case Tests
// =============================================================================

class OnceRegistryUseCaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryUseCaseTest, LogOncePattern) {
    int logCount = 0;

    for (int i = 0; i < 10; i++) {
        if (OnceRegistry::instance().shouldLog("deprecation_warning")) {
            logCount++;
        }
    }

    EXPECT_EQ(logCount, 1);
}

TEST_F(OnceRegistryUseCaseTest, MultipleWarningsEachLogOnce) {
    int warning1Count = 0;
    int warning2Count = 0;

    for (int i = 0; i < 10; i++) {
        if (OnceRegistry::instance().shouldLog("warning1")) {
            warning1Count++;
        }
        if (OnceRegistry::instance().shouldLog("warning2")) {
            warning2Count++;
        }
    }

    EXPECT_EQ(warning1Count, 1);
    EXPECT_EQ(warning2Count, 1);
}

}  // namespace nexo
