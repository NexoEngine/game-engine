#include <gtest/gtest.h>

// Init tests
// Use this main file instead of GTest::gtest_main in linking
/**
 * @brief Entry point for executing Google Test unit tests.
 *
 * @details Initializes the Google Test framework using command-line arguments and runs all registered tests.
 * Logs the total number of tests executed. If no tests are run, it logs a fatal error indicating a potential linking configuration issue and returns 1;
 * otherwise, it returns the result from running the tests.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 *
 * @return int Exit code based on the outcome of the tests.
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
