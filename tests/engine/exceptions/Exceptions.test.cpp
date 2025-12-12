//// Exceptions.test.cpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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

    TEST(ExceptionsTest, TooManyPointLightsException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        TooManyPointLightsException ex(1, 15);  // sceneRendered=1, nbPointLights=15
        std::string formattedMessage = ex.what();
        std::cout << "Formatted message: " << formattedMessage << std::endl;

        EXPECT_NE(formattedMessage.find("Too many point lights"), std::string::npos);
        EXPECT_NE(formattedMessage.find("15"), std::string::npos);  // light count
        EXPECT_NE(formattedMessage.find(std::to_string(MAX_POINT_LIGHTS)), std::string::npos);  // max lights
        EXPECT_NE(formattedMessage.find("[1]"), std::string::npos);  // scene id in brackets
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, TooManyPointLightsException_ExactlyAtMax) {
        // Test with exactly MAX_POINT_LIGHTS + 1 to verify boundary behavior
        TooManyPointLightsException ex(0, MAX_POINT_LIGHTS + 1);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Too many point lights"), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(MAX_POINT_LIGHTS + 1)), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        TooManySpotLightsException ex(2, 20);  // sceneRendered=2, nbSpotLights=20
        std::string formattedMessage = ex.what();
        std::cout << "Formatted message: " << formattedMessage << std::endl;

        EXPECT_NE(formattedMessage.find("Too many spot lights"), std::string::npos);
        EXPECT_NE(formattedMessage.find("20"), std::string::npos);  // light count
        EXPECT_NE(formattedMessage.find(std::to_string(MAX_SPOT_LIGHTS)), std::string::npos);  // max lights
        EXPECT_NE(formattedMessage.find("[2]"), std::string::npos);  // scene id in brackets
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_ExactlyAtMax) {
        // Test with exactly MAX_SPOT_LIGHTS + 1 to verify boundary behavior
        TooManySpotLightsException ex(5, MAX_SPOT_LIGHTS + 1);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Too many spot lights"), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(MAX_SPOT_LIGHTS + 1)), std::string::npos);
    }
}
