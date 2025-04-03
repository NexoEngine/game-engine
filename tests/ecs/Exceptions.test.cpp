//// ECSExceptions.test.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        02/12/2024
//  Description: Test file for the ECSExceptions class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/ECSExceptions.hpp"

namespace nexo::ecs {

    TEST(ECSExceptionsTest, ComponentNotFound) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        ComponentNotFound ex(42);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Component not found for: 42"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ECSExceptionsTest, ComponentNotRegistered) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        ComponentNotRegistered ex;
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Component has not been registered before use"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ECSExceptionsTest, SingletonComponentNotRegistered) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        SingletonComponentNotRegistered ex;
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Singleton component has not been registered before use"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ECSExceptionsTest, SystemNotRegistered) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        SystemNotRegistered ex;
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("System has not been registered before use"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ECSExceptionsTest, TooManyEntities) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TooManyEntities ex;
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find(std::format("Too many living entities, max is {}", MAX_ENTITIES)), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(ECSExceptionsTest, OutOfRange) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        OutOfRange ex(256);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Index 256 is out of range"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }
}
