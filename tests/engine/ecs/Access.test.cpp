//// Access.test.cpp ///////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ECS Access type helpers
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/Access.hpp"

namespace nexo::ecs {

// =============================================================================
// Test Component Types
// =============================================================================

struct Position {
    float x, y, z;
};

struct Velocity {
    float vx, vy, vz;
};

struct Health {
    int current;
    int max;
};

// =============================================================================
// AccessType Enum Tests
// =============================================================================

class AccessTypeEnumTest : public ::testing::Test {};

TEST_F(AccessTypeEnumTest, ReadValue) {
    EXPECT_EQ(static_cast<int>(AccessType::Read), 0);
}

TEST_F(AccessTypeEnumTest, WriteValue) {
    EXPECT_EQ(static_cast<int>(AccessType::Write), 1);
}

TEST_F(AccessTypeEnumTest, ValuesDistinct) {
    EXPECT_NE(AccessType::Read, AccessType::Write);
}

// =============================================================================
// ComponentAccess Tests
// =============================================================================

class ComponentAccessTest : public ::testing::Test {};

TEST_F(ComponentAccessTest, ReadAccessType) {
    using ReadPos = ComponentAccess<Position, AccessType::Read>;
    EXPECT_EQ(ReadPos::accessType, AccessType::Read);
}

TEST_F(ComponentAccessTest, WriteAccessType) {
    using WritePos = ComponentAccess<Position, AccessType::Write>;
    EXPECT_EQ(WritePos::accessType, AccessType::Write);
}

TEST_F(ComponentAccessTest, ComponentTypeExtraction) {
    using ReadPos = ComponentAccess<Position, AccessType::Read>;
    static_assert(std::is_same_v<ReadPos::ComponentType, Position>);
    SUCCEED();
}

TEST_F(ComponentAccessTest, DifferentComponentsSameAccess) {
    using ReadPos = ComponentAccess<Position, AccessType::Read>;
    using ReadVel = ComponentAccess<Velocity, AccessType::Read>;

    EXPECT_EQ(ReadPos::accessType, ReadVel::accessType);
    static_assert(!std::is_same_v<ReadPos::ComponentType, ReadVel::ComponentType>);
    SUCCEED();
}

// =============================================================================
// Read/Write Alias Tests
// =============================================================================

class ReadWriteAliasTest : public ::testing::Test {};

TEST_F(ReadWriteAliasTest, ReadAliasIsComponentAccessRead) {
    static_assert(std::is_same_v<Read<Position>, ComponentAccess<Position, AccessType::Read>>);
    SUCCEED();
}

TEST_F(ReadWriteAliasTest, WriteAliasIsComponentAccessWrite) {
    static_assert(std::is_same_v<Write<Position>, ComponentAccess<Position, AccessType::Write>>);
    SUCCEED();
}

TEST_F(ReadWriteAliasTest, ReadHasCorrectAccessType) {
    EXPECT_EQ(Read<Position>::accessType, AccessType::Read);
}

TEST_F(ReadWriteAliasTest, WriteHasCorrectAccessType) {
    EXPECT_EQ(Write<Position>::accessType, AccessType::Write);
}

TEST_F(ReadWriteAliasTest, ReadExtractsComponentType) {
    static_assert(std::is_same_v<Read<Position>::ComponentType, Position>);
    static_assert(std::is_same_v<Read<Velocity>::ComponentType, Velocity>);
    SUCCEED();
}

TEST_F(ReadWriteAliasTest, WriteExtractsComponentType) {
    static_assert(std::is_same_v<Write<Position>::ComponentType, Position>);
    static_assert(std::is_same_v<Write<Health>::ComponentType, Health>);
    SUCCEED();
}

// =============================================================================
// ReadSingleton/WriteSingleton Tests
// =============================================================================

class SingletonAccessTest : public ::testing::Test {};

TEST_F(SingletonAccessTest, ReadSingletonAccessType) {
    EXPECT_EQ(ReadSingleton<Position>::accessType, AccessType::Read);
}

TEST_F(SingletonAccessTest, WriteSingletonAccessType) {
    EXPECT_EQ(WriteSingleton<Position>::accessType, AccessType::Write);
}

TEST_F(SingletonAccessTest, ReadSingletonComponentType) {
    static_assert(std::is_same_v<ReadSingleton<Position>::ComponentType, Position>);
    SUCCEED();
}

TEST_F(SingletonAccessTest, WriteSingletonComponentType) {
    static_assert(std::is_same_v<WriteSingleton<Velocity>::ComponentType, Velocity>);
    SUCCEED();
}

// =============================================================================
// IsReadSingleton Type Trait Tests
// =============================================================================

class IsReadSingletonTest : public ::testing::Test {};

TEST_F(IsReadSingletonTest, TrueForReadSingleton) {
    static_assert(IsReadSingleton<ReadSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsReadSingletonTest, FalseForWriteSingleton) {
    static_assert(!IsReadSingleton<WriteSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsReadSingletonTest, FalseForRead) {
    static_assert(!IsReadSingleton<Read<Position>>::value);
    SUCCEED();
}

TEST_F(IsReadSingletonTest, FalseForWrite) {
    static_assert(!IsReadSingleton<Write<Position>>::value);
    SUCCEED();
}

TEST_F(IsReadSingletonTest, FalseForPlainType) {
    static_assert(!IsReadSingleton<Position>::value);
    SUCCEED();
}

// =============================================================================
// IsWriteSingleton Type Trait Tests
// =============================================================================

class IsWriteSingletonTest : public ::testing::Test {};

TEST_F(IsWriteSingletonTest, TrueForWriteSingleton) {
    static_assert(IsWriteSingleton<WriteSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsWriteSingletonTest, FalseForReadSingleton) {
    static_assert(!IsWriteSingleton<ReadSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsWriteSingletonTest, FalseForRead) {
    static_assert(!IsWriteSingleton<Read<Position>>::value);
    SUCCEED();
}

TEST_F(IsWriteSingletonTest, FalseForWrite) {
    static_assert(!IsWriteSingleton<Write<Position>>::value);
    SUCCEED();
}

TEST_F(IsWriteSingletonTest, FalseForPlainType) {
    static_assert(!IsWriteSingleton<Position>::value);
    SUCCEED();
}

// =============================================================================
// IsSingleton Type Trait Tests
// =============================================================================

class IsSingletonTest : public ::testing::Test {};

TEST_F(IsSingletonTest, TrueForReadSingleton) {
    static_assert(IsSingleton<ReadSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsSingletonTest, TrueForWriteSingleton) {
    static_assert(IsSingleton<WriteSingleton<Position>>::value);
    SUCCEED();
}

TEST_F(IsSingletonTest, FalseForRead) {
    static_assert(!IsSingleton<Read<Position>>::value);
    SUCCEED();
}

TEST_F(IsSingletonTest, FalseForWrite) {
    static_assert(!IsSingleton<Write<Position>>::value);
    SUCCEED();
}

TEST_F(IsSingletonTest, FalseForPlainType) {
    static_assert(!IsSingleton<Position>::value);
    SUCCEED();
}

// =============================================================================
// Owned/NonOwned Tests
// =============================================================================

class OwnedNonOwnedTest : public ::testing::Test {};

TEST_F(OwnedNonOwnedTest, OwnedContainsComponentTypes) {
    using OwnedTypes = Owned<Read<Position>, Write<Velocity>>;
    static_assert(std::tuple_size_v<OwnedTypes::ComponentTypes> == 2);
    SUCCEED();
}

TEST_F(OwnedNonOwnedTest, NonOwnedContainsComponentTypes) {
    using NonOwnedTypes = NonOwned<Read<Health>>;
    static_assert(std::tuple_size_v<NonOwnedTypes::ComponentTypes> == 1);
    SUCCEED();
}

TEST_F(OwnedNonOwnedTest, EmptyOwned) {
    using EmptyOwned = Owned<>;
    static_assert(std::tuple_size_v<EmptyOwned::ComponentTypes> == 0);
    SUCCEED();
}

TEST_F(OwnedNonOwnedTest, EmptyNonOwned) {
    using EmptyNonOwned = NonOwned<>;
    static_assert(std::tuple_size_v<EmptyNonOwned::ComponentTypes> == 0);
    SUCCEED();
}

// =============================================================================
// ExtractComponentTypes Tests
// =============================================================================

class ExtractComponentTypesTest : public ::testing::Test {};

TEST_F(ExtractComponentTypesTest, ExtractsFromTuple) {
    using AccessTuple = std::tuple<Read<Position>, Write<Velocity>>;
    using Extracted = ExtractComponentTypes<AccessTuple>::Types;

    static_assert(std::is_same_v<std::tuple_element_t<0, Extracted>, Position>);
    static_assert(std::is_same_v<std::tuple_element_t<1, Extracted>, Velocity>);
    SUCCEED();
}

TEST_F(ExtractComponentTypesTest, ExtractsFromSingleElement) {
    using AccessTuple = std::tuple<Read<Health>>;
    using Extracted = ExtractComponentTypes<AccessTuple>::Types;

    static_assert(std::tuple_size_v<Extracted> == 1);
    static_assert(std::is_same_v<std::tuple_element_t<0, Extracted>, Health>);
    SUCCEED();
}

// =============================================================================
// tuple_for_each Tests
// =============================================================================

class TupleForEachTest : public ::testing::Test {};

TEST_F(TupleForEachTest, IteratesOverTuple) {
    auto tuple = std::make_tuple(1, 2, 3);
    int sum = 0;

    tuple_for_each(tuple, [&sum](int val) { sum += val; });

    EXPECT_EQ(sum, 6);
}

TEST_F(TupleForEachTest, HandlesEmptyTuple) {
    auto tuple = std::make_tuple();
    int count = 0;

    tuple_for_each(tuple, [&count](auto) { count++; });

    EXPECT_EQ(count, 0);
}

TEST_F(TupleForEachTest, WorksWithDifferentTypes) {
    auto tuple = std::make_tuple(1, 2.5f, 3.0);
    double sum = 0;

    tuple_for_each(tuple, [&sum](auto val) { sum += static_cast<double>(val); });

    EXPECT_DOUBLE_EQ(sum, 6.5);
}

TEST_F(TupleForEachTest, CanModifyTupleElements) {
    auto tuple = std::make_tuple(1, 2, 3);

    tuple_for_each(tuple, [](int& val) { val *= 2; });

    EXPECT_EQ(std::get<0>(tuple), 2);
    EXPECT_EQ(std::get<1>(tuple), 4);
    EXPECT_EQ(std::get<2>(tuple), 6);
}

}  // namespace nexo::ecs
