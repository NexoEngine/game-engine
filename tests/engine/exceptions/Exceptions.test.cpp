//// Exceptions.test.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        02/12/2024
//  Description: Test file for the Exceptions class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "core/exceptions/Exceptions.hpp"

namespace nexo::core {

    TEST(ExceptionsTest, FileNotFoundException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        FileNotFoundException ex("missing_file.txt");
        std::string formattedMessage = ex.what();
        std::cout << "Formatted message: " << formattedMessage << std::endl;

        EXPECT_NE(formattedMessage.find("File not found: missing_file.txt"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        LoadModelException ex("model.obj", "Invalid format");
        std::string formattedMessage = ex.what();
        std::cout << "Formatted message: " << formattedMessage << std::endl;

        EXPECT_NE(formattedMessage.find("Failure to load model : model.obj : Invalid format"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        SceneManagerLifecycleException ex("Coordinator not set before creating scene");
        std::string formattedMessage = ex.what();
        std::cout << "Formatted message: " << formattedMessage << std::endl;

        EXPECT_NE(formattedMessage.find("Coordinator not set before creating scene"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }
}
