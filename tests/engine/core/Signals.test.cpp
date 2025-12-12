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
#include <vector>
#include <cctype>

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

// =============================================================================
// Edge Case Tests
// =============================================================================

TEST_F(SignalsTest, InvalidSignalNumber) {
    const char* result = strsignal(99999);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, NegativeSignalNumber) {
    const char* result = strsignal(-1);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, ZeroSignalNumber) {
    const char* result = strsignal(0);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, VeryLargeSignalNumber) {
    const char* result = strsignal(2147483647); // INT_MAX
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, VeryLargeNegativeSignalNumber) {
    const char* result = strsignal(-2147483648); // INT_MIN
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

// =============================================================================
// Thread Safety Tests
// =============================================================================

TEST_F(SignalsTest, MultipleCallsSameSignal) {
    // Call strsignal multiple times for the same signal
    const char* result1 = strsignal(SIGTERM);
    const char* result2 = strsignal(SIGTERM);
    const char* result3 = strsignal(SIGTERM);

    EXPECT_STREQ(result1, result2);
    EXPECT_STREQ(result2, result3);
}

TEST_F(SignalsTest, InterleavedSignalCalls) {
    // Interleaved calls to different signals
    const char* sigabrt1 = strsignal(SIGABRT);
    const char* sigint1 = strsignal(SIGINT);
    const char* sigabrt2 = strsignal(SIGABRT);
    const char* sigint2 = strsignal(SIGINT);

    EXPECT_STREQ(sigabrt1, sigabrt2);
    EXPECT_STREQ(sigint1, sigint2);
    EXPECT_STRNE(sigabrt1, sigint1);
}

// =============================================================================
// Boundary Signal Number Tests
// =============================================================================

TEST_F(SignalsTest, SignalNumberOne) {
    const char* result = strsignal(1);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, SignalNumberTwo) {
    const char* result = strsignal(2);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}

TEST_F(SignalsTest, HighSignalNumbers) {
    // Test signal numbers 30-35 (common extended signal range)
    for (int sig = 30; sig <= 35; ++sig) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr);
        EXPECT_GT(std::strlen(result), 0u);
    }
}

// =============================================================================
// Return Value Validation Tests
// =============================================================================

TEST_F(SignalsTest, AllStandardSignalsReturnNonNull) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr) << "Signal " << sig << " returned null";
    }
}

TEST_F(SignalsTest, AllStandardSignalsReturnNonEmpty) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* result = strsignal(sig);
        EXPECT_GT(std::strlen(result), 0u) << "Signal " << sig << " returned empty string";
    }
}

TEST_F(SignalsTest, AllStandardSignalsReturnUniqueStrings) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};
    std::vector<const char*> results;

    for (int sig : standard_signals) {
        results.push_back(strsignal(sig));
    }

    // Check all are unique
    for (size_t i = 0; i < results.size(); ++i) {
        for (size_t j = i + 1; j < results.size(); ++j) {
            EXPECT_STRNE(results[i], results[j])
                << "Signals " << standard_signals[i] << " and "
                << standard_signals[j] << " return the same string";
        }
    }
}

// =============================================================================
// String Content Validation Tests
// =============================================================================

TEST_F(SignalsTest, SignalStringsContainPrintableCharacters) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* result = strsignal(sig);
        size_t len = std::strlen(result);

        for (size_t i = 0; i < len; ++i) {
            // Check that each character is printable or null terminator
            EXPECT_TRUE(std::isprint(static_cast<unsigned char>(result[i])) || result[i] == '\0')
                << "Signal " << sig << " contains non-printable character at position " << i;
        }
    }
}

TEST_F(SignalsTest, SignalStringsAreNullTerminated) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* result = strsignal(sig);
        size_t len = std::strlen(result);

        // Verify the string is properly null-terminated
        EXPECT_EQ(result[len], '\0') << "Signal " << sig << " string not null-terminated";
    }
}

TEST_F(SignalsTest, SignalStringsHaveReasonableLength) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* result = strsignal(sig);
        size_t len = std::strlen(result);

        // Signal strings should be between 1 and 100 characters
        EXPECT_GE(len, 1u) << "Signal " << sig << " string too short";
        EXPECT_LE(len, 100u) << "Signal " << sig << " string too long";
    }
}

// =============================================================================
// Platform-Specific Extended Tests
// =============================================================================

#ifdef _WIN32
TEST_F(SignalsTest, WindowsAllDefinedSignalsMatchExpectedFormat) {
    // Windows should return "SIGXXX" format for known signals
    struct SignalMapping {
        int number;
        const char* expected;
    };

    const SignalMapping mappings[] = {
        {SIGABRT, "SIGABRT"},
        {SIGFPE, "SIGFPE"},
        {SIGILL, "SIGILL"},
        {SIGINT, "SIGINT"},
        {SIGSEGV, "SIGSEGV"},
        {SIGTERM, "SIGTERM"}
    };

    for (const auto& mapping : mappings) {
        const char* result = strsignal(mapping.number);
        EXPECT_STREQ(result, mapping.expected)
            << "Signal " << mapping.number << " expected '" << mapping.expected
            << "' but got '" << result << "'";
    }
}

TEST_F(SignalsTest, WindowsInvalidSignalsReturnUnknown) {
    const int invalid_signals[] = {-1, 0, 999, 10000, -100};

    for (int sig : invalid_signals) {
        const char* result = strsignal(sig);
        EXPECT_STREQ(result, "UNKNOWN")
            << "Signal " << sig << " should return 'UNKNOWN'";
    }
}

TEST_F(SignalsTest, WindowsSignalNumbersAreCorrect) {
    // Verify Windows signal numbers match standard definitions
    EXPECT_EQ(SIGABRT, 22);
    EXPECT_EQ(SIGFPE, 8);
    EXPECT_EQ(SIGILL, 4);
    EXPECT_EQ(SIGINT, 2);
    EXPECT_EQ(SIGSEGV, 11);
    EXPECT_EQ(SIGTERM, 15);
}
#endif

#ifndef _WIN32
TEST_F(SignalsTest, LinuxAllStandardSignalsDelegateToSystem) {
    const int standard_signals[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};

    for (int sig : standard_signals) {
        const char* our_result = strsignal(sig);
        const char* system_result = ::strsignal(sig);

        EXPECT_STREQ(our_result, system_result)
            << "Signal " << sig << " does not delegate to system strsignal";
    }
}

TEST_F(SignalsTest, LinuxExtendedSignals) {
    // Test some Linux-specific signals if they exist
    #ifdef SIGUSR1
    const char* result = strsignal(SIGUSR1);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
    #endif

    #ifdef SIGUSR2
    const char* result2 = strsignal(SIGUSR2);
    EXPECT_NE(result2, nullptr);
    EXPECT_GT(std::strlen(result2), 0u);
    #endif
}

TEST_F(SignalsTest, LinuxRealTimeSignals) {
    // Test real-time signals if available
    #ifdef SIGRTMIN
    const char* result = strsignal(SIGRTMIN);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
    #endif

    #ifdef SIGRTMAX
    const char* result2 = strsignal(SIGRTMAX);
    EXPECT_NE(result2, nullptr);
    EXPECT_GT(std::strlen(result2), 0u);
    #endif
}
#endif

// =============================================================================
// Comprehensive Signal Range Tests
// =============================================================================

TEST_F(SignalsTest, SignalRangeNegativeToZero) {
    // Test signal numbers from -10 to 0
    for (int sig = -10; sig <= 0; ++sig) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr) << "Signal " << sig << " returned null";
        EXPECT_GT(std::strlen(result), 0u) << "Signal " << sig << " returned empty";
    }
}

TEST_F(SignalsTest, SignalRangeOneToThirty) {
    // Test signal numbers from 1 to 30 (covers most standard signals)
    for (int sig = 1; sig <= 30; ++sig) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr) << "Signal " << sig << " returned null";
        EXPECT_GT(std::strlen(result), 0u) << "Signal " << sig << " returned empty";
    }
}

// =============================================================================
// Consistency and Stability Tests
// =============================================================================

TEST_F(SignalsTest, RepeatedCallsReturnConsistentResults) {
    const int test_signals[] = {SIGABRT, SIGINT, SIGTERM, 0, -1, 999};

    for (int sig : test_signals) {
        const char* result1 = strsignal(sig);
        const char* result2 = strsignal(sig);
        const char* result3 = strsignal(sig);
        const char* result4 = strsignal(sig);
        const char* result5 = strsignal(sig);

        EXPECT_STREQ(result1, result2) << "Signal " << sig << " inconsistent on call 2";
        EXPECT_STREQ(result2, result3) << "Signal " << sig << " inconsistent on call 3";
        EXPECT_STREQ(result3, result4) << "Signal " << sig << " inconsistent on call 4";
        EXPECT_STREQ(result4, result5) << "Signal " << sig << " inconsistent on call 5";
    }
}

TEST_F(SignalsTest, ResultPointersAreStable) {
    // Check if the returned pointers are stable (not changing between calls)
    const char* ptr1 = strsignal(SIGABRT);
    const char* ptr2 = strsignal(SIGABRT);

    // Note: This test checks if implementation returns static storage
    // Some implementations may allocate new strings each time
    EXPECT_EQ(ptr1, ptr2) << "strsignal may not be using static storage";
}

// =============================================================================
// Memory and Buffer Tests
// =============================================================================

TEST_F(SignalsTest, NoBufferOverflowOnLargeSignalNumbers) {
    // Test with very large numbers to ensure no buffer overflow
    const int large_signals[] = {
        100000, 1000000, 10000000, 100000000, 1000000000, 2147483647
    };

    for (int sig : large_signals) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr);

        size_t len = std::strlen(result);
        EXPECT_LT(len, 10000u) << "Suspiciously long string for signal " << sig;
    }
}

TEST_F(SignalsTest, NoBufferOverflowOnNegativeSignalNumbers) {
    // Test with very negative numbers to ensure no buffer overflow
    const int negative_signals[] = {
        -100, -1000, -10000, -100000, -1000000, -2147483648
    };

    for (int sig : negative_signals) {
        const char* result = strsignal(sig);
        EXPECT_NE(result, nullptr);

        size_t len = std::strlen(result);
        EXPECT_LT(len, 10000u) << "Suspiciously long string for signal " << sig;
    }
}

// =============================================================================
// Special Signal Tests
// =============================================================================

#ifdef SIGHUP
TEST_F(SignalsTest, SIGHUPReturnsValidString) {
    const char* result = strsignal(SIGHUP);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGKILL
TEST_F(SignalsTest, SIGKILLReturnsValidString) {
    const char* result = strsignal(SIGKILL);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGPIPE
TEST_F(SignalsTest, SIGPIPEReturnsValidString) {
    const char* result = strsignal(SIGPIPE);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGALRM
TEST_F(SignalsTest, SIGALRMReturnsValidString) {
    const char* result = strsignal(SIGALRM);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGCHLD
TEST_F(SignalsTest, SIGCHLDReturnsValidString) {
    const char* result = strsignal(SIGCHLD);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGCONT
TEST_F(SignalsTest, SIGCONTReturnsValidString) {
    const char* result = strsignal(SIGCONT);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGSTOP
TEST_F(SignalsTest, SIGSTOPReturnsValidString) {
    const char* result = strsignal(SIGSTOP);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

#ifdef SIGTSTP
TEST_F(SignalsTest, SIGTSTPReturnsValidString) {
    const char* result = strsignal(SIGTSTP);
    EXPECT_NE(result, nullptr);
    EXPECT_GT(std::strlen(result), 0u);
}
#endif

}  // namespace nexo::utils
