//// Definitions.test.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the utils in the definitions header
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Definitions.hpp"
#include <string>
#include <vector>
#include <type_traits>

namespace nexo::ecs {

	class DefinitionsTest : public ::testing::Test {};

	// Test structures for component type ID assignment
	struct TestComponent1 {};
	struct TestComponent2 {};
	struct TestComponent3 {};

	// A structure template to test type uniqueness
	template<int N>
	struct GenericComponent {};

	// getUniqueComponentTypeID function tests
	TEST_F(DefinitionsTest, GetUniqueComponentTypeIDAssignsUniqueIDs) {
	    // Get IDs for multiple component types
	    ComponentType id1 = getUniqueComponentTypeID<TestComponent1>();
	    ComponentType id2 = getUniqueComponentTypeID<TestComponent2>();
	    ComponentType id3 = getUniqueComponentTypeID<TestComponent3>();

	    // IDs should be unique
	    EXPECT_NE(id1, id2);
	    EXPECT_NE(id1, id3);
	    EXPECT_NE(id2, id3);

	    // IDs should be assigned sequentially starting from 0
	    EXPECT_EQ(id2, id1 + 1);
	    EXPECT_EQ(id3, id2 + 1);
	}

	TEST_F(DefinitionsTest, GetUniqueComponentTypeIDReturnsSameIDForSameType) {
	    // Get the ID for the same type multiple times
	    ComponentType id1 = getUniqueComponentTypeID<TestComponent1>();
	    ComponentType id2 = getUniqueComponentTypeID<TestComponent1>();
	    ComponentType id3 = getUniqueComponentTypeID<TestComponent1>();

	    // Same type should get the same ID
	    EXPECT_EQ(id1, id2);
	    EXPECT_EQ(id1, id3);
	}

	// getComponentTypeID function tests
	TEST_F(DefinitionsTest, GetComponentTypeIDRemovesTypeQualifiers) {
	    // Get IDs for TestComponent1 with different qualifiers
	    ComponentType baseId = getComponentTypeID<TestComponent1>();
	    ComponentType constId = getComponentTypeID<const TestComponent1>();
	    ComponentType volatileId = getComponentTypeID<volatile TestComponent1>();
	    ComponentType refId = getComponentTypeID<TestComponent1&>();
	    ComponentType constRefId = getComponentTypeID<const TestComponent1&>();

	    // All should return the same ID regardless of qualifiers
	    EXPECT_EQ(baseId, constId);
	    EXPECT_EQ(baseId, volatileId);
	    EXPECT_EQ(baseId, refId);
	    EXPECT_EQ(baseId, constRefId);
	}

	TEST_F(DefinitionsTest, GetComponentTypeIDForTemplatedTypes) {
	    // Test with template types
	    ComponentType id1 = getComponentTypeID<GenericComponent<1>>();
	    ComponentType id2 = getComponentTypeID<GenericComponent<2>>();
	    ComponentType id3 = getComponentTypeID<GenericComponent<3>>();

	    // Different template instantiations should get different IDs
	    EXPECT_NE(id1, id2);
	    EXPECT_NE(id1, id3);
	    EXPECT_NE(id2, id3);
	}
}
