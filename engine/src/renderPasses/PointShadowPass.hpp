#pragma once

#include "RenderPass.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo::renderer {

    class PointShadowPass : public RenderPass {
    public:
        PointShadowPass();

        void execute(RenderPipeline& pipeline) override;
        void resize(unsigned int width, unsigned int height) override {} // we keep fixed size shadows

        std::shared_ptr<NxFramebuffer> getOutput() const override { return nullptr; }

    private:
        // We use GPUResources::pointShadowMaps instead of NxFramebuffer here
    };

}
