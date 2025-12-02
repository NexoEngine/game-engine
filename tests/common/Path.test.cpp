#include <gtest/gtest.h>
#include "Path.hpp"

class PathTestFixture : public ::testing::Test {
protected:
    void TearDown() override {
        nexo::Path::resetCache();
    }
};

TEST_F(PathTestFixture, ResolvePathRelativeToExe) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "test.txt");
}

TEST_F(PathTestFixture, ResolvePathRelativeToExeTwice) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");
    const std::filesystem::path resolvedPath2 = nexo::Path::resolvePathRelativeToExe("test2.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "test.txt");
    EXPECT_EQ(resolvedPath2, exePath.parent_path() / "test2.txt");
}

TEST_F(PathTestFixture, ResolvePathRelativeToExeWithParent) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("resources/test.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "resources/test.txt");
}

/**
* @brief Test if the resolved paths are the same when called twice, check if cache works correctly
*/
TEST_F(PathTestFixture, ResolvePathRelativeToExeCache) {
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");
    const std::filesystem::path resolvedPath2 = nexo::Path::resolvePathRelativeToExe("test2.txt");

    EXPECT_EQ(resolvedPath.parent_path(), resolvedPath2.parent_path());
}

/**
* @brief Call twice the getExecutablePath method to check if the cache works correctly
*/
TEST_F(PathTestFixture, GetExecutablePathCache) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path exePath2 = nexo::Path::getExecutablePath();

    EXPECT_EQ(exePath, exePath2);
}

/**
 * @brief Test resetCache method
 */
TEST_F(PathTestFixture, ResetCache) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    nexo::Path::resetCache();
    const std::filesystem::path exePath2 = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");

    EXPECT_EQ(exePath, exePath2);
    EXPECT_EQ(resolvedPath, exePath2.parent_path() / "test.txt");
}

// ============================================================================
// normalizePathAndRemovePrefixSlash Tests
// ============================================================================

TEST(NormalizePathTest, EmptyPath) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash(""), "");
}

TEST(NormalizePathTest, RootPath) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/"), "");
}

TEST(NormalizePathTest, SimpleRelativePath) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("foo/bar"), "foo/bar");
}

TEST(NormalizePathTest, LeadingSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/bar"), "foo/bar");
}

TEST(NormalizePathTest, TrailingSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("foo/bar/"), "foo/bar");
}

TEST(NormalizePathTest, LeadingAndTrailingSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/bar/"), "foo/bar");
}

TEST(NormalizePathTest, MultipleLeadingSlashes) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("///foo/bar"), "foo/bar");
}

TEST(NormalizePathTest, MultipleTrailingSlashes) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("foo/bar///"), "foo/bar");
}

TEST(NormalizePathTest, DotDotNormalization) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/../bar"), "bar");
}

TEST(NormalizePathTest, DotNormalization) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/./bar"), "foo/bar");
}

TEST(NormalizePathTest, SingleFileName) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("file.txt"), "file.txt");
}

TEST(NormalizePathTest, SingleFileNameWithLeadingSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/file.txt"), "file.txt");
}

// ============================================================================
// splitPath Tests
// ============================================================================

TEST(SplitPathTest, EmptyPath) {
    const auto result = nexo::splitPath("");
    EXPECT_TRUE(result.empty());
}

TEST(SplitPathTest, SingleComponent) {
    const auto result = nexo::splitPath("foo");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "foo");
}

TEST(SplitPathTest, TwoComponents) {
    const auto result = nexo::splitPath("foo/bar");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
}

TEST(SplitPathTest, MultipleComponents) {
    const auto result = nexo::splitPath("foo/bar/baz/qux");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
    EXPECT_EQ(result[2], "baz");
    EXPECT_EQ(result[3], "qux");
}

TEST(SplitPathTest, AbsolutePath) {
    const auto result = nexo::splitPath("/foo/bar");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
}

TEST(SplitPathTest, PathWithExtension) {
    const auto result = nexo::splitPath("foo/bar/file.txt");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
    EXPECT_EQ(result[2], "file.txt");
}

TEST(SplitPathTest, RootPathOnly) {
    const auto result = nexo::splitPath("/");
    EXPECT_TRUE(result.empty());
}
