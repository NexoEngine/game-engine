//// RenderPipeline.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the render pipeline
//
///////////////////////////////////////////////////////////////////////////////
#include "RenderPipeline.hpp"
#include "Framebuffer.hpp"
#include <functional>
#include <set>
#include <utility>

namespace nexo::renderer {
    PassId RenderPipeline::addRenderPass(std::shared_ptr<RenderPass> pass)
    {
        // If this is the first pass, set it as the final output
        const PassId id = pass->getId();
        passes[id] = std::move(pass);
        if (passes.size() == 1)
            setFinalOutputPass(id);
        m_isDirty = true;
        return id;
    }

    void RenderPipeline::removeRenderPass(const PassId id)
    {
        if (!passes.contains(id))
            return;
        const auto &pass = passes[id];

        // Save prerequisites and effects before removal
        std::vector<PassId> prerequisites = pass->getPrerequisites();
        std::vector<PassId> effects = pass->getEffects();

        // For each prerequisite -> effect pair, create a new relationship
        for (const PassId prereqId : prerequisites) {
            for (const PassId effectId : effects) {
                addPrerequisite(effectId, prereqId);
                addEffect(prereqId, effectId);
            }
        }

        // Remove this pass from all prerequisites lists
        for (const auto &[passId, p] : passes) {
            auto& prereqs = p->getPrerequisites();
            std::erase(prereqs, id);
        }

        // Remove this pass from all effects lists
        for (const auto& [passId, p] : passes) {
            auto& effs = p->getEffects();
            std::erase(effs, id);
        }

        // First remove the pass, then find a new final output if needed
        const bool needNewFinalOutput = (finalOutputPass == static_cast<int>(id));

        // Remove the pass from the maps
        passes.erase(id);
        if (passOutputs.contains(id))
            passOutputs.erase(id);

        // Now, after removing the pass, find a new final output if needed
        if (needNewFinalOutput) {
            // If there are no passes left, set finalOutputPass to -1 directly
            if (passes.empty()) {
                finalOutputPass = -1;
            } else {
                // Prefer terminal passes (those with no effects)
                const auto terminalPasses = findTerminalPasses();
                if (!terminalPasses.empty()) {
                    setFinalOutputPass(terminalPasses[0]);
                } else {
                    // Fallback to any pass
                    setFinalOutputPass(passes.begin()->first);
                }
            }
        }
        m_isDirty = true;
    }

    void RenderPipeline::addPrerequisite(const PassId pass, const PassId prerequisite)
    {
        if (!passes.contains(pass) || !passes.contains(prerequisite))
            return;

        auto& prereqs = passes[pass]->getPrerequisites();
        if (std::find(prereqs.begin(), prereqs.end(), prerequisite) == prereqs.end())
            prereqs.push_back(prerequisite);
        m_isDirty = true;
    }

    void RenderPipeline::removePrerequisite(const PassId pass, const PassId prerequisite)
    {
        if (!passes.contains(pass))
            return;

        auto& prereqs = passes[pass]->getPrerequisites();
        std::erase(prereqs, prerequisite);
        m_isDirty = true;
    }

    void RenderPipeline::addEffect(const PassId pass, const PassId effect)
    {
        if (!passes.contains(pass) || !passes.contains(effect))
            return;

        auto& effects = passes[pass]->getEffects();
        if (std::find(effects.begin(), effects.end(), effect) == effects.end())
            effects.push_back(effect);
        m_isDirty = true;
    }

    void RenderPipeline::removeEffect(const PassId pass, const PassId effect)
    {
        if (!passes.contains(pass))
            return;

        auto& effects = passes[pass]->getEffects();
        std::erase(effects, effect);
        m_isDirty = true;
    }

    std::shared_ptr<RenderPass> RenderPipeline::getRenderPass(const PassId id)
    {
        const auto it = passes.find(id);
        if (it != passes.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<NxFramebuffer> RenderPipeline::getOutput(const PassId id)
    {
        auto it = passOutputs.find(id);
        if (it != passOutputs.end())
            return it->second;
        return nullptr;
    }

    void RenderPipeline::setOutput(const PassId id, const std::shared_ptr<NxFramebuffer>& output)
    {
        passOutputs[id] = output;
    }

    void RenderPipeline::setFinalOutputPass(const PassId id)
    {
        if (passes.contains(id)) {
            if (finalOutputPass != -1 && passes.contains(finalOutputPass))
                passes[finalOutputPass]->setFinal(false);

            passes[id]->setFinal(true);
            finalOutputPass = static_cast<int>(id);
        }
    }

    void RenderPipeline::setFinalRenderTarget(const std::shared_ptr<NxFramebuffer> &finalRenderTarget)
    {
        m_finalRenderTarget = finalRenderTarget;
    }

    std::shared_ptr<NxFramebuffer> RenderPipeline::getFinalRenderTarget() const
    {
        return m_finalRenderTarget;
    }

    std::vector<PassId> RenderPipeline::findTerminalPasses() const
    {
        std::vector<PassId> terminals;
        for (const auto& [id, pass] : passes) {
            if (pass->getEffects().empty()) {
                terminals.push_back(id);
            }
        }
        return terminals;
    }

    bool RenderPipeline::hasPrerequisites(const PassId id) const
    {
        const auto it = passes.find(id);
        return it != passes.end() && !it->second->getPrerequisites().empty();
    }

    bool RenderPipeline::hasEffects(const PassId id) const
    {
        const auto it = passes.find(id);
        return it != passes.end() && !it->second->getEffects().empty();
    }

    std::vector<PassId> RenderPipeline::createExecutionPlan()
    {
        std::vector<PassId> result;

        // Early return if there are no passes
        if (passes.empty()) {
            m_isDirty = false;
            return result;
        }

        std::set<PassId> visited;
        // DFS helper to build execution plan
        std::function<void(PassId)> buildPlan = [&](const PassId current) {
            if (visited.contains(current))
                return;

            // First process all prerequisites
            for (const PassId prereq : passes[current]->getPrerequisites()) {
                if (passes.contains(prereq))
                    buildPlan(prereq);
            }

            // Then add this pass
            visited.insert(current);
            result.push_back(current);
        };

        // Start with the final output pass if it exists
        if (finalOutputPass != -1 && passes.contains(finalOutputPass)) {
            buildPlan(finalOutputPass);
        } else {
            // Fallback to processing all terminal passes
            auto terminals = findTerminalPasses();
            if (terminals.empty()) {
                // If no terminal passes, include all passes
                for (const auto& [id, _] : passes)
                    terminals.push_back(id);
            }

            // Process each terminal pass
            for (const PassId term : terminals)
                buildPlan(term);
        }
        m_isDirty = false;
        return result;
    }

    void RenderPipeline::execute()
    {
        if (m_isDirty)
            m_plan = createExecutionPlan();

        for (PassId id : m_plan) {
            if (passes.contains(id))
                passes[id]->execute(*this);
        }
        m_drawCommands.clear();
    }

    void RenderPipeline::addDrawCommands(const std::vector<DrawCommand> &drawCommands)
    {
        m_drawCommands.reserve(m_drawCommands.size() + drawCommands.size());
        m_drawCommands.insert(m_drawCommands.end(), drawCommands.begin(), drawCommands.end());
    }

    void RenderPipeline::addDrawCommand(const DrawCommand &drawCommand)
    {
        m_drawCommands.push_back(drawCommand);
    }

    const std::vector<DrawCommand> &RenderPipeline::getDrawCommands() const
    {
        return m_drawCommands;
    }

    void RenderPipeline::setCameraClearColor(const glm::vec4 &clearColor)
    {
        m_cameraClearColor = clearColor;
    }

    const glm::vec4 &RenderPipeline::getCameraClearColor() const
    {
        return m_cameraClearColor;
    }

    void RenderPipeline::resize(const unsigned int width, const unsigned int height) const
    {
        if (!m_finalRenderTarget)
            return;
        m_finalRenderTarget->resize(width, height);
        for (const auto &[_, pass] : passes)
            pass->resize(width, height);
    }
}
