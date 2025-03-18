#include <gtest/gtest.h>

// Init tests
// Use this main file instead of GTest::gtest_main in linking
/**
 * @brief Entry point for executing Google Test cases.
 *
 * Initializes the Google Test framework with command-line arguments and runs all registered tests.
 * Logs the total number of tests executed. If no tests are run, outputs a fatal error indicating a potential
 * linking configuration issue and returns 1. Otherwise, returns the result of the test execution.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of C-strings representing the command-line arguments.
 *
 * @return int Returns 1 if no tests were executed; otherwise, returns the status code from the test execution.
 */
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    const int rv = RUN_ALL_TESTS();
    GTEST_LOG_(INFO) << "Ran " << ::testing::UnitTest::GetInstance()->test_to_run_count() << " tests";
    if (::testing::UnitTest::GetInstance()->test_to_run_count() == 0) {
        GTEST_LOG_(FATAL) << "No tests were run. This might be due to a wrong linking configuration";
        return 1;
    }
    return rv;
}
