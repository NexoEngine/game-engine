//// AssetRef.test.cpp ////////////////////////////////////////////////////////
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
//  Author:      Guillaume HEIN
//  Date:        18/03/2025
//  Description: Unit tests for the AssetRef class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetRef.hpp"
#include "assets/Asset.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::assets {

    class AssetRefTest : public ::testing::Test {
        protected:
            std::shared_ptr<Texture> textureAsset;
            std::shared_ptr<Model> modelAsset;
            std::shared_ptr<IAsset> genericAsset;

            void SetUp() override {
                textureAsset = std::make_shared<Texture>();
                modelAsset = std::make_shared<Model>();
                genericAsset = std::make_shared<Texture>(); // Using Texture as our generic asset
            }
    };

    // GenericAssetRef Tests
    TEST_F(AssetRefTest, DefaultConstructorCreatesNullReference) {
        GenericAssetRef ref;
        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_EQ(ref.lock(), nullptr);
    }

    TEST_F(AssetRefTest, ConstructorWithSharedPtrCreatesValidReference) {
        GenericAssetRef ref(genericAsset);
        EXPECT_TRUE(ref.isValid());
        EXPECT_TRUE(ref);
        EXPECT_EQ(ref.lock(), genericAsset);
    }

    TEST_F(AssetRefTest, NullStaticMethodReturnsNullReference) {
        auto ref = GenericAssetRef::null();
        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
    }

    TEST_F(AssetRefTest, IsValidReturnsCorrectValue) {
        const GenericAssetRef ref(genericAsset);
        EXPECT_TRUE(ref.isValid());

        // Simulate expired pointer
        GenericAssetRef tempRef;
        {
            const auto tempAsset = std::make_shared<Texture>();
            tempRef = GenericAssetRef(tempAsset);
            EXPECT_TRUE(tempRef.isValid());
        }
        // tempAsset goes out of scope here, should expire
        EXPECT_FALSE(tempRef.isValid());
    }

    TEST_F(AssetRefTest, LockReturnsCorrectPointer) {
        GenericAssetRef validRef(genericAsset);
        GenericAssetRef nullRef;

        EXPECT_EQ(validRef.lock(), genericAsset);
        EXPECT_EQ(nullRef.lock(), nullptr);
    }

    TEST_F(AssetRefTest, AsMethodCastsCorrectly) {
        // Create refs
        GenericAssetRef textureRef(textureAsset);
        GenericAssetRef modelRef(modelAsset);
        GenericAssetRef nullRef;

        // Cast to texture ref
        auto castedTextureRef = textureRef.as<Texture>();
        EXPECT_TRUE(castedTextureRef.isValid());
        EXPECT_EQ(castedTextureRef.lock(), textureAsset);

        // Cast to model ref
        auto castedModelRef = modelRef.as<Model>();
        EXPECT_TRUE(castedModelRef.isValid());
        EXPECT_EQ(castedModelRef.lock(), modelAsset);

        // Cast null ref
        auto castedNullRef = nullRef.as<Texture>();
        EXPECT_FALSE(castedNullRef.isValid());

        // Wrong cast should produce invalid reference
        auto wrongCastRef = textureRef.as<Model>();
        EXPECT_FALSE(wrongCastRef.isValid());
    }

    TEST_F(AssetRefTest, BoolOperatorWorksAsExpected) {
        GenericAssetRef validRef(genericAsset);
        GenericAssetRef nullRef;

        EXPECT_TRUE(static_cast<bool>(validRef));
        EXPECT_FALSE(static_cast<bool>(nullRef));
    }

    TEST_F(AssetRefTest, CopyConstructorWorksCorrectly) {
        GenericAssetRef original(genericAsset);
        GenericAssetRef copy(original);

        EXPECT_TRUE(copy.isValid());
        EXPECT_EQ(copy.lock(), original.lock());
    }

    TEST_F(AssetRefTest, AssignmentOperatorWorksCorrectly) {
        GenericAssetRef original(genericAsset);
        GenericAssetRef assigned;
        assigned = original;

        EXPECT_TRUE(assigned.isValid());
        EXPECT_EQ(assigned.lock(), original.lock());
    }

    // AssetRef<T> Tests
    TEST_F(AssetRefTest, TypedDefaultConstructorCreatesNullReference) {
        AssetRef<Texture> ref;
        EXPECT_FALSE(ref.isValid());
        EXPECT_EQ(ref.lock(), nullptr);
    }

    TEST_F(AssetRefTest, TypedConstructorWithSharedPtrCreatesValidReference) {
        AssetRef<Texture> ref(textureAsset);
        EXPECT_TRUE(ref.isValid());
        EXPECT_EQ(ref.lock(), textureAsset);
    }

    TEST_F(AssetRefTest, TypedNullStaticMethodReturnsNullReference) {
        auto ref = AssetRef<Texture>::null();
        EXPECT_FALSE(ref.isValid());
    }

    TEST_F(AssetRefTest, TypedLockReturnsCorrectPointer) {
        AssetRef<Texture> validRef(textureAsset);
        AssetRef<Texture> nullRef;

        EXPECT_EQ(validRef.lock(), textureAsset);
        EXPECT_EQ(nullRef.lock(), nullptr);
    }

    TEST_F(AssetRefTest, IsLoadedReturnsCorrectState) {
        // Set up a loaded asset
        auto loadedAsset = std::make_shared<Texture>();
        loadedAsset->m_metadata.status = AssetStatus::LOADED;

        // Set up an unloaded asset
        auto unloadedAsset = std::make_shared<Texture>();
        unloadedAsset->m_metadata.status = AssetStatus::UNLOADED;

        AssetRef<Texture> loadedRef(loadedAsset);
        AssetRef<Texture> unloadedRef(unloadedAsset);
        AssetRef<Texture> nullRef;

        EXPECT_TRUE(loadedRef.isLoaded());
        EXPECT_FALSE(unloadedRef.isLoaded());
        EXPECT_FALSE(nullRef.isLoaded());
    }

    TEST_F(AssetRefTest, LoadAndUnloadMethodsCalledSuccessfully) {
        // These methods currently only log warnings, but we test they don't crash
        GenericAssetRef ref(genericAsset);
        EXPECT_NO_FATAL_FAILURE(ref.load());
        EXPECT_NO_FATAL_FAILURE(ref.unload());
    }

    // GenericAssetRef Comparison Operator Tests
    TEST_F(AssetRefTest, EqualityOperatorComparingTwoRefsToSameAsset) {
        GenericAssetRef ref1(genericAsset);
        GenericAssetRef ref2(genericAsset);

        EXPECT_TRUE(ref1 == ref2);
        EXPECT_FALSE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingTwoRefsToSameAssetAfterCopy) {
        GenericAssetRef ref1(genericAsset);
        GenericAssetRef ref2(ref1); // Copy constructor

        EXPECT_TRUE(ref1 == ref2);
        EXPECT_FALSE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingTwoRefsToSameAssetAfterAssignment) {
        GenericAssetRef ref1(genericAsset);
        GenericAssetRef ref2;
        ref2 = ref1; // Copy assignment

        EXPECT_TRUE(ref1 == ref2);
        EXPECT_FALSE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingTwoRefsToDifferentAssets) {
        GenericAssetRef ref1(textureAsset);
        GenericAssetRef ref2(modelAsset);

        EXPECT_FALSE(ref1 == ref2);
        EXPECT_TRUE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingTwoNullRefs) {
        GenericAssetRef ref1;
        GenericAssetRef ref2;

        // Two null refs should be equal (both lock to nullptr)
        EXPECT_TRUE(ref1 == ref2);
        EXPECT_FALSE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingValidRefToNullRef) {
        GenericAssetRef validRef(genericAsset);
        GenericAssetRef nullRef;

        EXPECT_FALSE(validRef == nullRef);
        EXPECT_TRUE(validRef != nullRef);
    }

    TEST_F(AssetRefTest, EqualityOperatorComparingExpiredRefToNullRef) {
        GenericAssetRef expiredRef;
        {
            auto tempAsset = std::make_shared<Texture>();
            expiredRef = GenericAssetRef(tempAsset);
        }
        // tempAsset goes out of scope, expiredRef is now expired

        GenericAssetRef nullRef;

        // Expired ref and null ref should be equal (both lock to nullptr)
        EXPECT_TRUE(expiredRef == nullRef);
        EXPECT_FALSE(expiredRef != nullRef);
    }

    TEST_F(AssetRefTest, NullptrEqualityOperatorWithNullRef) {
        GenericAssetRef nullRef;

        EXPECT_TRUE(nullRef == nullptr);
        EXPECT_FALSE(nullRef != nullptr);
    }

    TEST_F(AssetRefTest, NullptrEqualityOperatorWithValidRef) {
        GenericAssetRef validRef(genericAsset);

        EXPECT_FALSE(validRef == nullptr);
        EXPECT_TRUE(validRef != nullptr);
    }

    TEST_F(AssetRefTest, NullptrEqualityOperatorWithExpiredRef) {
        GenericAssetRef expiredRef;
        {
            auto tempAsset = std::make_shared<Texture>();
            expiredRef = GenericAssetRef(tempAsset);
        }
        // tempAsset goes out of scope, expiredRef is now expired

        EXPECT_TRUE(expiredRef == nullptr);
        EXPECT_FALSE(expiredRef != nullptr);
    }

    TEST_F(AssetRefTest, InequalityOperatorWithDifferentAssetTypes) {
        GenericAssetRef textureRef(textureAsset);
        GenericAssetRef modelRef(modelAsset);

        EXPECT_TRUE(textureRef != modelRef);
        EXPECT_FALSE(textureRef == modelRef);
    }

    TEST_F(AssetRefTest, EqualityOperatorWithSelfComparison) {
        GenericAssetRef ref(genericAsset);

        EXPECT_TRUE(ref == ref);
        EXPECT_FALSE(ref != ref);
    }

    // TypedAssetRef Comparison Tests
    TEST_F(AssetRefTest, TypedAssetRefEqualityWithSameAsset) {
        AssetRef<Texture> ref1(textureAsset);
        AssetRef<Texture> ref2(textureAsset);

        EXPECT_TRUE(ref1 == ref2);
        EXPECT_FALSE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, TypedAssetRefEqualityWithDifferentAssets) {
        auto texture1 = std::make_shared<Texture>();
        auto texture2 = std::make_shared<Texture>();

        AssetRef<Texture> ref1(texture1);
        AssetRef<Texture> ref2(texture2);

        EXPECT_FALSE(ref1 == ref2);
        EXPECT_TRUE(ref1 != ref2);
    }

    TEST_F(AssetRefTest, TypedAssetRefNullptrComparison) {
        AssetRef<Texture> validRef(textureAsset);
        AssetRef<Texture> nullRef;

        EXPECT_FALSE(validRef == nullptr);
        EXPECT_TRUE(validRef != nullptr);

        EXPECT_TRUE(nullRef == nullptr);
        EXPECT_FALSE(nullRef != nullptr);
    }

    TEST_F(AssetRefTest, TypedAssetRefConstructedWithNullptr) {
        AssetRef<Texture> nullRef(nullptr);

        EXPECT_TRUE(nullRef == nullptr);
        EXPECT_FALSE(nullRef != nullptr);
        EXPECT_FALSE(nullRef.isValid());
    }

} // namespace nexo::assets
