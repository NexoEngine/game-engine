#include <gtest/gtest.h>

// Init tests
// Use this main file instead of GTest::gtest_main in linking
/**
 * @brief Initializes and runs Google Test unit tests.
 *
 * This function sets up the Google Test framework using the provided command-line arguments,
 * executes all registered tests, and logs the total number of tests run. If no tests are executed,
 * it logs a fatal error indicating a potential linking configuration issue and returns an error code.
 * Otherwise, it returns the status code from the test run.
 *
 * @return int The exit code from running the tests, or 1 if no tests were executed.
 *
 * @see https://github.com/google/googletest/issues/2157
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
