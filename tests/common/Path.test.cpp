//// Path.test.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        23/11/2024
//  Description: Tests for the Path class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include "Path.hpp"

TEST(PathTest, ResolvePathRelativeToExe) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "test.txt");
}

TEST(PathTest, ResolvePathRelativeToExeTwice) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("test.txt");
    const std::filesystem::path resolvedPath2 = nexo::Path::resolvePathRelativeToExe("test2.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "test.txt");
    EXPECT_EQ(resolvedPath2, exePath.parent_path() / "test2.txt");
}

TEST(PathTest, ResolvePathRelativeToExeWithParent) {
    const std::filesystem::path exePath = nexo::Path::getExecutablePath();
    const std::filesystem::path resolvedPath = nexo::Path::resolvePathRelativeToExe("assets/test.txt");

    EXPECT_EQ(resolvedPath, exePath.parent_path() / "assets/test.txt");
}
