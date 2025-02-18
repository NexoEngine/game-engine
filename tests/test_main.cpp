#include <gtest/gtest.h>

// Init tests
// Use this main file instead of GTest::gtest_main in linking
// See: https://github.com/google/googletest/issues/2157
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    const int rv = RUN_ALL_TESTS();
    GTEST_LOG_(INFO) << "Ran " << ::testing::UnitTest::GetInstance()->test_to_run_count() << " tests";
    if (::testing::UnitTest::GetInstance()->test_to_run_count() == 0) {
        GTEST_LOG_(FATAL) << "No tests were run. This might be due to a wrong linking configuration";
        return 1;
    }
    return RUN_ALL_TESTS();
}
