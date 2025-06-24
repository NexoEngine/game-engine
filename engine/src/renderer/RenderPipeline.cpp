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

namespace nexo::renderer {
    PassId RenderPipeline::addRenderPass(std::shared_ptr<RenderPass> pass)
    {
        PassId id = nextPassId++;
        pass->id = id;
        passes[id] = std::move(pass);

        // If this is the first pass, set it as the final output
        if (passes.size() == 1)
            setFinalOutputPass(id);
        m_isDirty = true;
        return id;
    }

    void RenderPipeline::removeRenderPass(PassId id)
    {
        if (passes.find(id) == passes.end())
            return;

        auto& pass = passes[id];

        // Save prerequisites and effects before removal
        std::vector<PassId> prerequisites = pass->prerequisites;
        std::vector<PassId> effects = pass->effects;

        // For each prerequisite -> effect pair, create a new relationship
        for (PassId prereqId : prerequisites) {
            for (PassId effectId : effects) {
                // Connect prerequisite directly to effect
                addPrerequisite(effectId, prereqId);
                addEffect(prereqId, effectId);
            }
        }

        // Remove this pass from all prerequisites lists
        for (auto& [passId, p] : passes) {
            auto& prereqs = p->prerequisites;
            prereqs.erase(
                std::remove(prereqs.begin(), prereqs.end(), id),
                prereqs.end()
            );
        }

        // Remove this pass from all effects lists
        for (auto& [passId, p] : passes) {
            auto& effs = p->effects;
            effs.erase(
                std::remove(effs.begin(), effs.end(), id),
                effs.end()
            );
        }

        // If this was the final output pass, find a new one
        if (finalOutputPass == static_cast<int>(id)) {
            // Prefer terminal passes (those with no effects)
            auto terminalPasses = findTerminalPasses();
            if (!terminalPasses.empty()) {
                setFinalOutputPass(terminalPasses[0]);
            } else if (!passes.empty()) {
                // Fallback to any pass if no terminal passes
                setFinalOutputPass(passes.begin()->first);
            } else {
                finalOutputPass = -1;
            }
        }

        passes.erase(id);
        if (passOutputs.find(id) != passOutputs.end())
            passOutputs.erase(id);
        m_isDirty = true;
    }

    void RenderPipeline::addPrerequisite(PassId pass, PassId prerequisite)
    {
        if (passes.find(pass) == passes.end() || passes.find(prerequisite) == passes.end())
            return;

        auto& prereqs = passes[pass]->prerequisites;
        if (std::find(prereqs.begin(), prereqs.end(), prerequisite) == prereqs.end())
            prereqs.push_back(prerequisite);
        m_isDirty = true;
    }

    void RenderPipeline::removePrerequisite(PassId pass, PassId prerequisite)
    {
        if (passes.find(pass) == passes.end())
            return;

        auto& prereqs = passes[pass]->prerequisites;
        prereqs.erase(
            std::remove(prereqs.begin(), prereqs.end(), prerequisite),
            prereqs.end()
        );
        m_isDirty = true;
    }

    void RenderPipeline::addEffect(PassId pass, PassId effect)
    {
        if (passes.find(pass) == passes.end() || passes.find(effect) == passes.end())
            return;

        auto& effects = passes[pass]->effects;
        if (std::find(effects.begin(), effects.end(), effect) == effects.end())
            effects.push_back(effect);
        m_isDirty = true;
    }

    void RenderPipeline::removeEffect(PassId pass, PassId effect)
    {
        if (passes.find(pass) == passes.end())
            return;

        auto& effects = passes[pass]->effects;
        effects.erase(
            std::remove(effects.begin(), effects.end(), effect),
            effects.end()
        );
        m_isDirty = true;
    }

    std::shared_ptr<RenderPass> RenderPipeline::getRenderPass(PassId id)
    {
        auto it = passes.find(id);
        if (it != passes.end())
            return it->second;
        return nullptr;
    }

    std::shared_ptr<NxFramebuffer> RenderPipeline::getOutput(PassId id)
    {
        auto it = passOutputs.find(id);
        if (it != passOutputs.end())
            return it->second;
        return nullptr;
    }

    void RenderPipeline::setOutput(PassId id, std::shared_ptr<NxFramebuffer> output)
    {
        passOutputs[id] = output;
    }

    void RenderPipeline::setFinalOutputPass(PassId id)
    {
        if (passes.find(id) != passes.end()) {
            if (finalOutputPass != -1)
                passes[finalOutputPass]->setFinal(false);
            passes[id]->setFinal(true);
            finalOutputPass = id;
        }
    }

    void RenderPipeline::setFinalRenderTarget(std::shared_ptr<NxFramebuffer> finalRenderTarget)
    {
        m_finalRenderTarget = finalRenderTarget;
    }

    std::shared_ptr<NxFramebuffer> RenderPipeline::getFinalRenderTarget()
    {
        return m_finalRenderTarget;
    }

    std::vector<PassId> RenderPipeline::findTerminalPasses() const
    {
        std::vector<PassId> terminals;
        for (const auto& [id, pass] : passes) {
            if (pass->effects.empty()) {
                terminals.push_back(id);
            }
        }
        return terminals;
    }

    bool RenderPipeline::hasPrerequisites(PassId id) const
    {
        auto it = passes.find(id);
        return it != passes.end() && !it->second->prerequisites.empty();
    }

    bool RenderPipeline::hasEffects(PassId id) const
    {
        auto it = passes.find(id);
        return it != passes.end() && !it->second->effects.empty();
    }

    std::vector<PassId> RenderPipeline::createExecutionPlan()
    {
        std::vector<PassId> result;
        std::set<PassId> visited;
        // DFS helper to build execution plan
        std::function<void(PassId)> buildPlan = [&](PassId current) {
            if (visited.find(current) != visited.end())
                return;  // Already processed

            // First process all prerequisites
            for (PassId prereq : passes[current]->prerequisites) {
                if (passes.find(prereq) != passes.end())
                    buildPlan(prereq);
            }

            // Then add this pass
            visited.insert(current);
            result.push_back(current);
        };

        // Start with the final output pass if it exists
        if (finalOutputPass != -1 && passes.find(finalOutputPass) != passes.end()) {
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
            for (PassId term : terminals)
                buildPlan(term);
        }
        m_isDirty = false;
        return result;
    }

    void RenderPipeline::execute()
    {
        if (m_isDirty)
            m_plan = createExecutionPlan();

        // Execute passes in order
        for (PassId id : m_plan) {
            if (passes.find(id) != passes.end())
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

    void RenderPipeline::resize(unsigned int width, unsigned int height)
    {
        if (!m_finalRenderTarget)
            return;
        m_finalRenderTarget->resize(width, height);
        for (const auto &[_, pass] : passes)
            pass->resize(width, height);
    }
}
