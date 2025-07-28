//// RenderPipeline.hpp ///////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the render pipeline
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Framebuffer.hpp"
#include "RenderPass.hpp"
#include "DrawCommand.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

namespace nexo::renderer {

    class RenderPipeline {
        public:
            // Add a render pass to the pipeline
            PassId addRenderPass(std::shared_ptr<RenderPass> pass);

            // Remove a render pass from the pipeline and automatically reconnect dependencies
            void removeRenderPass(PassId id);

            // Add a prerequisite
            void addPrerequisite(PassId pass, PassId prerequisite);

            // Remove a prerequisite
            void removePrerequisite(PassId pass, PassId prerequisite);

            // Add an effect
            void addEffect(PassId pass, PassId effect);

            // Remove an effect
            void removeEffect(PassId pass, PassId effect);

            // Get a render pass by ID
            std::shared_ptr<RenderPass> getRenderPass(PassId id);

            // Get output from a pass
            std::shared_ptr<NxFramebuffer> getOutput(PassId id);

            // Set output for a pass
            void setOutput(PassId id, const std::shared_ptr<NxFramebuffer>& output);

            void setFinalRenderTarget(const std::shared_ptr<NxFramebuffer>& finalRenderTarget);
            std::shared_ptr<NxFramebuffer> getFinalRenderTarget() const;

            // Set the final output pass
            void setFinalOutputPass(PassId id);

            // Get the final output pass
            PassId getFinalOutputPass() const { return finalOutputPass; }

            // Calculate execution plan using GOAP
            std::vector<PassId> createExecutionPlan();

            // Execute the pipeline
            void execute();

            // Find terminal passes (passes with no effects)
            std::vector<PassId> findTerminalPasses() const;

            // Check if a pass has prerequisites
            bool hasPrerequisites(PassId id) const;

            // Check if a pass has effects
            bool hasEffects(PassId id) const;

            void addDrawCommands(const std::vector<DrawCommand> &drawCommands);
            void addDrawCommand(const DrawCommand &drawCommand);
            const std::vector<DrawCommand> &getDrawCommands() const;

            void setCameraClearColor(const glm::vec4 &clearColor);
            const glm::vec4 &getCameraClearColor() const;

            void resize(unsigned int width, unsigned int height) const;

        private:
            std::vector<DrawCommand> m_drawCommands;
            glm::vec4 m_cameraClearColor{};
            std::vector<PassId> m_plan{};
            bool m_isDirty = true;

            // Store all render passes
            std::unordered_map<PassId, std::shared_ptr<RenderPass>> passes;

            // For generating unique IDs
            PassId nextPassId = 0;

            // Stores outputs of passes
            std::unordered_map<PassId, std::shared_ptr<NxFramebuffer>> passOutputs;

            std::shared_ptr<NxFramebuffer> m_finalRenderTarget = nullptr;

            // The final output pass (what gets rendered to screen)
            int finalOutputPass = -1;
    };
}
