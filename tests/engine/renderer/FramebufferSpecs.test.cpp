//// FramebufferSpecs.test.cpp ////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Framebuffer specification structures
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Framebuffer.hpp"

namespace nexo::renderer {

// =============================================================================
// NxFrameBufferTextureFormats Enum Tests
// =============================================================================

class FrameBufferTextureFormatsEnumTest : public ::testing::Test {};

TEST_F(FrameBufferTextureFormatsEnumTest, NoneValueIsZero) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::NONE), 0);
}

TEST_F(FrameBufferTextureFormatsEnumTest, RGBA8ValueExists) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::RGBA8), 1);
}

TEST_F(FrameBufferTextureFormatsEnumTest, RGBA16ValueExists) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::RGBA16), 2);
}

TEST_F(FrameBufferTextureFormatsEnumTest, RedIntegerValueExists) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::RED_INTEGER), 3);
}

TEST_F(FrameBufferTextureFormatsEnumTest, Depth24Stencil8ValueExists) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::DEPTH24STENCIL8), 4);
}

TEST_F(FrameBufferTextureFormatsEnumTest, DepthAliasEqualsDepth24Stencil8) {
    EXPECT_EQ(NxFrameBufferTextureFormats::Depth, NxFrameBufferTextureFormats::DEPTH24STENCIL8);
}

TEST_F(FrameBufferTextureFormatsEnumTest, NbTextureFormatsCount) {
    EXPECT_EQ(static_cast<int>(NxFrameBufferTextureFormats::NB_TEXTURE_FORMATS), 5);
}

TEST_F(FrameBufferTextureFormatsEnumTest, AllFormatsAreDistinct) {
    EXPECT_NE(NxFrameBufferTextureFormats::NONE, NxFrameBufferTextureFormats::RGBA8);
    EXPECT_NE(NxFrameBufferTextureFormats::RGBA8, NxFrameBufferTextureFormats::RGBA16);
    EXPECT_NE(NxFrameBufferTextureFormats::RGBA16, NxFrameBufferTextureFormats::RED_INTEGER);
    EXPECT_NE(NxFrameBufferTextureFormats::RED_INTEGER, NxFrameBufferTextureFormats::DEPTH24STENCIL8);
}

// =============================================================================
// NxFrameBufferTextureSpecifications Tests
// =============================================================================

class FrameBufferTextureSpecificationsTest : public ::testing::Test {};

TEST_F(FrameBufferTextureSpecificationsTest, DefaultConstructorSetsNone) {
    NxFrameBufferTextureSpecifications spec;
    EXPECT_EQ(spec.textureFormat, NxFrameBufferTextureFormats::NONE);
}

TEST_F(FrameBufferTextureSpecificationsTest, ConstructorWithRGBA8) {
    NxFrameBufferTextureSpecifications spec(NxFrameBufferTextureFormats::RGBA8);
    EXPECT_EQ(spec.textureFormat, NxFrameBufferTextureFormats::RGBA8);
}

TEST_F(FrameBufferTextureSpecificationsTest, ConstructorWithRGBA16) {
    NxFrameBufferTextureSpecifications spec(NxFrameBufferTextureFormats::RGBA16);
    EXPECT_EQ(spec.textureFormat, NxFrameBufferTextureFormats::RGBA16);
}

TEST_F(FrameBufferTextureSpecificationsTest, ConstructorWithRedInteger) {
    NxFrameBufferTextureSpecifications spec(NxFrameBufferTextureFormats::RED_INTEGER);
    EXPECT_EQ(spec.textureFormat, NxFrameBufferTextureFormats::RED_INTEGER);
}

TEST_F(FrameBufferTextureSpecificationsTest, ConstructorWithDepth) {
    NxFrameBufferTextureSpecifications spec(NxFrameBufferTextureFormats::Depth);
    EXPECT_EQ(spec.textureFormat, NxFrameBufferTextureFormats::DEPTH24STENCIL8);
}

TEST_F(FrameBufferTextureSpecificationsTest, CopyConstructor) {
    NxFrameBufferTextureSpecifications original(NxFrameBufferTextureFormats::RGBA16);
    NxFrameBufferTextureSpecifications copy = original;
    EXPECT_EQ(copy.textureFormat, NxFrameBufferTextureFormats::RGBA16);
}

TEST_F(FrameBufferTextureSpecificationsTest, AssignmentOperator) {
    NxFrameBufferTextureSpecifications spec1(NxFrameBufferTextureFormats::RGBA8);
    NxFrameBufferTextureSpecifications spec2;
    spec2 = spec1;
    EXPECT_EQ(spec2.textureFormat, NxFrameBufferTextureFormats::RGBA8);
}

// =============================================================================
// NxFrameBufferAttachmentsSpecifications Tests
// =============================================================================

class FrameBufferAttachmentsSpecificationsTest : public ::testing::Test {};

TEST_F(FrameBufferAttachmentsSpecificationsTest, DefaultConstructorEmptyAttachments) {
    NxFrameBufferAttachmentsSpecifications specs;
    EXPECT_TRUE(specs.attachments.empty());
}

TEST_F(FrameBufferAttachmentsSpecificationsTest, InitializerListSingleAttachment) {
    NxFrameBufferAttachmentsSpecifications specs{
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8)
    };
    ASSERT_EQ(specs.attachments.size(), 1);
    EXPECT_EQ(specs.attachments[0].textureFormat, NxFrameBufferTextureFormats::RGBA8);
}

TEST_F(FrameBufferAttachmentsSpecificationsTest, InitializerListMultipleAttachments) {
    NxFrameBufferAttachmentsSpecifications specs{
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA16),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };
    ASSERT_EQ(specs.attachments.size(), 3);
    EXPECT_EQ(specs.attachments[0].textureFormat, NxFrameBufferTextureFormats::RGBA8);
    EXPECT_EQ(specs.attachments[1].textureFormat, NxFrameBufferTextureFormats::RGBA16);
    EXPECT_EQ(specs.attachments[2].textureFormat, NxFrameBufferTextureFormats::Depth);
}

TEST_F(FrameBufferAttachmentsSpecificationsTest, PushBackAttachment) {
    NxFrameBufferAttachmentsSpecifications specs;
    specs.attachments.push_back(NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8));
    ASSERT_EQ(specs.attachments.size(), 1);
    EXPECT_EQ(specs.attachments[0].textureFormat, NxFrameBufferTextureFormats::RGBA8);
}

TEST_F(FrameBufferAttachmentsSpecificationsTest, ColorAndDepthAttachment) {
    NxFrameBufferAttachmentsSpecifications specs{
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::DEPTH24STENCIL8)
    };
    EXPECT_EQ(specs.attachments.size(), 2);
}

// =============================================================================
// NxFramebufferSpecs Tests
// =============================================================================

class FramebufferSpecsTest : public ::testing::Test {};

TEST_F(FramebufferSpecsTest, DefaultWidthIsZero) {
    NxFramebufferSpecs specs;
    EXPECT_EQ(specs.width, 0u);
}

TEST_F(FramebufferSpecsTest, DefaultHeightIsZero) {
    NxFramebufferSpecs specs;
    EXPECT_EQ(specs.height, 0u);
}

TEST_F(FramebufferSpecsTest, DefaultSamplesIsOne) {
    NxFramebufferSpecs specs;
    EXPECT_EQ(specs.samples, 1u);
}

TEST_F(FramebufferSpecsTest, DefaultSwapChainTargetIsFalse) {
    NxFramebufferSpecs specs;
    EXPECT_FALSE(specs.swapChainTarget);
}

TEST_F(FramebufferSpecsTest, DefaultAttachmentsEmpty) {
    NxFramebufferSpecs specs;
    EXPECT_TRUE(specs.attachments.attachments.empty());
}

TEST_F(FramebufferSpecsTest, SetDimensions) {
    NxFramebufferSpecs specs;
    specs.width = 1920;
    specs.height = 1080;
    EXPECT_EQ(specs.width, 1920u);
    EXPECT_EQ(specs.height, 1080u);
}

TEST_F(FramebufferSpecsTest, SetSamples) {
    NxFramebufferSpecs specs;
    specs.samples = 4;
    EXPECT_EQ(specs.samples, 4u);
}

TEST_F(FramebufferSpecsTest, SetSwapChainTarget) {
    NxFramebufferSpecs specs;
    specs.swapChainTarget = true;
    EXPECT_TRUE(specs.swapChainTarget);
}

TEST_F(FramebufferSpecsTest, SetAttachments) {
    NxFramebufferSpecs specs;
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };
    EXPECT_EQ(specs.attachments.attachments.size(), 2);
}

TEST_F(FramebufferSpecsTest, FullConfiguration) {
    NxFramebufferSpecs specs;
    specs.width = 800;
    specs.height = 600;
    specs.samples = 8;
    specs.swapChainTarget = false;
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA16),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RED_INTEGER),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };

    EXPECT_EQ(specs.width, 800u);
    EXPECT_EQ(specs.height, 600u);
    EXPECT_EQ(specs.samples, 8u);
    EXPECT_FALSE(specs.swapChainTarget);
    EXPECT_EQ(specs.attachments.attachments.size(), 3);
}

TEST_F(FramebufferSpecsTest, CopyConstructor) {
    NxFramebufferSpecs original;
    original.width = 1024;
    original.height = 768;
    original.samples = 2;

    NxFramebufferSpecs copy = original;

    EXPECT_EQ(copy.width, 1024u);
    EXPECT_EQ(copy.height, 768u);
    EXPECT_EQ(copy.samples, 2u);
}

// =============================================================================
// Common Framebuffer Configuration Patterns
// =============================================================================

class FramebufferCommonPatternsTest : public ::testing::Test {};

TEST_F(FramebufferCommonPatternsTest, SimpleColorBuffer) {
    NxFramebufferSpecs specs;
    specs.width = 1280;
    specs.height = 720;
    specs.attachments = {NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8)};

    EXPECT_EQ(specs.attachments.attachments.size(), 1);
    EXPECT_EQ(specs.attachments.attachments[0].textureFormat, NxFrameBufferTextureFormats::RGBA8);
}

TEST_F(FramebufferCommonPatternsTest, ColorAndDepthBuffer) {
    NxFramebufferSpecs specs;
    specs.width = 1920;
    specs.height = 1080;
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };

    EXPECT_EQ(specs.attachments.attachments.size(), 2);
}

TEST_F(FramebufferCommonPatternsTest, GBufferSetup) {
    // G-buffer typically has multiple color attachments + depth
    NxFramebufferSpecs specs;
    specs.width = 1920;
    specs.height = 1080;
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA16),  // Position
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA16),  // Normal
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),   // Albedo
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)    // Depth
    };

    EXPECT_EQ(specs.attachments.attachments.size(), 4);
}

TEST_F(FramebufferCommonPatternsTest, EntityPickingBuffer) {
    // Entity ID picking uses RED_INTEGER
    NxFramebufferSpecs specs;
    specs.width = 800;
    specs.height = 600;
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RED_INTEGER),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };

    EXPECT_EQ(specs.attachments.attachments.size(), 2);
    EXPECT_EQ(specs.attachments.attachments[0].textureFormat, NxFrameBufferTextureFormats::RED_INTEGER);
}

TEST_F(FramebufferCommonPatternsTest, MultisampledBuffer) {
    NxFramebufferSpecs specs;
    specs.width = 1920;
    specs.height = 1080;
    specs.samples = 4;  // 4x MSAA
    specs.attachments = {
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::RGBA8),
        NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::Depth)
    };

    EXPECT_EQ(specs.samples, 4u);
}

}  // namespace nexo::renderer
