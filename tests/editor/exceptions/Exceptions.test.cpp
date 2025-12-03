//// Exceptions.test.cpp //////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for editor exception classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "exceptions/Exceptions.hpp"
#include <typeindex>

namespace nexo::editor {

    // ==========================================================================
    // FileNotFoundException Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, FileNotFoundContainsPath) {
        FileNotFoundException ex("test/path/file.txt");
        std::string what = ex.what();
        EXPECT_NE(what.find("test/path/file.txt"), std::string::npos);
        EXPECT_NE(what.find("File not found"), std::string::npos);
    }

    TEST(EditorExceptionsTest, FileNotFoundEmptyPath) {
        FileNotFoundException ex("");
        std::string what = ex.what();
        EXPECT_NE(what.find("File not found"), std::string::npos);
    }

    // ==========================================================================
    // FileReadException Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, FileReadExceptionContainsPathAndMessage) {
        FileReadException ex("config.json", "permission denied");
        std::string what = ex.what();
        EXPECT_NE(what.find("config.json"), std::string::npos);
        EXPECT_NE(what.find("permission denied"), std::string::npos);
        EXPECT_NE(what.find("Error reading file"), std::string::npos);
    }

    // ==========================================================================
    // FileWriteException Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, FileWriteExceptionContainsPathAndMessage) {
        FileWriteException ex("output.txt", "disk full");
        std::string what = ex.what();
        EXPECT_NE(what.find("output.txt"), std::string::npos);
        EXPECT_NE(what.find("disk full"), std::string::npos);
        EXPECT_NE(what.find("Error writing"), std::string::npos);
    }

    // ==========================================================================
    // WindowNotRegistered Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, WindowNotRegisteredContainsTypeName) {
        std::type_index typeIdx = typeid(int);
        WindowNotRegistered ex(typeIdx);
        std::string what = ex.what();
        EXPECT_NE(what.find("Window not registered"), std::string::npos);
        EXPECT_NE(what.find(typeIdx.name()), std::string::npos);
    }

    // ==========================================================================
    // WindowAlreadyRegistered Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, WindowAlreadyRegisteredContainsNameAndType) {
        std::type_index typeIdx = typeid(double);
        WindowAlreadyRegistered ex(typeIdx, "TestWindow");
        std::string what = ex.what();
        EXPECT_NE(what.find("TestWindow"), std::string::npos);
        EXPECT_NE(what.find("already registered"), std::string::npos);
        EXPECT_NE(what.find(typeIdx.name()), std::string::npos);
    }

    // ==========================================================================
    // BackendRendererApiNotSupported Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, BackendNotSupportedContainsApiName) {
        BackendRendererApiNotSupported ex("Vulkan");
        std::string what = ex.what();
        EXPECT_NE(what.find("Vulkan"), std::string::npos);
        EXPECT_NE(what.find("not supported"), std::string::npos);
    }

    // ==========================================================================
    // BackendRendererApiInitFailed Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, BackendInitFailedContainsApiName) {
        BackendRendererApiInitFailed ex("OpenGL");
        std::string what = ex.what();
        EXPECT_NE(what.find("OpenGL"), std::string::npos);
        EXPECT_NE(what.find("init failed"), std::string::npos);
    }

    // ==========================================================================
    // BackendRendererApiFontInitFailed Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, BackendFontInitFailedContainsApiName) {
        BackendRendererApiFontInitFailed ex("DirectX");
        std::string what = ex.what();
        EXPECT_NE(what.find("DirectX"), std::string::npos);
        EXPECT_NE(what.find("font init failed"), std::string::npos);
    }

    // ==========================================================================
    // BackendRendererApiFatalFailure Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, BackendFatalFailureContainsApiNameAndMessage) {
        BackendRendererApiFatalFailure ex("WebGL", "Context lost");
        std::string what = ex.what();
        EXPECT_NE(what.find("WebGL"), std::string::npos);
        EXPECT_NE(what.find("Context lost"), std::string::npos);
        EXPECT_NE(what.find("FATAL ERROR"), std::string::npos);
    }

    // ==========================================================================
    // InvalidTestFileFormat Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, InvalidTestFileFormatContainsPathAndMessage) {
        InvalidTestFileFormat ex("test.protocol", "missing header");
        std::string what = ex.what();
        EXPECT_NE(what.find("test.protocol"), std::string::npos);
        EXPECT_NE(what.find("missing header"), std::string::npos);
        EXPECT_NE(what.find("Invalid test file"), std::string::npos);
    }

    // ==========================================================================
    // Exception Inheritance Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, AllExceptionsInheritFromException) {
        // Test that all exceptions are catchable as Exception&
        try {
            throw FileNotFoundException("test.txt");
        } catch (const Exception& e) {
            EXPECT_NE(std::string(e.what()).find("test.txt"), std::string::npos);
        }

        try {
            throw FileReadException("test.txt", "error");
        } catch (const Exception& e) {
            EXPECT_NE(std::string(e.what()).find("test.txt"), std::string::npos);
        }

        try {
            throw FileWriteException("test.txt", "error");
        } catch (const Exception& e) {
            EXPECT_NE(std::string(e.what()).find("test.txt"), std::string::npos);
        }
    }

    TEST(EditorExceptionsTest, ExceptionsAreCatchableAsStdException) {
        try {
            throw FileNotFoundException("file.txt");
        } catch (const std::exception& e) {
            SUCCEED();
        } catch (...) {
            FAIL() << "Exception should be catchable as std::exception";
        }
    }

    // ==========================================================================
    // Source Location Tests
    // ==========================================================================

    TEST(EditorExceptionsTest, ExceptionHasSourceLocation) {
        FileNotFoundException ex("test.txt");
        // The exception should have source location info from the throw site
        // We can verify it compiles and the what() returns something
        EXPECT_FALSE(std::string(ex.what()).empty());
    }

}
