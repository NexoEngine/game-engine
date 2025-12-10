//// Error.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Error utilities (SafeStrerror, strerror)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Error.hpp"
#include <cerrno>

namespace nexo {

// =============================================================================
// SafeStrerror Tests
// =============================================================================

class SafeStrerrorTest : public ::testing::Test {};

TEST_F(SafeStrerrorTest, GetErrorMessageForEINVAL) {
    std::string msg = SafeStrerror::getErrorMessage(EINVAL);
    EXPECT_FALSE(msg.empty());
    // Should not be "Unknown error" for a valid error code
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, GetErrorMessageForENOENT) {
    std::string msg = SafeStrerror::getErrorMessage(ENOENT);
    EXPECT_FALSE(msg.empty());
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, GetErrorMessageForEACCES) {
    std::string msg = SafeStrerror::getErrorMessage(EACCES);
    EXPECT_FALSE(msg.empty());
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, GetErrorMessageForEIO) {
    std::string msg = SafeStrerror::getErrorMessage(EIO);
    EXPECT_FALSE(msg.empty());
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, GetErrorMessageForENOMEM) {
    std::string msg = SafeStrerror::getErrorMessage(ENOMEM);
    EXPECT_FALSE(msg.empty());
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, GetErrorMessageForZero) {
    // Error code 0 typically means "Success" or "No error"
    std::string msg = SafeStrerror::getErrorMessage(0);
    EXPECT_FALSE(msg.empty());
}

TEST_F(SafeStrerrorTest, GetErrorMessageReturnsNonEmptyString) {
    // For any typical error code, we should get a non-empty string
    for (int i = 1; i <= 10; ++i) {
        std::string msg = SafeStrerror::getErrorMessage(i);
        EXPECT_FALSE(msg.empty()) << "Error code " << i << " returned empty string";
    }
}

TEST_F(SafeStrerrorTest, GetErrorMessageNoExceptionForNegative) {
    // Should not throw for negative error codes
    EXPECT_NO_THROW(SafeStrerror::getErrorMessage(-1));
}

TEST_F(SafeStrerrorTest, GetErrorMessageNoExceptionForLargeValue) {
    // Should not throw for large error codes
    EXPECT_NO_THROW(SafeStrerror::getErrorMessage(99999));
}

TEST_F(SafeStrerrorTest, GetCurrentErrnoMessage) {
    errno = ENOENT;
    std::string msg = SafeStrerror::getErrorMessage();
    EXPECT_FALSE(msg.empty());
    EXPECT_NE(msg, "Unknown error");
}

TEST_F(SafeStrerrorTest, DifferentErrorsGiveDifferentMessages) {
    std::string msg1 = SafeStrerror::getErrorMessage(ENOENT);
    std::string msg2 = SafeStrerror::getErrorMessage(EACCES);
    // Different errors should (usually) produce different messages
    // Note: This test may fail on some systems if messages are identical
    EXPECT_NE(msg1, msg2);
}

// =============================================================================
// nexo::strerror() Wrapper Tests
// =============================================================================

class StrerrorWrapperTest : public ::testing::Test {};

TEST_F(StrerrorWrapperTest, StrerrorWithErrorNumber) {
    std::string msg = nexo::strerror(EINVAL);
    EXPECT_FALSE(msg.empty());
    EXPECT_EQ(msg, SafeStrerror::getErrorMessage(EINVAL));
}

TEST_F(StrerrorWrapperTest, StrerrorWithCurrentErrno) {
    errno = ENOMEM;
    std::string msgWrapper = nexo::strerror();
    std::string msgDirect = SafeStrerror::getErrorMessage(ENOMEM);
    EXPECT_EQ(msgWrapper, msgDirect);
}

TEST_F(StrerrorWrapperTest, StrerrorIsNoexcept) {
    // The function is marked noexcept, verify it doesn't throw
    EXPECT_NO_THROW(nexo::strerror(EINVAL));
    EXPECT_NO_THROW(nexo::strerror());
}

TEST_F(StrerrorWrapperTest, StrerrorConsistentResults) {
    // Multiple calls with same error should return same message
    std::string msg1 = nexo::strerror(ENOENT);
    std::string msg2 = nexo::strerror(ENOENT);
    EXPECT_EQ(msg1, msg2);
}

// =============================================================================
// Thread Safety Tests (Basic)
// =============================================================================

TEST_F(SafeStrerrorTest, MultipleCallsAreConsistent) {
    // Call multiple times to ensure consistency
    const int error_code = EINVAL;
    std::string first_result = SafeStrerror::getErrorMessage(error_code);

    for (int i = 0; i < 100; ++i) {
        std::string result = SafeStrerror::getErrorMessage(error_code);
        EXPECT_EQ(result, first_result) << "Iteration " << i;
    }
}

}  // namespace nexo
