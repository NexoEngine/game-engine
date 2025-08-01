//// RenderPipeline.cpp ///////////////////////////////////////////////////////
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
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the render pipeline
//
///////////////////////////////////////////////////////////////////////////////
#include "RenderPipeline.hpp"
#include "Exception.hpp"
#include "Framebuffer.hpp"
#include "RenderCommand.hpp"
#include "RendererExceptions.hpp"
#include "Renderer3D.hpp"
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

        // Save dependencies before removal
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

        // Erase the pass
        passes.erase(id);

        // Choose new final output if necessary
        if (needNewFinalOutput) {
            if (passes.empty()) {
                finalOutputPass = -1;
            } else {
                // Prefer terminal passes (those with no effects)
                const auto terminalPasses = findTerminalPasses();
                if (!terminalPasses.empty()) {
                    setFinalOutputPass(terminalPasses[0]);
                } else {
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

    void RenderPipeline::setRenderTarget(std::shared_ptr<NxFramebuffer> renderTarget)
    {
        m_renderTarget = std::move(renderTarget);
    }

    std::shared_ptr<NxFramebuffer> RenderPipeline::getRenderTarget() const
    {
        return m_renderTarget;
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
            visited.insert(current);
            result.push_back(current);
        };

        if (finalOutputPass != -1 && passes.contains(finalOutputPass)) {
            buildPlan(finalOutputPass);
        } else {
            auto terminals = findTerminalPasses();
            if (terminals.empty()) {
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

        if (!m_renderTarget)
            THROW_EXCEPTION(NxPipelineRenderTargetNotSetException);

        for (PassId id : m_plan) {
            if (passes.contains(id))
                passes[id]->execute(*this);
        }
        m_drawCommands.clear();
    }

    void RenderPipeline::addDrawCommands(const std::vector<DrawCommand>& drawCommands)
    {
        m_drawCommands.reserve(m_drawCommands.size() + drawCommands.size());
        m_drawCommands.insert(m_drawCommands.end(), drawCommands.begin(), drawCommands.end());
    }

    void RenderPipeline::addDrawCommand(const DrawCommand& drawCommand)
    {
        m_drawCommands.push_back(drawCommand);
    }

    const std::vector<DrawCommand>& RenderPipeline::getDrawCommands() const
    {
        return m_drawCommands;
    }

    void RenderPipeline::setCameraClearColor(const glm::vec4& clearColor)
    {
        m_cameraClearColor = clearColor;
    }

    const glm::vec4& RenderPipeline::getCameraClearColor() const
    {
        return m_cameraClearColor;
    }

    void RenderPipeline::resize(const unsigned int width, const unsigned int height) const
    {
        if (!m_renderTarget)
            return;
        m_renderTarget->resize(width, height);
        for (const auto& [_, pass] : passes)
            pass->resize(width, height);
    }
}
