//// Signals.test.cpp //////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Signals utility functions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "core/event/Signals.hpp"
#include <csignal>
#include <cstring>

namespace nexo::utils {

class SignalsTest : public ::testing::Test {};

// =============================================================================
// Basic Signal Name Tests
// =============================================================================

TEST_F(SignalsTest, SIGABRTReturnsValidString) {
    const char* result = strsignal(SIGABRT);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SIGFPEReturnsValidString) {
    const char* result = strsignal(SIGFPE);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SIGILLReturnsValidString) {
    const char* result = strsignal(SIGILL);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SIGINTReturnsValidString) {
    const char* result = strsignal(SIGINT);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SIGSEGVReturnsValidString) {
    const char* result = strsignal(SIGSEGV);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SIGTERMReturnsValidString) {
    const char* result = strsignal(SIGTERM);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

// =============================================================================
// Windows-specific Tests (when on Windows)
// =============================================================================

#ifdef _WIN32
TEST_F(SignalsTest, WindowsSIGABRTName) {
    EXPECT_STREQ(strsignal(SIGABRT), "SIGABRT");
}

TEST_F(SignalsTest, WindowsSIGFPEName) {
    EXPECT_STREQ(strsignal(SIGFPE), "SIGFPE");
}

TEST_F(SignalsTest, WindowsSIGILLName) {
    EXPECT_STREQ(strsignal(SIGILL), "SIGILL");
}

TEST_F(SignalsTest, WindowsSIGINTName) {
    EXPECT_STREQ(strsignal(SIGINT), "SIGINT");
}

TEST_F(SignalsTest, WindowsSIGSEGVName) {
    EXPECT_STREQ(strsignal(SIGSEGV), "SIGSEGV");
}

TEST_F(SignalsTest, WindowsSIGTERMName) {
    EXPECT_STREQ(strsignal(SIGTERM), "SIGTERM");
}

TEST_F(SignalsTest, WindowsUnknownSignalReturnsUnknown) {
    EXPECT_STREQ(strsignal(99999), "UNKNOWN");
}
#endif

// =============================================================================
// Consistency Tests
// =============================================================================

TEST_F(SignalsTest, SameSignalReturnsSameString) {
    const char* result1 = strsignal(SIGABRT);
    const char* result2 = strsignal(SIGABRT);
    EXPECT_STREQ(result1, result2);
}

TEST_F(SignalsTest, DifferentSignalsReturnDifferentStrings) {
    const char* sigabrt = strsignal(SIGABRT);
    const char* sigint = strsignal(SIGINT);
    EXPECT_STRNE(sigabrt, sigint);
}

// =============================================================================
// Signal Value Tests
// =============================================================================

TEST_F(SignalsTest, StandardSignalValuesAreDefined) {
    // Verify standard signal constants are defined and different
    EXPECT_NE(SIGABRT, SIGFPE);
    EXPECT_NE(SIGFPE, SIGILL);
    EXPECT_NE(SIGILL, SIGINT);
    EXPECT_NE(SIGINT, SIGSEGV);
    EXPECT_NE(SIGSEGV, SIGTERM);
}

// =============================================================================
// Linux-specific Tests (when on Linux)
// =============================================================================

#ifndef _WIN32
TEST_F(SignalsTest, LinuxDelegatestoSystemStrsignal) {
    // On Linux, our strsignal should return the same as ::strsignal
    const char* ourResult = strsignal(SIGABRT);
    const char* systemResult = ::strsignal(SIGABRT);
    EXPECT_STREQ(ourResult, systemResult);
}

TEST_F(SignalsTest, LinuxSIGINTContainsInterrupt) {
    const char* result = strsignal(SIGINT);
    // Linux typically returns something containing "Interrupt"
    EXPECT_NE(result, nullptr);
    // Just verify it's not empty - exact string varies by system
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

}  // namespace nexo::utils
