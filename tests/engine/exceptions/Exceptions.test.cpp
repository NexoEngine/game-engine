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

    // ==================== Inheritance Tests ====================

    TEST(ExceptionsTest, FileNotFoundException_IsStdException) {
        FileNotFoundException ex("test.txt");
        std::exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, LoadModelException_IsStdException) {
        LoadModelException ex("model.obj", "error");
        std::exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_IsStdException) {
        SceneManagerLifecycleException ex("lifecycle error");
        std::exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, TooManyPointLightsException_IsStdException) {
        TooManyPointLightsException ex(0, 15);
        std::exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_IsStdException) {
        TooManySpotLightsException ex(0, 15);
        std::exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, FileNotFoundException_IsNexoException) {
        FileNotFoundException ex("test.txt");
        nexo::Exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    TEST(ExceptionsTest, LoadModelException_IsNexoException) {
        LoadModelException ex("model.obj", "error");
        nexo::Exception* basePtr = &ex;
        EXPECT_NE(basePtr, nullptr);
        EXPECT_NE(basePtr->what(), nullptr);
    }

    // ==================== Catch as std::exception Tests ====================

    TEST(ExceptionsTest, CatchFileNotFoundAsStdException) {
        try {
            throw FileNotFoundException("missing.txt");
        } catch (const std::exception& e) {
            std::string msg = e.what();
            EXPECT_NE(msg.find("File not found: missing.txt"), std::string::npos);
        } catch (...) {
            FAIL() << "Should be caught as std::exception";
        }
    }

    TEST(ExceptionsTest, CatchLoadModelAsStdException) {
        try {
            throw LoadModelException("model.fbx", "Parse error");
        } catch (const std::exception& e) {
            std::string msg = e.what();
            EXPECT_NE(msg.find("Failure to load model"), std::string::npos);
            EXPECT_NE(msg.find("model.fbx"), std::string::npos);
            EXPECT_NE(msg.find("Parse error"), std::string::npos);
        } catch (...) {
            FAIL() << "Should be caught as std::exception";
        }
    }

    TEST(ExceptionsTest, CatchSceneManagerLifecycleAsStdException) {
        try {
            throw SceneManagerLifecycleException("Invalid state transition");
        } catch (const std::exception& e) {
            std::string msg = e.what();
            EXPECT_NE(msg.find("Invalid state transition"), std::string::npos);
        } catch (...) {
            FAIL() << "Should be caught as std::exception";
        }
    }

    TEST(ExceptionsTest, CatchTooManyPointLightsAsStdException) {
        try {
            throw TooManyPointLightsException(3, 25);
        } catch (const std::exception& e) {
            std::string msg = e.what();
            EXPECT_NE(msg.find("Too many point lights"), std::string::npos);
            EXPECT_NE(msg.find("25"), std::string::npos);
        } catch (...) {
            FAIL() << "Should be caught as std::exception";
        }
    }

    TEST(ExceptionsTest, CatchTooManySpotLightsAsStdException) {
        try {
            throw TooManySpotLightsException(7, 30);
        } catch (const std::exception& e) {
            std::string msg = e.what();
            EXPECT_NE(msg.find("Too many spot lights"), std::string::npos);
            EXPECT_NE(msg.find("30"), std::string::npos);
        } catch (...) {
            FAIL() << "Should be caught as std::exception";
        }
    }

    // ==================== Edge Cases: Empty Messages ====================

    TEST(ExceptionsTest, FileNotFoundException_EmptyPath) {
        FileNotFoundException ex("");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("File not found: "), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_EmptyPath) {
        LoadModelException ex("", "Some error");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("Failure to load model"), std::string::npos);
        EXPECT_NE(formattedMessage.find("Some error"), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_EmptyError) {
        LoadModelException ex("model.dae", "");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("Failure to load model"), std::string::npos);
        EXPECT_NE(formattedMessage.find("model.dae"), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_BothEmpty) {
        LoadModelException ex("", "");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("Failure to load model"), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_EmptyMessage) {
        SceneManagerLifecycleException ex("");
        std::string formattedMessage = ex.what();
        // Should still contain file and line information
        EXPECT_FALSE(formattedMessage.empty());
    }

    // ==================== Edge Cases: Special Characters ====================

    TEST(ExceptionsTest, FileNotFoundException_PathWithSpaces) {
        FileNotFoundException ex("path with spaces/file.txt");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("File not found: path with spaces/file.txt"), std::string::npos);
    }

    TEST(ExceptionsTest, FileNotFoundException_PathWithSpecialChars) {
        FileNotFoundException ex("path/to/@file-name_v1.0.txt");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("File not found: path/to/@file-name_v1.0.txt"), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_PathWithUnicode) {
        LoadModelException ex("modèle.fbx", "Erreur de chargement");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("modèle.fbx"), std::string::npos);
        EXPECT_NE(formattedMessage.find("Erreur de chargement"), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_MessageWithNewlines) {
        SceneManagerLifecycleException ex("Error on line 1\nError on line 2");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("Error on line 1"), std::string::npos);
        EXPECT_NE(formattedMessage.find("Error on line 2"), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_MessageWithQuotes) {
        SceneManagerLifecycleException ex("Error: \"coordinator\" not initialized");
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("\"coordinator\""), std::string::npos);
    }

    // ==================== Source Location Tracking ====================

    TEST(ExceptionsTest, FileNotFoundException_SourceLocationAccurate) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FileNotFoundException ex("test.txt");

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_SourceLocationAccurate) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        LoadModelException ex("model.obj", "error");

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_SourceLocationAccurate) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        SceneManagerLifecycleException ex("message");

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, TooManyPointLightsException_SourceLocationAccurate) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TooManyPointLightsException ex(0, 15);

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_SourceLocationAccurate) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TooManySpotLightsException ex(0, 15);

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    // ==================== Light Count Formatting Tests ====================

    TEST(ExceptionsTest, TooManyPointLightsException_ZeroLights) {
        TooManyPointLightsException ex(0, 0);
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("(0 > " + std::to_string(MAX_POINT_LIGHTS) + ")"), std::string::npos);
        EXPECT_NE(formattedMessage.find("[0]"), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_ZeroLights) {
        TooManySpotLightsException ex(0, 0);
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("(0 > " + std::to_string(MAX_SPOT_LIGHTS) + ")"), std::string::npos);
        EXPECT_NE(formattedMessage.find("[0]"), std::string::npos);
    }

    TEST(ExceptionsTest, TooManyPointLightsException_LargeLightCount) {
        TooManyPointLightsException ex(999, 9999);
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("9999"), std::string::npos);
        EXPECT_NE(formattedMessage.find("[999]"), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_LargeLightCount) {
        TooManySpotLightsException ex(999, 9999);
        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find("9999"), std::string::npos);
        EXPECT_NE(formattedMessage.find("[999]"), std::string::npos);
    }

    TEST(ExceptionsTest, TooManyPointLightsException_MultipleScenes) {
        TooManyPointLightsException ex1(1, 15);
        TooManyPointLightsException ex2(2, 20);
        TooManyPointLightsException ex3(3, 25);

        std::string msg1 = ex1.what();
        std::string msg2 = ex2.what();
        std::string msg3 = ex3.what();

        EXPECT_NE(msg1.find("[1]"), std::string::npos);
        EXPECT_NE(msg2.find("[2]"), std::string::npos);
        EXPECT_NE(msg3.find("[3]"), std::string::npos);

        EXPECT_NE(msg1.find("15"), std::string::npos);
        EXPECT_NE(msg2.find("20"), std::string::npos);
        EXPECT_NE(msg3.find("25"), std::string::npos);
    }

    TEST(ExceptionsTest, TooManySpotLightsException_MultipleScenes) {
        TooManySpotLightsException ex1(10, 15);
        TooManySpotLightsException ex2(20, 20);
        TooManySpotLightsException ex3(30, 25);

        std::string msg1 = ex1.what();
        std::string msg2 = ex2.what();
        std::string msg3 = ex3.what();

        EXPECT_NE(msg1.find("[10]"), std::string::npos);
        EXPECT_NE(msg2.find("[20]"), std::string::npos);
        EXPECT_NE(msg3.find("[30]"), std::string::npos);

        EXPECT_NE(msg1.find("15"), std::string::npos);
        EXPECT_NE(msg2.find("20"), std::string::npos);
        EXPECT_NE(msg3.find("25"), std::string::npos);
    }

    // ==================== Message Propagation Tests ====================

    TEST(ExceptionsTest, FileNotFoundException_MessagePropagation) {
        const std::string testPath = "/absolute/path/to/missing/file.txt";
        FileNotFoundException ex(testPath);

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(testPath), std::string::npos);
        EXPECT_NE(formattedMessage.find("File not found: "), std::string::npos);
    }

    TEST(ExceptionsTest, LoadModelException_PathAndErrorPropagation) {
        const std::string testPath = "assets/models/character.fbx";
        const std::string testError = "Assimp error: Invalid vertex data";

        LoadModelException ex(testPath, testError);

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(testPath), std::string::npos);
        EXPECT_NE(formattedMessage.find(testError), std::string::npos);
        EXPECT_NE(formattedMessage.find("Failure to load model : "), std::string::npos);
    }

    TEST(ExceptionsTest, SceneManagerLifecycleException_CustomMessagePropagation) {
        const std::string customMessage = "Cannot destroy scene while rendering";
        SceneManagerLifecycleException ex(customMessage);

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(customMessage), std::string::npos);
    }

    // ==================== Multiple Exception Type Tests ====================

    TEST(ExceptionsTest, MultipleExceptionTypes_Sequential) {
        // Test that multiple exception types can be created and used correctly
        FileNotFoundException ex1("file1.txt");
        LoadModelException ex2("model.obj", "error");
        SceneManagerLifecycleException ex3("lifecycle");
        TooManyPointLightsException ex4(0, 15);
        TooManySpotLightsException ex5(0, 20);

        EXPECT_NE(std::string(ex1.what()).find("File not found"), std::string::npos);
        EXPECT_NE(std::string(ex2.what()).find("Failure to load model"), std::string::npos);
        EXPECT_NE(std::string(ex3.what()).find("lifecycle"), std::string::npos);
        EXPECT_NE(std::string(ex4.what()).find("Too many point lights"), std::string::npos);
        EXPECT_NE(std::string(ex5.what()).find("Too many spot lights"), std::string::npos);
    }

    TEST(ExceptionsTest, ExceptionTypes_DistinctMessages) {
        // Verify that different exception types have distinct, non-overlapping core messages
        FileNotFoundException ex1("test.txt");
        LoadModelException ex2("test.txt", "error");
        SceneManagerLifecycleException ex3("test message");

        std::string msg1 = ex1.what();
        std::string msg2 = ex2.what();
        std::string msg3 = ex3.what();

        // FileNotFoundException should have unique identifier
        EXPECT_NE(msg1.find("File not found"), std::string::npos);
        EXPECT_EQ(msg1.find("Failure to load model"), std::string::npos);

        // LoadModelException should have unique identifier
        EXPECT_NE(msg2.find("Failure to load model"), std::string::npos);
        EXPECT_EQ(msg2.find("File not found"), std::string::npos);

        // SceneManagerLifecycleException should just have the message
        EXPECT_EQ(msg3.find("File not found"), std::string::npos);
        EXPECT_EQ(msg3.find("Failure to load model"), std::string::npos);
    }

    // ==================== what() Method Tests ====================

    TEST(ExceptionsTest, WhatMethod_ReturnsNonNull) {
        FileNotFoundException ex("test.txt");
        EXPECT_NE(ex.what(), nullptr);
    }

    TEST(ExceptionsTest, WhatMethod_ReturnsNonEmptyString) {
        LoadModelException ex("model.obj", "error");
        std::string msg = ex.what();
        EXPECT_FALSE(msg.empty());
    }

    TEST(ExceptionsTest, WhatMethod_ConsistentAcrossMultipleCalls) {
        SceneManagerLifecycleException ex("test message");
        std::string msg1 = ex.what();
        std::string msg2 = ex.what();
        std::string msg3 = ex.what();

        EXPECT_EQ(msg1, msg2);
        EXPECT_EQ(msg2, msg3);
    }

    TEST(ExceptionsTest, WhatMethod_ContainsSourceLocation) {
        constexpr const char* expectedFile = __FILE__;
        FileNotFoundException ex("test.txt");

        std::string msg = ex.what();
        EXPECT_NE(msg.find(expectedFile), std::string::npos);
        EXPECT_NE(msg.find("Exception occurred in"), std::string::npos);
    }
}
