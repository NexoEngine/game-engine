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

TEST(SplitPathTest, TrailingSlash) {
    const auto result = nexo::splitPath("foo/bar/");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
}

TEST(SplitPathTest, DotComponent) {
    const auto result = nexo::splitPath("foo/./bar");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], ".");
    EXPECT_EQ(result[2], "bar");
}

TEST(SplitPathTest, DotDotComponent) {
    const auto result = nexo::splitPath("foo/../bar");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "..");
    EXPECT_EQ(result[2], "bar");
}

// ============================================================================
// normalizePathAndRemovePrefixSlash - Additional Edge Cases
// ============================================================================

TEST(NormalizePathTest, OnlySlashes) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("///"), "");
}

TEST(NormalizePathTest, RedundantSlashes) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("foo//bar///baz"), "foo/bar/baz");
}

TEST(NormalizePathTest, ComplexDotDotPath) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/bar/../baz/../../qux"), "qux");
}

TEST(NormalizePathTest, PathWithSpaces) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo bar/baz"), "foo bar/baz");
}

TEST(NormalizePathTest, PathWithSpecialCharacters) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo-bar_baz/file@123.txt"), "foo-bar_baz/file@123.txt");
}

TEST(NormalizePathTest, PathWithDots) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo.bar/baz.qux"), "foo.bar/baz.qux");
}

TEST(NormalizePathTest, SingleDot) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("."), ".");
}

TEST(NormalizePathTest, SingleDotWithSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/./"), "");
}

TEST(NormalizePathTest, DoubleDot) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash(".."), "..");
}

TEST(NormalizePathTest, DoubleDotWithSlash) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/../"), "");
}

TEST(NormalizePathTest, BackslashPath) {
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("foo\\bar\\baz");
    EXPECT_TRUE(result == "foo/bar/baz" || result == "foo\\bar\\baz");
}

TEST(NormalizePathTest, MixedSlashes) {
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("/foo\\bar/baz");
    EXPECT_TRUE(result == "foo/bar/baz" || result.find("foo") != std::string::npos);
}

TEST(NormalizePathTest, HiddenFile) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/.hidden"), ".hidden");
}

TEST(NormalizePathTest, HiddenDirectory) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/.hidden/file.txt"), ".hidden/file.txt");
}

// ============================================================================
// splitPath - Additional Edge Cases
// ============================================================================

TEST(SplitPathTest, HiddenFile) {
    const auto result = nexo::splitPath(".hidden");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], ".hidden");
}

TEST(SplitPathTest, HiddenDirectory) {
    const auto result = nexo::splitPath(".config/app/settings.ini");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], ".config");
    EXPECT_EQ(result[1], "app");
    EXPECT_EQ(result[2], "settings.ini");
}

TEST(SplitPathTest, FileWithMultipleDots) {
    const auto result = nexo::splitPath("archive.tar.gz");
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "archive.tar.gz");
}

TEST(SplitPathTest, PathWithSpaces) {
    const auto result = nexo::splitPath("My Documents/My Files/file.txt");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "My Documents");
    EXPECT_EQ(result[1], "My Files");
    EXPECT_EQ(result[2], "file.txt");
}

TEST(SplitPathTest, PathWithSpecialCharacters) {
    const auto result = nexo::splitPath("foo-bar_baz/file@123.txt");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "foo-bar_baz");
    EXPECT_EQ(result[1], "file@123.txt");
}

TEST(SplitPathTest, DeepPath) {
    const auto result = nexo::splitPath("a/b/c/d/e/f/g/h/i/j");
    ASSERT_EQ(result.size(), 10);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[9], "j");
}

TEST(SplitPathTest, BackslashPath) {
    const auto result = nexo::splitPath("foo\\bar\\baz");
#ifdef _WIN32
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "foo");
    EXPECT_EQ(result[1], "bar");
    EXPECT_EQ(result[2], "baz");
#else
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "foo\\bar\\baz");
#endif
}

// ============================================================================
// Path Fixture - Extension Operations
// ============================================================================

TEST_F(PathTestFixture, ExtensionExtraction) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("file.txt");
    EXPECT_EQ(resolved.extension().string(), ".txt");
}

TEST_F(PathTestFixture, MultipleExtensions) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("archive.tar.gz");
    EXPECT_EQ(resolved.extension().string(), ".gz");
}

TEST_F(PathTestFixture, NoExtension) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("README");
    EXPECT_TRUE(resolved.extension().empty());
}

TEST_F(PathTestFixture, HiddenFileAsExtension) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe(".gitignore");
    EXPECT_EQ(resolved.filename().string(), ".gitignore");
}

// ============================================================================
// Path Fixture - Parent Directory Operations
// ============================================================================

TEST_F(PathTestFixture, ParentOfNestedPath) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("a/b/c/file.txt");
    EXPECT_EQ(resolved.parent_path().filename().string(), "c");
}

TEST_F(PathTestFixture, ParentOfSingleFile) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("file.txt");
    const auto exePath = nexo::Path::getExecutablePath();
    EXPECT_EQ(resolved.parent_path(), exePath.parent_path());
}

TEST_F(PathTestFixture, MultipleParentCalls) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("a/b/c/d/file.txt");
    auto parent = resolved.parent_path();
    parent = parent.parent_path();
    parent = parent.parent_path();
    EXPECT_EQ(parent.filename().string(), "b");
}

// ============================================================================
// Path Fixture - Path Concatenation
// ============================================================================

TEST_F(PathTestFixture, ConcatenateMultiplePaths) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto base = exePath.parent_path();
    const auto concatenated = base / "assets" / "textures" / "image.png";
    EXPECT_TRUE(concatenated.string().find("assets") != std::string::npos);
    EXPECT_TRUE(concatenated.string().find("textures") != std::string::npos);
    EXPECT_TRUE(concatenated.string().find("image.png") != std::string::npos);
}

TEST_F(PathTestFixture, ConcatenateEmptyPath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto result = exePath / "";
    // When concatenating empty path, it may add a trailing slash
    EXPECT_TRUE(result == exePath || result == exePath.string() + "/");
}

TEST_F(PathTestFixture, ConcatenateWithDotDot) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("foo/../bar/file.txt");
    EXPECT_TRUE(resolved.string().find("bar") != std::string::npos);
}

// ============================================================================
// Path Fixture - Absolute vs Relative Paths
// ============================================================================

TEST_F(PathTestFixture, ResolveRelativePathIsAbsolute) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("relative/path.txt");
    EXPECT_TRUE(resolved.is_absolute());
}

TEST_F(PathTestFixture, ExecutablePathIsAbsolute) {
    const auto exePath = nexo::Path::getExecutablePath();
    EXPECT_TRUE(exePath.is_absolute());
}

TEST_F(PathTestFixture, ResolveAlreadyAbsolutePath) {
#ifdef _WIN32
    const auto resolved = nexo::Path::resolvePathRelativeToExe("C:/absolute/path.txt");
    EXPECT_EQ(resolved.string(), "C:/absolute/path.txt");
#else
    const auto resolved = nexo::Path::resolvePathRelativeToExe("/absolute/path.txt");
    EXPECT_EQ(resolved.string(), "/absolute/path.txt");
#endif
}

// ============================================================================
// Path Fixture - Special Characters and Unicode
// ============================================================================

TEST_F(PathTestFixture, PathWithSpaces) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("My Documents/My Files/file.txt");
    EXPECT_TRUE(resolved.string().find("My Documents") != std::string::npos);
    EXPECT_TRUE(resolved.string().find("My Files") != std::string::npos);
}

TEST_F(PathTestFixture, PathWithSpecialCharacters) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("file-name_123@test.txt");
    EXPECT_TRUE(resolved.string().find("file-name_123@test.txt") != std::string::npos);
}

TEST_F(PathTestFixture, PathWithParentheses) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("Program Files (x86)/app/file.dll");
    EXPECT_TRUE(resolved.string().find("Program Files (x86)") != std::string::npos);
}

TEST_F(PathTestFixture, PathWithUnicodeCharacters) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("文档/ファイル/документ.txt");
    EXPECT_FALSE(resolved.empty());
}

TEST_F(PathTestFixture, PathWithEmoji) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("folder/file_😀.txt");
    EXPECT_FALSE(resolved.empty());
}

// ============================================================================
// Path Fixture - Very Long Paths
// ============================================================================

TEST_F(PathTestFixture, VeryLongPath) {
    std::string longPath = "a/";
    for (int i = 0; i < 50; ++i) {
        longPath += "very_long_directory_name_component_" + std::to_string(i) + "/";
    }
    longPath += "file.txt";

    const auto resolved = nexo::Path::resolvePathRelativeToExe(longPath);
    EXPECT_FALSE(resolved.empty());
    EXPECT_TRUE(resolved.string().find("file.txt") != std::string::npos);
}

TEST_F(PathTestFixture, VeryLongFilename) {
    std::string longFilename(200, 'a');
    longFilename += ".txt";

    const auto resolved = nexo::Path::resolvePathRelativeToExe(longFilename);
    EXPECT_FALSE(resolved.empty());
    EXPECT_TRUE(resolved.string().find(".txt") != std::string::npos);
}

// ============================================================================
// normalizePathAndRemovePrefixSlash - Extended Edge Cases
// ============================================================================

TEST(NormalizePathTest, EmptyStringAfterNormalization) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/."), "");
}

TEST(NormalizePathTest, MultipleConsecutiveDots) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/.../bar"), "foo/.../bar");
}

TEST(NormalizePathTest, PathWithOnlyDotDots) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("../../.."), "../../..");
}

TEST(NormalizePathTest, DotDotAtStart) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("../foo/bar"), "../foo/bar");
}

TEST(NormalizePathTest, DotDotAtEnd) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo/bar/.."), "foo");
}

TEST(NormalizePathTest, MixedDotAndDotDot) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/./foo/../bar/./baz"), "bar/baz");
}

TEST(NormalizePathTest, PathWithUnicode) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/folder/文档/file.txt"), "folder/文档/file.txt");
}

TEST(NormalizePathTest, PathWithEmoji) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/folder/😀/file.txt"), "folder/😀/file.txt");
}

TEST(NormalizePathTest, PathWithTabs) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/foo\t/bar"), "foo\t/bar");
}

TEST(NormalizePathTest, PathWithNewlines) {
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("/foo\n/bar");
    EXPECT_TRUE(result.find("foo") != std::string::npos);
}

TEST(NormalizePathTest, VeryLongPath) {
    std::string longPath = "/";
    for (int i = 0; i < 100; ++i) {
        longPath += "dir" + std::to_string(i) + "/";
    }
    longPath += "file.txt";

    const std::string result = nexo::normalizePathAndRemovePrefixSlash(longPath);
    EXPECT_FALSE(result.empty());
    EXPECT_TRUE(result.find("file.txt") != std::string::npos);
    EXPECT_TRUE(result[0] != '/');
}

TEST(NormalizePathTest, PathWithMultipleFileExtensions) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/path/to/file.tar.gz.bak"), "path/to/file.tar.gz.bak");
}

TEST(NormalizePathTest, PathWithNoExtension) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/path/to/README"), "path/to/README");
}

TEST(NormalizePathTest, PathWithDotInDirectoryName) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/path/to/v1.2.3/file"), "path/to/v1.2.3/file");
}

TEST(NormalizePathTest, ComplexRelativePath) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("./foo/../bar/./baz/../qux"), "bar/qux");
}

TEST(NormalizePathTest, WindowsStyleDriveLetter) {
#ifdef _WIN32
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("C:/Users/test/file.txt");
    EXPECT_TRUE(result.find("file.txt") != std::string::npos);
#else
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("C:/Users/test/file.txt"), "C:/Users/test/file.txt");
#endif
}

TEST(NormalizePathTest, UNCPath) {
#ifdef _WIN32
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("//server/share/file.txt");
    EXPECT_FALSE(result.empty());
#else
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("//server/share/file.txt"), "server/share/file.txt");
#endif
}

TEST(NormalizePathTest, PathWithQuotes) {
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("/path/\"quoted\"/file.txt");
    EXPECT_TRUE(result.find("file.txt") != std::string::npos);
}

TEST(NormalizePathTest, PathWithAmpersand) {
    EXPECT_EQ(nexo::normalizePathAndRemovePrefixSlash("/path/foo&bar/file.txt"), "path/foo&bar/file.txt");
}

TEST(NormalizePathTest, PathWithPipe) {
    const std::string result = nexo::normalizePathAndRemovePrefixSlash("/path/foo|bar/file.txt");
    EXPECT_TRUE(result.find("file.txt") != std::string::npos);
}

// ============================================================================
// splitPath - Extended Edge Cases
// ============================================================================

TEST(SplitPathTest, EmptyComponents) {
    const auto result = nexo::splitPath("foo//bar");
    EXPECT_GE(result.size(), 2);
    EXPECT_EQ(result[0], "foo");
}

TEST(SplitPathTest, VeryLongPath) {
    std::string longPath = "";
    for (int i = 0; i < 100; ++i) {
        longPath += "dir" + std::to_string(i) + "/";
    }
    longPath += "file.txt";

    const auto result = nexo::splitPath(longPath);
    EXPECT_EQ(result.size(), 101);
    EXPECT_EQ(result[result.size() - 1], "file.txt");
}

TEST(SplitPathTest, PathWithUnicode) {
    const auto result = nexo::splitPath("folder/文档/ファイル.txt");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "folder");
    EXPECT_EQ(result[1], "文档");
    EXPECT_EQ(result[2], "ファイル.txt");
}

TEST(SplitPathTest, PathWithEmoji) {
    const auto result = nexo::splitPath("folder/😀/file.txt");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "folder");
    EXPECT_EQ(result[1], "😀");
    EXPECT_EQ(result[2], "file.txt");
}

TEST(SplitPathTest, OnlyDots) {
    const auto result = nexo::splitPath("./././.");
    EXPECT_TRUE(std::all_of(result.begin(), result.end(),
        [](const std::string& s) { return s == "."; }));
}

TEST(SplitPathTest, OnlyDoubleDots) {
    const auto result = nexo::splitPath("../../..");
    EXPECT_TRUE(std::all_of(result.begin(), result.end(),
        [](const std::string& s) { return s == ".."; }));
}

TEST(SplitPathTest, AlternatingDotsDotDots) {
    const auto result = nexo::splitPath("./../..");
    ASSERT_GE(result.size(), 3);
    EXPECT_EQ(result[0], ".");
    EXPECT_EQ(result[1], "..");
    EXPECT_EQ(result[2], "..");
}

TEST(SplitPathTest, FilenameThatIsJustDot) {
    const auto result = nexo::splitPath("folder/.");
    EXPECT_TRUE(result.size() >= 1);
    EXPECT_EQ(result[0], "folder");
}

TEST(SplitPathTest, FilenameThatIsJustDotDot) {
    const auto result = nexo::splitPath("folder/..");
    EXPECT_TRUE(result.size() >= 2);
    EXPECT_EQ(result[0], "folder");
    EXPECT_EQ(result[1], "..");
}

TEST(SplitPathTest, PathWithParentheses) {
    const auto result = nexo::splitPath("Program Files (x86)/app/file.dll");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "Program Files (x86)");
    EXPECT_EQ(result[1], "app");
    EXPECT_EQ(result[2], "file.dll");
}

TEST(SplitPathTest, PathWithBrackets) {
    const auto result = nexo::splitPath("folder[1]/file.txt");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "folder[1]");
    EXPECT_EQ(result[1], "file.txt");
}

TEST(SplitPathTest, PathWithCurlyBraces) {
    const auto result = nexo::splitPath("folder{test}/file.txt");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "folder{test}");
    EXPECT_EQ(result[1], "file.txt");
}

TEST(SplitPathTest, VeryLongFilename) {
    std::string longFilename(250, 'a');
    longFilename += ".txt";
    const auto result = nexo::splitPath("folder/" + longFilename);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "folder");
    EXPECT_EQ(result[1], longFilename);
}

TEST(SplitPathTest, SingleCharacterComponents) {
    const auto result = nexo::splitPath("a/b/c/d/e/f");
    ASSERT_EQ(result.size(), 6);
    EXPECT_EQ(result[0], "a");
    EXPECT_EQ(result[5], "f");
}

TEST(SplitPathTest, NumbersOnlyComponents) {
    const auto result = nexo::splitPath("123/456/789");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "123");
    EXPECT_EQ(result[1], "456");
    EXPECT_EQ(result[2], "789");
}

TEST(SplitPathTest, MixedCaseComponents) {
    const auto result = nexo::splitPath("CamelCase/UPPERCASE/lowercase");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "CamelCase");
    EXPECT_EQ(result[1], "UPPERCASE");
    EXPECT_EQ(result[2], "lowercase");
}

TEST(SplitPathTest, PathWithAmpersand) {
    const auto result = nexo::splitPath("foo&bar/file.txt");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "foo&bar");
    EXPECT_EQ(result[1], "file.txt");
}

TEST(SplitPathTest, PathWithPlusSign) {
    const auto result = nexo::splitPath("C++/boost/file.hpp");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "C++");
    EXPECT_EQ(result[1], "boost");
    EXPECT_EQ(result[2], "file.hpp");
}

TEST(SplitPathTest, PathWithEquals) {
    const auto result = nexo::splitPath("key=value/file.txt");
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "key=value");
    EXPECT_EQ(result[1], "file.txt");
}

TEST(SplitPathTest, WindowsDriveLetter) {
#ifdef _WIN32
    const auto result = nexo::splitPath("C:/Users/test/file.txt");
    EXPECT_TRUE(result.size() >= 3);
    EXPECT_EQ(result[result.size() - 1], "file.txt");
#else
    const auto result = nexo::splitPath("C:/Users/test/file.txt");
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "C:");
    EXPECT_EQ(result[1], "Users");
#endif
}

// ============================================================================
// Path Fixture - Root Path Operations
// ============================================================================

TEST_F(PathTestFixture, RootNameOfExecutablePath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto rootName = exePath.root_name();
#ifdef _WIN32
    EXPECT_FALSE(rootName.empty());
#else
    EXPECT_TRUE(rootName.empty());
#endif
}

TEST_F(PathTestFixture, RootDirectoryOfExecutablePath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto rootDir = exePath.root_directory();
#ifdef _WIN32
    EXPECT_EQ(rootDir.string(), "\\");
#else
    EXPECT_EQ(rootDir.string(), "/");
#endif
}

TEST_F(PathTestFixture, RootPathOfExecutablePath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto rootPath = exePath.root_path();
    EXPECT_FALSE(rootPath.empty());
}

TEST_F(PathTestFixture, RelativePathFromRoot) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto relative = exePath.relative_path();
    EXPECT_FALSE(relative.empty());
}

// ============================================================================
// Path Fixture - Filename Operations
// ============================================================================

TEST_F(PathTestFixture, FilenameExtraction) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("folder/subfolder/file.txt");
    EXPECT_EQ(resolved.filename().string(), "file.txt");
}

TEST_F(PathTestFixture, StemExtraction) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("folder/file.txt");
    EXPECT_EQ(resolved.stem().string(), "file");
}

TEST_F(PathTestFixture, StemWithMultipleExtensions) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("archive.tar.gz");
    EXPECT_EQ(resolved.stem().string(), "archive.tar");
}

TEST_F(PathTestFixture, EmptyFilename) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("folder/subfolder/");
    const auto filename = resolved.filename();
    EXPECT_TRUE(filename.empty() || filename == ".");
}

// ============================================================================
// Path Fixture - Lexical Operations
// ============================================================================

TEST_F(PathTestFixture, LexicallyNormalPath) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("a/./b/../c");
    EXPECT_TRUE(resolved.string().find("/c") != std::string::npos ||
                resolved.string().find("\\c") != std::string::npos);
}

TEST_F(PathTestFixture, LexicallyRelativePath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto base = exePath.parent_path();
    const auto target = base / "folder" / "file.txt";
    const auto relative = target.lexically_relative(base);

    EXPECT_FALSE(relative.empty());
    EXPECT_TRUE(relative.is_relative());
}

TEST_F(PathTestFixture, LexicallyProximate) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto base = exePath.parent_path();
    const auto target = base / "folder" / "file.txt";
    const auto proximate = target.lexically_proximate(base);

    EXPECT_FALSE(proximate.empty());
}

// ============================================================================
// Path Fixture - Edge Cases with Empty Paths
// ============================================================================

TEST_F(PathTestFixture, ResolveEmptyPath) {
    const auto exePath = nexo::Path::getExecutablePath();
    const auto resolved = nexo::Path::resolvePathRelativeToExe("");
    // When resolving empty path, lexically_normal may add a trailing slash
    const auto parent = exePath.parent_path();
    EXPECT_TRUE(resolved == parent || resolved == parent.string() + "/");
}

TEST_F(PathTestFixture, ConcatenateWithSlashOnly) {
    const auto resolved = nexo::Path::resolvePathRelativeToExe("/");
#ifdef _WIN32
    EXPECT_EQ(resolved.string(), "/");
#else
    EXPECT_EQ(resolved.string(), "/");
#endif
}

// ============================================================================
// Path Fixture - Comparison Operations
// ============================================================================

TEST_F(PathTestFixture, CompareSamePaths) {
    const auto path1 = nexo::Path::resolvePathRelativeToExe("file.txt");
    const auto path2 = nexo::Path::resolvePathRelativeToExe("file.txt");
    EXPECT_EQ(path1, path2);
}

TEST_F(PathTestFixture, CompareDifferentPaths) {
    const auto path1 = nexo::Path::resolvePathRelativeToExe("file1.txt");
    const auto path2 = nexo::Path::resolvePathRelativeToExe("file2.txt");
    EXPECT_NE(path1, path2);
}

TEST_F(PathTestFixture, CompareNormalizedPaths) {
    const auto path1 = nexo::Path::resolvePathRelativeToExe("a/./b/../c/file.txt");
    const auto path2 = nexo::Path::resolvePathRelativeToExe("a/c/file.txt");
    EXPECT_EQ(path1, path2);
}
