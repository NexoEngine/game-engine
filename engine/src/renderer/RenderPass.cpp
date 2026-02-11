#include "RenderPass.hpp"
#include "RenderPipeline.hpp"

namespace nexo::renderer {

    std::shared_ptr<NxFramebuffer> RenderPass::getPrerequisiteOutput(RenderPipeline &pipeline, const PassId targetPass) const
    {
        for (const auto prereq : prerequisites) {
            auto p = pipeline.getRenderPass(prereq);
            if (p && p->getId() == targetPass)
                return p->getOutput();
        }
        return nullptr;
    }

} // namespace nexo::renderer
