//// ECSExceptions.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ECS exception types
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/ECSExceptions.hpp"
#include <string>

namespace nexo::ecs {

// =============================================================================
// InternalError Tests
// =============================================================================

class InternalErrorTest : public ::testing::Test {};

TEST_F(InternalErrorTest, ContainsMessage) {
    try {
        throw InternalError("test message");
    } catch (const InternalError& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("test message"), std::string::npos);
    }
}

TEST_F(InternalErrorTest, ContainsInternalErrorPrefix) {
    try {
        throw InternalError("error details");
    } catch (const InternalError& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("Internal error"), std::string::npos);
    }
}

TEST_F(InternalErrorTest, InheritsFromException) {
    try {
        throw InternalError("test");
    } catch (const Exception& e) {
        SUCCEED();
        return;
    }
    FAIL() << "InternalError should inherit from Exception";
}

// =============================================================================
// ComponentNotFound Tests
// =============================================================================

class ComponentNotFoundTest : public ::testing::Test {};

TEST_F(ComponentNotFoundTest, ContainsEntityId) {
    try {
        throw ComponentNotFound(42);
    } catch (const ComponentNotFound& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("42"), std::string::npos);
    }
}

TEST_F(ComponentNotFoundTest, ContainsNotFoundMessage) {
    try {
        throw ComponentNotFound(100);
    } catch (const ComponentNotFound& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("not found"), std::string::npos);
    }
}

TEST_F(ComponentNotFoundTest, DifferentEntitiesProduceDifferentMessages) {
    std::string msg1, msg2;

    try {
        throw ComponentNotFound(1);
    } catch (const ComponentNotFound& e) {
        msg1 = e.what();
    }

    try {
        throw ComponentNotFound(999);
    } catch (const ComponentNotFound& e) {
        msg2 = e.what();
    }

    EXPECT_NE(msg1, msg2);
}

// =============================================================================
// OverlappingGroupsException Tests
// =============================================================================

class OverlappingGroupsExceptionTest : public ::testing::Test {};

TEST_F(OverlappingGroupsExceptionTest, ContainsExistingGroupName) {
    try {
        throw OverlappingGroupsException("ExistingGroup", "NewGroup", 5);
    } catch (const OverlappingGroupsException& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("ExistingGroup"), std::string::npos);
    }
}

TEST_F(OverlappingGroupsExceptionTest, ContainsNewGroupName) {
    try {
        throw OverlappingGroupsException("ExistingGroup", "NewGroup", 5);
    } catch (const OverlappingGroupsException& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("NewGroup"), std::string::npos);
    }
}

TEST_F(OverlappingGroupsExceptionTest, ContainsComponentType) {
    try {
        throw OverlappingGroupsException("Group1", "Group2", 7);
    } catch (const OverlappingGroupsException& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("7"), std::string::npos);
    }
}

TEST_F(OverlappingGroupsExceptionTest, ContainsOverlappingKeyword) {
    try {
        throw OverlappingGroupsException("A", "B", 0);
    } catch (const OverlappingGroupsException& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("overlapping"), std::string::npos);
    }
}

// =============================================================================
// GroupNotFound Tests
// =============================================================================

class GroupNotFoundTest : public ::testing::Test {};

TEST_F(GroupNotFoundTest, ContainsGroupKey) {
    try {
        throw GroupNotFound("MyGroupKey");
    } catch (const GroupNotFound& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("MyGroupKey"), std::string::npos);
    }
}

TEST_F(GroupNotFoundTest, ContainsNotFoundMessage) {
    try {
        throw GroupNotFound("TestKey");
    } catch (const GroupNotFound& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("not found"), std::string::npos);
    }
}

// =============================================================================
// InvalidGroupComponent Tests
// =============================================================================

class InvalidGroupComponentTest : public ::testing::Test {};

TEST_F(InvalidGroupComponentTest, HasDescriptiveMessage) {
    try {
        throw InvalidGroupComponent();
    } catch (const InvalidGroupComponent& e) {
        std::string what = e.what();
        EXPECT_FALSE(what.empty());
        EXPECT_NE(what.find("group"), std::string::npos);
    }
}

// =============================================================================
// ComponentNotRegistered Tests
// =============================================================================

class ComponentNotRegisteredTest : public ::testing::Test {};

TEST_F(ComponentNotRegisteredTest, HasDescriptiveMessage) {
    try {
        throw ComponentNotRegistered();
    } catch (const ComponentNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("registered"), std::string::npos);
    }
}

TEST_F(ComponentNotRegisteredTest, MentionsComponent) {
    try {
        throw ComponentNotRegistered();
    } catch (const ComponentNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("Component"), std::string::npos);
    }
}

// =============================================================================
// SingletonComponentNotRegistered Tests
// =============================================================================

class SingletonComponentNotRegisteredTest : public ::testing::Test {};

TEST_F(SingletonComponentNotRegisteredTest, HasDescriptiveMessage) {
    try {
        throw SingletonComponentNotRegistered();
    } catch (const SingletonComponentNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("Singleton"), std::string::npos);
    }
}

TEST_F(SingletonComponentNotRegisteredTest, MentionsRegistered) {
    try {
        throw SingletonComponentNotRegistered();
    } catch (const SingletonComponentNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("registered"), std::string::npos);
    }
}

// =============================================================================
// SystemNotRegistered Tests
// =============================================================================

class SystemNotRegisteredTest : public ::testing::Test {};

TEST_F(SystemNotRegisteredTest, HasDescriptiveMessage) {
    try {
        throw SystemNotRegistered();
    } catch (const SystemNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("System"), std::string::npos);
    }
}

TEST_F(SystemNotRegisteredTest, MentionsRegistered) {
    try {
        throw SystemNotRegistered();
    } catch (const SystemNotRegistered& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("registered"), std::string::npos);
    }
}

// =============================================================================
// TooManyEntities Tests
// =============================================================================

class TooManyEntitiesTest : public ::testing::Test {};

TEST_F(TooManyEntitiesTest, ContainsMaxEntitiesValue) {
    try {
        throw TooManyEntities();
    } catch (const TooManyEntities& e) {
        std::string what = e.what();
        EXPECT_NE(what.find(std::to_string(MAX_ENTITIES)), std::string::npos);
    }
}

TEST_F(TooManyEntitiesTest, MentionsTooMany) {
    try {
        throw TooManyEntities();
    } catch (const TooManyEntities& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("Too many"), std::string::npos);
    }
}

// =============================================================================
// OutOfRange Tests
// =============================================================================

class OutOfRangeTest : public ::testing::Test {};

TEST_F(OutOfRangeTest, ContainsIndex) {
    try {
        throw OutOfRange(42);
    } catch (const OutOfRange& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("42"), std::string::npos);
    }
}

TEST_F(OutOfRangeTest, ContainsOutOfRangeMessage) {
    try {
        throw OutOfRange(100);
    } catch (const OutOfRange& e) {
        std::string what = e.what();
        EXPECT_NE(what.find("out of range"), std::string::npos);
    }
}

TEST_F(OutOfRangeTest, DifferentIndicesProduceDifferentMessages) {
    std::string msg1, msg2;

    try {
        throw OutOfRange(1);
    } catch (const OutOfRange& e) {
        msg1 = e.what();
    }

    try {
        throw OutOfRange(999);
    } catch (const OutOfRange& e) {
        msg2 = e.what();
    }

    EXPECT_NE(msg1, msg2);
}

// =============================================================================
// ECS Exception Inheritance Tests
// =============================================================================

class ECSExceptionInheritanceTest : public ::testing::Test {};

TEST_F(ECSExceptionInheritanceTest, AllECSExceptionsInheritFromException) {
    // Test all exceptions can be caught as Exception&
    bool allCaught = true;

    try { throw InternalError("test"); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw ComponentNotFound(0); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw OverlappingGroupsException("a", "b", 0); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw GroupNotFound("key"); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw InvalidGroupComponent(); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw ComponentNotRegistered(); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw SingletonComponentNotRegistered(); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw SystemNotRegistered(); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw TooManyEntities(); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    try { throw OutOfRange(0); } catch (const Exception&) {}
    catch (...) { allCaught = false; }

    EXPECT_TRUE(allCaught);
}

}  // namespace nexo::ecs
