//// Logger.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Logger utilities (LogLevel, getFileName, OnceRegistry)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Logger.hpp"

namespace nexo {

// =============================================================================
// LogLevel toString Tests
// =============================================================================

class LogLevelTest : public ::testing::Test {};

TEST_F(LogLevelTest, FatalToString) {
    EXPECT_EQ(toString(LogLevel::FATAL), "FATAL");
}

TEST_F(LogLevelTest, ErrorToString) {
    EXPECT_EQ(toString(LogLevel::ERR), "ERROR");
}

TEST_F(LogLevelTest, WarnToString) {
    EXPECT_EQ(toString(LogLevel::WARN), "WARN");
}

TEST_F(LogLevelTest, InfoToString) {
    EXPECT_EQ(toString(LogLevel::INFO), "INFO");
}

TEST_F(LogLevelTest, DebugToString) {
    EXPECT_EQ(toString(LogLevel::DEBUG), "DEBUG");
}

TEST_F(LogLevelTest, DevToString) {
    EXPECT_EQ(toString(LogLevel::DEV), "DEV");
}

TEST_F(LogLevelTest, UserToString) {
    EXPECT_EQ(toString(LogLevel::USER), "USER");
}

// =============================================================================
// getFileName Tests
// =============================================================================

class GetFileNameTest : public ::testing::Test {};

TEST_F(GetFileNameTest, ExtractsFilenameFromUnixPath) {
    EXPECT_EQ(getFileName("/home/user/project/src/main.cpp"), "main.cpp");
}

TEST_F(GetFileNameTest, ExtractsFilenameFromWindowsPath) {
    EXPECT_EQ(getFileName("C:\\Users\\user\\project\\src\\main.cpp"), "main.cpp");
}

TEST_F(GetFileNameTest, ExtractsFilenameFromMixedPath) {
    EXPECT_EQ(getFileName("/home/user/project\\src/main.cpp"), "main.cpp");
}

TEST_F(GetFileNameTest, ReturnsFilenameWhenNoPath) {
    EXPECT_EQ(getFileName("main.cpp"), "main.cpp");
}

TEST_F(GetFileNameTest, HandlesTrailingSlash) {
    // When path ends with slash, returns empty string (everything after last slash)
    EXPECT_EQ(getFileName("/home/user/"), "");
}

TEST_F(GetFileNameTest, HandlesSingleFilename) {
    EXPECT_EQ(getFileName("test"), "test");
}

TEST_F(GetFileNameTest, HandlesDeepNestedPath) {
    EXPECT_EQ(getFileName("/a/b/c/d/e/f/g/file.txt"), "file.txt");
}

TEST_F(GetFileNameTest, HandlesFilenameWithMultipleDots) {
    EXPECT_EQ(getFileName("/path/to/file.test.cpp"), "file.test.cpp");
}

TEST_F(GetFileNameTest, HandlesEmptyString) {
    EXPECT_EQ(getFileName(""), "");
}

TEST_F(GetFileNameTest, HandlesRootPath) {
    EXPECT_EQ(getFileName("/"), "");
}

// =============================================================================
// OnceRegistry Tests
// =============================================================================

class OnceRegistryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset registry before each test
        OnceRegistry::instance().resetAll();
    }

    void TearDown() override {
        // Clean up after each test
        OnceRegistry::instance().resetAll();
    }
};

TEST_F(OnceRegistryTest, SingletonReturnssSameInstance) {
    auto& instance1 = OnceRegistry::instance();
    auto& instance2 = OnceRegistry::instance();
    EXPECT_EQ(&instance1, &instance2);
}

TEST_F(OnceRegistryTest, ShouldLogReturnsTrueFirstTime) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryTest, ShouldLogReturnsFalseSecondTime) {
    OnceRegistry::instance().shouldLog("test_key");
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryTest, DifferentKeysAreIndependent) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key2"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("key2"));
}

TEST_F(OnceRegistryTest, ResetAllowsLoggingAgain) {
    OnceRegistry::instance().shouldLog("test_key");
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("test_key"));

    OnceRegistry::instance().reset("test_key");
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("test_key"));
}

TEST_F(OnceRegistryTest, ResetOnlyAffectsSpecifiedKey) {
    OnceRegistry::instance().shouldLog("key1");
    OnceRegistry::instance().shouldLog("key2");

    OnceRegistry::instance().reset("key1");

    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog("key2"));
}

TEST_F(OnceRegistryTest, ResetAllClearsAllKeys) {
    OnceRegistry::instance().shouldLog("key1");
    OnceRegistry::instance().shouldLog("key2");
    OnceRegistry::instance().shouldLog("key3");

    OnceRegistry::instance().resetAll();

    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key1"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key2"));
    EXPECT_TRUE(OnceRegistry::instance().shouldLog("key3"));
}

TEST_F(OnceRegistryTest, ResetNonExistentKeyDoesNotThrow) {
    EXPECT_NO_THROW(OnceRegistry::instance().reset("nonexistent"));
}

TEST_F(OnceRegistryTest, EmptyKeyWorks) {
    EXPECT_TRUE(OnceRegistry::instance().shouldLog(""));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog(""));
}

TEST_F(OnceRegistryTest, LongKeyWorks) {
    std::string longKey(1000, 'x');
    EXPECT_TRUE(OnceRegistry::instance().shouldLog(longKey));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog(longKey));
}

TEST_F(OnceRegistryTest, SpecialCharactersInKeyWork) {
    std::string specialKey = "key@with#special$chars%and^symbols";
    EXPECT_TRUE(OnceRegistry::instance().shouldLog(specialKey));
    EXPECT_FALSE(OnceRegistry::instance().shouldLog(specialKey));
}

// =============================================================================
// Logger generateKey Tests
// =============================================================================

class LoggerGenerateKeyTest : public ::testing::Test {};

TEST_F(LoggerGenerateKeyTest, GeneratesKeyWithFormat) {
    std::string key = Logger::generateKey("test format", "file.cpp:10");
    EXPECT_FALSE(key.empty());
    EXPECT_NE(key.find("test format"), std::string::npos);
    EXPECT_NE(key.find("file.cpp:10"), std::string::npos);
}

TEST_F(LoggerGenerateKeyTest, GeneratesKeyWithParameters) {
    std::string key = Logger::generateKey("value: {}", "file.cpp:10", 42);
    EXPECT_NE(key.find("42"), std::string::npos);
}

TEST_F(LoggerGenerateKeyTest, GeneratesKeyWithMultipleParameters) {
    std::string key = Logger::generateKey("values: {} {}", "file.cpp:10", 1, 2);
    EXPECT_NE(key.find("1"), std::string::npos);
    EXPECT_NE(key.find("2"), std::string::npos);
}

TEST_F(LoggerGenerateKeyTest, DifferentParametersGenerateDifferentKeys) {
    std::string key1 = Logger::generateKey("value: {}", "file.cpp:10", 1);
    std::string key2 = Logger::generateKey("value: {}", "file.cpp:10", 2);
    EXPECT_NE(key1, key2);
}

TEST_F(LoggerGenerateKeyTest, SameParametersGenerateSameKey) {
    std::string key1 = Logger::generateKey("value: {}", "file.cpp:10", 42);
    std::string key2 = Logger::generateKey("value: {}", "file.cpp:10", 42);
    EXPECT_EQ(key1, key2);
}

TEST_F(LoggerGenerateKeyTest, DifferentLocationsGenerateDifferentKeys) {
    std::string key1 = Logger::generateKey("test", "file1.cpp:10");
    std::string key2 = Logger::generateKey("test", "file2.cpp:20");
    EXPECT_NE(key1, key2);
}

// =============================================================================
// toFormatFriendly Tests
// =============================================================================

class ToFormatFriendlyTest : public ::testing::Test {};

TEST_F(ToFormatFriendlyTest, StringViewPassthrough) {
    std::string_view sv = "test";
    auto result = toFormatFriendly(sv);
    EXPECT_EQ(result, "test");
}

TEST_F(ToFormatFriendlyTest, CStringConversion) {
    const char* cs = "test";
    auto result = toFormatFriendly(cs);
    EXPECT_EQ(result, "test");
}

TEST_F(ToFormatFriendlyTest, IntegerConversion) {
    auto result = toFormatFriendly(42);
    EXPECT_EQ(result, "42");
}

TEST_F(ToFormatFriendlyTest, FloatConversion) {
    auto result = toFormatFriendly(3.14f);
    EXPECT_NE(result.find("3.14"), std::string::npos);
}

TEST_F(ToFormatFriendlyTest, NegativeNumberConversion) {
    auto result = toFormatFriendly(-100);
    EXPECT_EQ(result, "-100");
}

}  // namespace nexo
