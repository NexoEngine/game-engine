//// Pipeline.test.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        24/06/2025
//  Description: Test file for the render pipeline class
//
///////////////////////////////////////////////////////////////////////////////
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>

#include "RenderPipeline.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"

namespace nexo::renderer {


class MockRenderPass : public RenderPass {
public:
    MockRenderPass(PassId id, const std::string& name) : RenderPass(id, name) {
        // Explicitly initialize prerequisites and effects to empty vectors
        prerequisites = {};
        effects = {};
    }

    MOCK_METHOD(void, execute, (RenderPipeline& pipeline), (override));
    MOCK_METHOD(void, resize, (unsigned int width, unsigned int height), (override));
};

// Forward declarations for mocking
class NxFramebufferSpecs; // Forward declaration if needed

class MockFramebuffer : public NxFramebuffer {
public:
    MOCK_METHOD(void, bind, (), (override));
    MOCK_METHOD(void, unbind, (), (override));
    MOCK_METHOD(void, resize, (unsigned int width, unsigned int height), (override));
    MOCK_METHOD(glm::vec2, getSize, (), (const, override));
    MOCK_METHOD(unsigned int, getColorAttachmentId, (unsigned int index), (const, override));
    // MOCK_METHOD(void, clearAttachment, (unsigned int index, int value), (override));
    // MOCK_METHOD(void, clearAttachment, (unsigned int index, float value), (override));
    // MOCK_METHOD(void, clearAttachment, (unsigned int index, const glm::vec4& value), (override));
    // MOCK_METHOD(void, invalidate, (), (override));

    // Add missing methods that need to be implemented
    MOCK_METHOD(void, bindAsTexture, (unsigned int slot, unsigned int attachment), (override));
    MOCK_METHOD(void, bindDepthAsTexture, (unsigned int slot), (override));
    MOCK_METHOD(void, setClearColor, (const glm::vec4& color), (override));
    MOCK_METHOD(void, copy, (const std::shared_ptr<NxFramebuffer> source), (override));
    MOCK_METHOD(unsigned int, getFramebufferId, (), (const, override));
    MOCK_METHOD(void, getPixelWrapper, (unsigned int attachmentIndex, int x, int y, void* result, const std::type_info& ti), (const, override));
    MOCK_METHOD(void, clearAttachmentWrapper, (unsigned int attachmentIndex, const void* value, const std::type_info& ti), (const, override));
    MOCK_METHOD(NxFramebufferSpecs&, getSpecs, (), (override));
    MOCK_METHOD(const NxFramebufferSpecs&, getSpecs, (), (const, override));
    MOCK_METHOD(unsigned int, getNbColorAttachments, (), (const, override));
    MOCK_METHOD(unsigned int, getDepthAttachmentId, (), (const, override));
    MOCK_METHOD(bool, hasDepthAttachment, (), (const, override));
    MOCK_METHOD(bool, hasStencilAttachment, (), (const, override));
    MOCK_METHOD(bool, hasDepthStencilAttachment, (), (const, override));
};

class RenderPipelineTest : public ::testing::Test {
protected:
    RenderPipeline pipeline;

    void SetUp() override {
        // Common setup for tests
    }

    void TearDown() override {
        // Common teardown for tests
    }

    std::shared_ptr<MockRenderPass> createMockPass(const std::string& name) {
        static PassId id = 0;
        return std::make_shared<MockRenderPass>(id++, name);
    }

    std::shared_ptr<MockFramebuffer> createMockFramebuffer() {
        return std::make_shared<MockFramebuffer>();
    }
};

TEST_F(RenderPipelineTest, AddRenderPass) {
    // Test adding a render pass
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);

    // Verify passes were added with unique IDs
    EXPECT_NE(id1, id2);
    EXPECT_EQ(pass1->getId(), id1);
    EXPECT_EQ(pass2->getId(), id2);

    // First pass should be set as final output
    EXPECT_EQ(pipeline.getFinalOutputPass(), id1);
}

TEST_F(RenderPipelineTest, RemoveRenderPass) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    pipeline.setFinalOutputPass(id1);

    // Remove pass1 and check pass2 becomes final output
    pipeline.removeRenderPass(id1);
    EXPECT_EQ(pipeline.getFinalOutputPass(), static_cast<int>(id2));
    EXPECT_EQ(pipeline.getRenderPass(id1), nullptr);
    EXPECT_NE(pipeline.getRenderPass(id2), nullptr);

    // Now remove the last pass
    pipeline.removeRenderPass(id2);

    // Verify we can access getFinalOutputPass without crashing
    // and it returns -1 (since there are no more passes)
    EXPECT_EQ(pipeline.getFinalOutputPass(), -1);

    // Verify createExecutionPlan returns an empty vector
    auto plan = pipeline.createExecutionPlan();
    EXPECT_TRUE(plan.empty());

    // Verify the pass is gone
    EXPECT_EQ(pipeline.getRenderPass(id2), nullptr);

    // Add a new pass and verify it becomes the final output
    auto pass3 = createMockPass("Pass3");
    PassId id3 = pipeline.addRenderPass(pass3);

    EXPECT_EQ(pipeline.getFinalOutputPass(), static_cast<int>(id3));
}

TEST_F(RenderPipelineTest, PrerequisitesAndEffects) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto pass3 = createMockPass("Pass3");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    PassId id3 = pipeline.addRenderPass(pass3);

    // Add relationships
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    pipeline.addPrerequisite(id3, id2);
    pipeline.addEffect(id2, id3);

    // Check relationships
    EXPECT_THAT(pass2->getPrerequisites(), ::testing::ElementsAre(id1));
    EXPECT_THAT(pass3->getPrerequisites(), ::testing::ElementsAre(id2));
    EXPECT_THAT(pass1->getEffects(), ::testing::ElementsAre(id2));
    EXPECT_THAT(pass2->getEffects(), ::testing::ElementsAre(id3));

    // Test removal of relationships
    pipeline.removePrerequisite(id2, id1);
    EXPECT_THAT(pass2->getPrerequisites(), ::testing::IsEmpty());

    pipeline.removeEffect(id2, id3);
    EXPECT_THAT(pass2->getEffects(), ::testing::IsEmpty());
}

TEST_F(RenderPipelineTest, ExecutionPlanCorrectOrder) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto pass3 = createMockPass("Pass3");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    PassId id3 = pipeline.addRenderPass(pass3);

    // Setup a simple chain: pass1 -> pass2 -> pass3
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    pipeline.addPrerequisite(id3, id2);
    pipeline.addEffect(id2, id3);

    // Set final output to pass3
    pipeline.setFinalOutputPass(id3);

    // Get execution plan
    auto plan = pipeline.createExecutionPlan();

    // Execution plan should have all passes in correct order: pass1, pass2, pass3
    EXPECT_EQ(plan.size(), 3);
    EXPECT_EQ(plan[0], id1);
    EXPECT_EQ(plan[1], id2);
    EXPECT_EQ(plan[2], id3);
}

TEST_F(RenderPipelineTest, RemovePassPreservesConnections) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto pass3 = createMockPass("Pass3");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    PassId id3 = pipeline.addRenderPass(pass3);

    // Setup a simple chain: pass1 -> pass2 -> pass3
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    pipeline.addPrerequisite(id3, id2);
    pipeline.addEffect(id2, id3);

    // Remove middle pass
    pipeline.removeRenderPass(id2);

    // Now pass1 should be connected to pass3
    EXPECT_THAT(pass3->getPrerequisites(), ::testing::ElementsAre(id1));
    EXPECT_THAT(pass1->getEffects(), ::testing::ElementsAre(id3));
}

TEST_F(RenderPipelineTest, ExecutePipeline) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto renderTarget = createMockFramebuffer();

    // Set up expectations for the execute method
    EXPECT_CALL(*pass1, execute(::testing::_)).Times(1);
    EXPECT_CALL(*pass2, execute(::testing::_)).Times(1);

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);

    std::cout << "la" << std::endl;
    // Setup pass1 -> pass2
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);
    std::cout << "la2" << std::endl;


    // Make sure pass2 is the final output to ensure it gets executed
    pipeline.setFinalOutputPass(id2);
    pipeline.setRenderTarget(renderTarget);
    std::cout << "la3" << std::endl;


    // Verify the setup is correct
    EXPECT_EQ(pipeline.getFinalOutputPass(), static_cast<int>(id2));
    std::cout << "la7" << std::endl;


    // Execute the pipeline
    pipeline.execute();
    std::cout << "la5" << std::endl;

}

TEST_F(RenderPipelineTest, SetandGetRenderTarget) {
    auto pass = createMockPass("Pass1");
    auto framebuffer = createMockFramebuffer();

    pipeline.addRenderPass(pass);

    // Set and verify output framebuffer for a pass
    pipeline.setRenderTarget(framebuffer);
    auto retrievedFramebuffer = pipeline.getRenderTarget();

    EXPECT_EQ(retrievedFramebuffer, framebuffer);
}

TEST_F(RenderPipelineTest, DrawCommandsManagement) {
    DrawCommand cmd1;

    DrawCommand cmd2;

    auto mockRenderTarget = createMockFramebuffer();

    // Add single command
    pipeline.addDrawCommand(cmd1);

    // Add multiple commands
    std::vector<DrawCommand> commands = {cmd1, cmd2};
    pipeline.addDrawCommands(commands);

    // Verify commands were added
    auto& retrievedCommands = pipeline.getDrawCommands();
    EXPECT_EQ(retrievedCommands.size(), 3);

    pipeline.setRenderTarget(mockRenderTarget);

    // Check they're cleared after execution
    pipeline.execute();
    EXPECT_TRUE(pipeline.getDrawCommands().empty());
}

TEST_F(RenderPipelineTest, CameraClearColor) {
    glm::vec4 clearColor(0.1f, 0.2f, 0.3f, 1.0f);

    pipeline.setCameraClearColor(clearColor);
    auto retrievedColor = pipeline.getCameraClearColor();

    EXPECT_EQ(retrievedColor, clearColor);
}

TEST_F(RenderPipelineTest, FindTerminalPasses) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto pass3 = createMockPass("Pass3");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    PassId id3 = pipeline.addRenderPass(pass3);

    // Make pass1 -> pass2, but pass3 has no effects
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    // Get terminal passes
    auto terminals = pipeline.findTerminalPasses();

    // pass2 and pass3 should be terminal (no outgoing connections)
    EXPECT_EQ(terminals.size(), 2);
    EXPECT_THAT(terminals, ::testing::UnorderedElementsAre(id2, id3));
}

TEST_F(RenderPipelineTest, ResizePipeline) {
    auto pass = createMockPass("Pass1");
    auto framebuffer = createMockFramebuffer();

    EXPECT_CALL(*pass, resize(800, 600)).Times(1);
    EXPECT_CALL(*framebuffer, resize(800, 600)).Times(1);

    // Don't need the ID, but need to add pass to pipeline
    pipeline.addRenderPass(pass);
    pipeline.setRenderTarget(framebuffer);

    // Test resize
    pipeline.resize(800, 600);
}

TEST_F(RenderPipelineTest, ComplexDependencyChain) {
    // Create a more complex dependency graph:
    /* Tree structure:
        pass1
       /     \
     pass2   pass3
       \     /
        pass4
    */

    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");
    auto pass3 = createMockPass("Pass3");
    auto pass4 = createMockPass("Pass4");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);
    PassId id3 = pipeline.addRenderPass(pass3);
    PassId id4 = pipeline.addRenderPass(pass4);

    // Setup dependencies
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    pipeline.addPrerequisite(id3, id1);
    pipeline.addEffect(id1, id3);

    pipeline.addPrerequisite(id4, id2);
    pipeline.addEffect(id2, id4);

    pipeline.addPrerequisite(id4, id3);
    pipeline.addEffect(id3, id4);

    pipeline.setFinalOutputPass(id4);

    // Get execution plan
    auto plan = pipeline.createExecutionPlan();

    // Plan should have all passes, and pass1 must come before pass2 and pass3,
    // which must both come before pass4
    EXPECT_EQ(plan.size(), 4);

    // Find positions of each pass in the plan
    auto pos1 = std::find(plan.begin(), plan.end(), id1);
    auto pos2 = std::find(plan.begin(), plan.end(), id2);
    auto pos3 = std::find(plan.begin(), plan.end(), id3);
    auto pos4 = std::find(plan.begin(), plan.end(), id4);

    // Check all passes are in the plan
    EXPECT_NE(pos1, plan.end());
    EXPECT_NE(pos2, plan.end());
    EXPECT_NE(pos3, plan.end());
    EXPECT_NE(pos4, plan.end());

    // Check dependencies are respected
    EXPECT_LT(std::distance(plan.begin(), pos1), std::distance(plan.begin(), pos2));
    EXPECT_LT(std::distance(plan.begin(), pos1), std::distance(plan.begin(), pos3));
    EXPECT_LT(std::distance(plan.begin(), pos2), std::distance(plan.begin(), pos4));
    EXPECT_LT(std::distance(plan.begin(), pos3), std::distance(plan.begin(), pos4));
}

TEST_F(RenderPipelineTest, FinalOutputPassManagement) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);

    // Get what the implementation actually sets as the first final output
    int initialFinalOutput = pipeline.getFinalOutputPass();

    // Initial pass should be final
    EXPECT_EQ(initialFinalOutput, static_cast<int>(id1));

    // Change to pass2
    pipeline.setFinalOutputPass(id2);
    EXPECT_EQ(pipeline.getFinalOutputPass(), static_cast<int>(id2));

    // Remove pass2, pass1 should become final again
    pipeline.removeRenderPass(id2);
    EXPECT_EQ(pipeline.getFinalOutputPass(), static_cast<int>(id1));

    // Set final render target
    auto framebuffer = createMockFramebuffer();
    pipeline.setRenderTarget(framebuffer);
    EXPECT_EQ(pipeline.getRenderTarget(), framebuffer);
}

TEST_F(RenderPipelineTest, HasPrerequisitesAndEffects) {
    auto pass1 = createMockPass("Pass1");
    auto pass2 = createMockPass("Pass2");

    PassId id1 = pipeline.addRenderPass(pass1);
    PassId id2 = pipeline.addRenderPass(pass2);

    // Initially no connections
    EXPECT_FALSE(pipeline.hasPrerequisites(id1));
    EXPECT_FALSE(pipeline.hasPrerequisites(id2));
    EXPECT_FALSE(pipeline.hasEffects(id1));
    EXPECT_FALSE(pipeline.hasEffects(id2));

    // Add connections
    pipeline.addPrerequisite(id2, id1);
    pipeline.addEffect(id1, id2);

    // Check connections
    EXPECT_FALSE(pipeline.hasPrerequisites(id1));
    EXPECT_TRUE(pipeline.hasPrerequisites(id2));
    EXPECT_TRUE(pipeline.hasEffects(id1));
    EXPECT_FALSE(pipeline.hasEffects(id2));

    // Invalid ID should return false
    EXPECT_FALSE(pipeline.hasPrerequisites(9999));
    EXPECT_FALSE(pipeline.hasEffects(9999));
}

} // namespace nexo::renderer
