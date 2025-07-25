//// RenderPass.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the render pass interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <vector>
#include <cstdint>


namespace nexo::renderer {
    using PassId = uint32_t;

    class RenderPipeline;

    class RenderPass {
        public:
            RenderPass(PassId id, const std::string& debugName = "") : id(id), name(debugName) {}
            virtual ~RenderPass() = default;

            // The actual rendering work
            virtual void execute(RenderPipeline& pipeline) = 0;
            virtual void resize(unsigned int width, unsigned int height) = 0;
            void setFinal(bool isFinal) {m_isFinal = isFinal;};
            bool isFinal() const {return m_isFinal;}

            PassId getId() const { return id; }
            const std::string& getName() const { return name; }
            std::vector<PassId> &getPrerequisites() { return prerequisites; }
            const std::vector<PassId> &getPrerequisites() const { return prerequisites; }
            std::vector<PassId> &getEffects() { return effects; }
            const std::vector<PassId> &getEffects() const { return effects; }
        protected:
            bool m_isFinal = false;
            PassId id;
            std::string name;

            // Prerequisites - which passes must run before this one
            std::vector<PassId> prerequisites;
            // Effects - which passes this one enables
            std::vector<PassId> effects;
    };
}
