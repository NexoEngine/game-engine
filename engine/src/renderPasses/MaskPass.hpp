//// MaskPass.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the mask render pass
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/Framebuffer.hpp"
#include "renderer/RenderPass.hpp"

namespace nexo::renderer {

    class MaskPass : public RenderPass {
       public:
        MaskPass(unsigned int width, unsigned int height);
        ~MaskPass() override = default;

     /**
         * @brief Executes the mask rendering pass.
         *
         * This method binds the internal framebuffer, clears it, and executes all draw commands
         * that have the `F_OUTLINE_MASK` filter mask set. After rendering, it unbinds the framebuffer.
         *
         * @param pipeline The render pipeline containing draw commands and render target information.
         */
        void execute(RenderPipeline& pipeline) override;

      /**
         * @brief Resizes the internal framebuffer to the specified dimensions.
         *
         * This method adjusts the size of the internal framebuffer used for rendering the mask.
         * It should be called whenever the viewport or render target size changes to ensure
         * correct rendering.
         *
         * @param width The new width of the framebuffer in pixels.
         * @param height The new height of the framebuffer in pixels.
         */
        void resize(unsigned int width, unsigned int height) override;

        /**
         * @brief Retrieves the output framebuffer containing the rendered mask.
         *
         * This method returns a shared pointer to the internal framebuffer that holds the
         * result of the mask rendering pass. This framebuffer can be used as input for
         * subsequent rendering passes or post-processing effects.
         *
         * @return A shared pointer to the `NxFramebuffer` containing the mask output.
         */
        [[nodiscard]] std::shared_ptr<NxFramebuffer> getOutput() const override;

       private:
        std::shared_ptr<NxFramebuffer> m_mask;
    };
} // namespace nexo::renderer
