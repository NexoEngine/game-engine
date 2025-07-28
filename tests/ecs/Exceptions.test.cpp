//// ECSExceptionsTest.cpp ///////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        2025-04-09
//  Description: Test file for ECS exceptions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ECSExceptions.hpp"
#include <stdexcept>
#include <type_traits>

namespace nexo::ecs {

	// Base test fixture for exception tests
	class ECSExceptionsTest : public ::testing::Test {
	protected:
	    void SetUp() override {
	        // No specific setup needed
	    }

	    // Helper method to verify exception inheritance
	    template<typename ExceptionType>
	    void verifyExceptionHierarchy() {
	        static_assert(std::is_base_of_v<Exception, ExceptionType>,
	                      "Exception class must inherit from Exception");
	        static_assert(std::is_final_v<ExceptionType>,
	                      "Exception class should be marked final");
	    }

	    // Helper method to check exception message contains expected text
	    template<typename ExceptionType, typename... Args>
	    void verifyExceptionMessage(const std::string& expectedSubstring, Args&&... args) {
	        try {
	            throw ExceptionType(std::forward<Args>(args)...);
	        } catch (const Exception& e) {
	            std::string message = e.what();
	            EXPECT_NE(message.find(expectedSubstring), std::string::npos)
	                << "Exception message '" << message << "' should contain '"
	                << expectedSubstring << "'";
	        } catch (...) {
	            FAIL() << "Exception was not caught as Exception base class";
	        }
	    }
	};

	// Test InternalError exception
	TEST_F(ECSExceptionsTest, InternalErrorTest) {
	    verifyExceptionHierarchy<InternalError>();

	    const std::string errorMsg = "Something bad happened";
	    verifyExceptionMessage<InternalError>(errorMsg, errorMsg);
	    verifyExceptionMessage<InternalError>("Internal error", errorMsg);

	    // Test polymorphic catching
	    try {
	        throw InternalError("Test error");
	        FAIL() << "Exception was not thrown";
	    } catch (const InternalError& e) {
	        SUCCEED();
	    } catch (...) {
	        FAIL() << "Wrong exception type caught";
	    }
	}

	// Test ComponentNotFound exception
	TEST_F(ECSExceptionsTest, ComponentNotFoundTest) {
	    verifyExceptionHierarchy<ComponentNotFound>();

	    const Entity testEntity = 42;
	    verifyExceptionMessage<ComponentNotFound>(std::to_string(testEntity), testEntity);
	    verifyExceptionMessage<ComponentNotFound>("Component not found", testEntity);

	    // Test with different entity values
	    verifyExceptionMessage<ComponentNotFound>("0", Entity(0));
	    verifyExceptionMessage<ComponentNotFound>(std::to_string(MAX_ENTITIES-1), MAX_ENTITIES-1);
	}

	// Test OverlappingGroupsException exception
	TEST_F(ECSExceptionsTest, OverlappingGroupsExceptionTest) {
	    verifyExceptionHierarchy<OverlappingGroupsException>();

	    const std::string existingGroup = "Group1";
	    const std::string newGroup = "Group2";
	    const ComponentType conflictComponent = 5;

	    verifyExceptionMessage<OverlappingGroupsException>(existingGroup,
	        existingGroup, newGroup, conflictComponent);
	    verifyExceptionMessage<OverlappingGroupsException>(newGroup,
	        existingGroup, newGroup, conflictComponent);
	    verifyExceptionMessage<OverlappingGroupsException>(std::to_string(conflictComponent),
	        existingGroup, newGroup, conflictComponent);
	    verifyExceptionMessage<OverlappingGroupsException>("overlapping owned component",
	        existingGroup, newGroup, conflictComponent);

	    // Test with different component types
	    verifyExceptionMessage<OverlappingGroupsException>("component #0",
	        "GroupA", "GroupB", ComponentType(0));
	    verifyExceptionMessage<OverlappingGroupsException>("component #31",
	        "GroupX", "GroupY", ComponentType(31));
	}

	// Test GroupNotFound exception
	TEST_F(ECSExceptionsTest, GroupNotFoundTest) {
	    verifyExceptionHierarchy<GroupNotFound>();

	    const std::string groupKey = "TestGroup";
	    verifyExceptionMessage<GroupNotFound>(groupKey, groupKey);
	    verifyExceptionMessage<GroupNotFound>("Group not found", groupKey);

	    // Test with empty key
	    verifyExceptionMessage<GroupNotFound>("", "");
	}

	// Test ComponentNotRegistered exception
	TEST_F(ECSExceptionsTest, ComponentNotRegisteredTest) {
	    verifyExceptionHierarchy<ComponentNotRegistered>();

	    verifyExceptionMessage<ComponentNotRegistered>("Component has not been registered");

	    // Test that no parameters are needed
	    try {
	        throw ComponentNotRegistered();
	        FAIL() << "Exception was not thrown";
	    } catch (const ComponentNotRegistered&) {
	        SUCCEED();
	    } catch (...) {
	        FAIL() << "Wrong exception type caught";
	    }
	}

	// Test SingletonComponentNotRegistered exception
	TEST_F(ECSExceptionsTest, SingletonComponentNotRegisteredTest) {
	    verifyExceptionHierarchy<SingletonComponentNotRegistered>();

	    verifyExceptionMessage<SingletonComponentNotRegistered>("Singleton component");
	    verifyExceptionMessage<SingletonComponentNotRegistered>("not been registered");

	    // Make sure it's distinct from ComponentNotRegistered
	    try {
	        throw SingletonComponentNotRegistered();
	    } catch (const ComponentNotRegistered&) {
	        FAIL() << "SingletonComponentNotRegistered should not be caught as ComponentNotRegistered";
	    } catch (const SingletonComponentNotRegistered&) {
	        SUCCEED();
	    } catch (...) {
	        FAIL() << "Wrong exception type caught";
	    }
	}

	// Test SystemNotRegistered exception
	TEST_F(ECSExceptionsTest, SystemNotRegisteredTest) {
	    verifyExceptionHierarchy<SystemNotRegistered>();

	    verifyExceptionMessage<SystemNotRegistered>("System has not been registered");
	}

	// Test TooManyEntities exception
	TEST_F(ECSExceptionsTest, TooManyEntitiesTest) {
	    verifyExceptionHierarchy<TooManyEntities>();

	    verifyExceptionMessage<TooManyEntities>("Too many living entities");
	    verifyExceptionMessage<TooManyEntities>(std::to_string(MAX_ENTITIES));
	}

	// Test OutOfRange exception
	TEST_F(ECSExceptionsTest, OutOfRangeTest) {
	    verifyExceptionHierarchy<OutOfRange>();

	    const unsigned int testIndex = 999;
	    verifyExceptionMessage<OutOfRange>(std::to_string(testIndex), testIndex);
	    verifyExceptionMessage<OutOfRange>("out of range", testIndex);

	    // Test with different index values
	    verifyExceptionMessage<OutOfRange>("0", 0u);
	    verifyExceptionMessage<OutOfRange>(std::to_string(UINT_MAX), UINT_MAX);
	}

	// Test that all exceptions can be caught polymorphically as Exception
	TEST_F(ECSExceptionsTest, PolymorphicExceptionHandlingTest) {
	    int caughtCount = 0;

	    try {
	        // Randomly choose one exception to throw
	        int choice = rand() % 8;
	        switch (choice) {
	            case 0: throw InternalError("Test");
	            case 1: throw ComponentNotFound(5);
	            case 2: throw OverlappingGroupsException("G1", "G2", 3);
	            case 3: throw GroupNotFound("Key");
	            case 4: throw ComponentNotRegistered();
	            case 5: throw SingletonComponentNotRegistered();
	            case 6: throw SystemNotRegistered();
	            case 7: throw TooManyEntities();
	            default: throw OutOfRange(10);
	        }
	    } catch (const Exception& e) {
	        // All exceptions should be caught here
	        caughtCount++;
	    } catch (...) {
	        FAIL() << "Exception not caught polymorphically";
	    }

	    EXPECT_EQ(caughtCount, 1) << "Exception should be caught exactly once";
	}
}
