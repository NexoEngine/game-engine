//// Exception.test.cpp ///////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        02/12/2024
//  Description: Test file for the Exception class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Exception.hpp"

namespace nexo {

    TEST(ExceptionTest, BasicExceptionMessage) {
        Exception ex("Test exception message", std::source_location::current());

        EXPECT_EQ(ex.getMessage(), "Test exception message");
        EXPECT_NE(std::string(ex.what()).find("Test exception message"), std::string::npos);
    }

    TEST(ExceptionTest, FormattedMessageContainsFileAndLine) {
        constexpr const char *expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        Exception ex("Formatted message test", std::source_location::current());

        std::string formattedMessage = ex.what();
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
        EXPECT_NE(formattedMessage.find("Formatted message test"), std::string::npos);
    }

    TEST(ExceptionTest, THROW_EXCEPTIONMacro) {
        try {
            THROW_EXCEPTION(Exception, "Macro test exception");
        } catch (const Exception &ex) {
            EXPECT_EQ(ex.getMessage(), "Macro test exception");
            EXPECT_NE(std::string(ex.what()).find("Macro test exception"), std::string::npos);
        } catch (...) {
            FAIL() << "Unexpected exception type thrown";
        }
    }

    TEST(ExceptionTest, DerivedExceptionWithMacro) {
        class DerivedException : public Exception {
        public:
            explicit DerivedException(const std::string &message,
                                      const std::source_location loc = std::source_location::current())
                : Exception(message, loc) {}
        };

        try {
            THROW_EXCEPTION(DerivedException, "Derived exception test");
        } catch (const DerivedException &ex) {
            EXPECT_EQ(ex.getMessage(), "Derived exception test");
            EXPECT_NE(std::string(ex.what()).find("Derived exception test"), std::string::npos);
        } catch (...) {
            FAIL() << "Unexpected exception type thrown";
        }
    }
}
