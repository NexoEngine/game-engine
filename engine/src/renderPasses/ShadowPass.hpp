// ShadowPass.hpp
#pragma once

#include "RenderPass.hpp"
#include "Framebuffer.hpp"

namespace nexo::renderer {

    class ShadowPass : public RenderPass {
    public:
        ShadowPass(unsigned int size);
        void execute(RenderPipeline &pipeline) override;
        void resize(unsigned int width, unsigned int height) override;
        std::shared_ptr<NxFramebuffer> getOutput() const override { return m_shadowMap; }

    private:
        std::shared_ptr<NxFramebuffer> m_shadowMap;
        unsigned int m_size;
    };

} // namespace nexo::renderer
