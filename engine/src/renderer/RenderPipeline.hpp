//// RenderPipeline.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the render pipeline
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <array>
#include "DrawCommand.hpp"
#include "Framebuffer.hpp"
#include "RenderPass.hpp"
#include "ShaderStorageBuffer.hpp"
#include "ShaderUniformBuffer.hpp"

namespace nexo::renderer {

    struct GpuPointShadowMap {
        unsigned int depthCube = 0;
        unsigned int fbo       = 0;
        float        farPlane  = 25.0f;
        bool         inUse     = false;
    };

    class RenderPipeline {
       public:
        /**
         *  @brief Adds a render pass to the rendering pipeline.
         *
         * This method allows you to add a new render pass to the pipeline. Each render pass
         * represents a distinct stage in the rendering process, such as shadow mapping,
         * post-processing effects, or final composition. By adding a render pass, you can
         * define the sequence of operations that will be executed during rendering.
         *
         * The method takes a shared pointer to a `RenderPass` object and returns a unique
         * identifier (PassId) for the added pass. This ID can be used later to reference
         * the pass for operations like setting prerequisites, effects, or removal.
         *
         * @param pass A shared pointer to the `RenderPass` object to be added to the pipeline.
         * @return A unique identifier (PassId) for the added render pass.
         */
        PassId addRenderPass(std::shared_ptr<RenderPass> pass);

        /**
         *  @brief Removes a render pass from the rendering pipeline and reconnects its dependencies.
         *
         * This method removes a specified render pass from the pipeline. Before removal, it saves the
         * prerequisites and effects of the pass to be removed. It then creates new relationships between
         * each prerequisite and each effect of the removed pass, effectively reconnecting the dependencies
         * in the pipeline. This ensures that the overall rendering sequence remains intact even after the
         * removal of a pass.
         *
         * If the specified render pass does not exist in the pipeline, the method does nothing.
         *
         * @param id The unique identifier (PassId) of the render pass to be removed.
         */
        void removeRenderPass(PassId id);

        /**
         *  @brief Adds a prerequisite to a specified render pass in the pipeline.
         *
         * This method establishes a relationship between two render passes, where one pass (the prerequisite)
         * must be executed before another pass (the dependent pass). By adding a prerequisite, you are
         * indicating that the specified render pass should be executed after the given prerequisite pass,
         * allowing for complex rendering sequences and dependencies within the pipeline.
         *
         * If either the specified render pass or prerequisite does not exist in the pipeline, the method does nothing.
         *
         * @param pass The unique identifier (PassId) of the render pass to which the prerequisite is being added.
         * @param prerequisite The unique identifier (PassId) of the prerequisite (render pass) that must be executed
         * before the given pass.
         */
        void addPrerequisite(PassId pass, PassId prerequisite);

        /**
         *  @brief Removes a prerequisite from a specified render pass in the pipeline.
         *
         * This method removes a specified prerequisite (another render pass) from the list of prerequisites
         * associated with a given render pass. If the specified render pass does not exist in the pipeline,
         * the method does nothing. Removing a prerequisite means that the specified render pass will no longer
         * be considered as a dependency that must be executed before the given render pass.
         *
         * @param pass The unique identifier (PassId) of the render pass from which to remove the prerequisite.
         * @param prerequisite The unique identifier (PassId) of the prerequisite (render pass) to be removed.
         */
        void removePrerequisite(PassId pass, PassId prerequisite);

        /**
         *  @brief Adds an effect to a specified render pass in the pipeline.
         *
         * This method establishes a relationship between two render passes, where one pass (the effect)
         * is dependent on the output of another pass (the prerequisite). By adding an effect, you are
         * indicating that the specified render pass should be executed after the given prerequisite pass,
         * allowing for complex rendering sequences and dependencies within the pipeline.
         *
         * If either the specified render pass or effect does not exist in the pipeline, the method does nothing.
         *
         * @param pass The unique identifier (PassId) of the render pass to which the effect is being added.
         * @param effect The unique identifier (PassId) of the effect (render pass) that depends on the output of the
         * given pass.
         */
        void addEffect(PassId pass, PassId effect);

        /**
         *  @brief Removes an effect from a specified render pass in the pipeline.
         *
         * This method removes a specified effect (another render pass) from the list of effects
         * associated with a given render pass. If the specified render pass or effect does not exist
         * in the pipeline, the method does nothing. Removing an effect means that the specified
         * render pass will no longer be considered as a subsequent pass that depends on the output
         * of the given render pass.
         *
         * @param pass The unique identifier (PassId) of the render pass from which to remove the effect.
         * @param effect The unique identifier (PassId) of the effect (render pass) to be removed.
         */
        void removeEffect(PassId pass, PassId effect);

        /**
         *  @brief Retrieves a render pass by its unique identifier.
         *
         * This method searches for a render pass within the pipeline using its unique identifier (PassId).
         * If a render pass with the specified ID exists, it returns a shared pointer to that render pass.
         * If no such render pass is found, it returns a null pointer.
         *
         * @param id The unique identifier of the render pass to retrieve.
         * @return A shared pointer to the render pass if found, or nullptr if not found.
         */
        std::shared_ptr<RenderPass> getRenderPass(PassId id);

        /**
         * @brief Sets the render target framebuffer for the pipeline.
         *
         * This method allows you to specify a framebuffer that will be used as the render target
         * for the rendering pipeline. The render target is where the final rendered image or output
         * will be directed. By setting a render target, you can control where the rendering results
         * are stored, whether it's to the screen, an off-screen buffer, or a texture.
         *
         * @param finalRenderTarget A shared pointer to the framebuffer to be used as the render target.
         */
        void setRenderTarget(std::shared_ptr<NxFramebuffer> finalRenderTarget);

        /**
         * @brief Retrieves the current render target framebuffer.
         *
         * This method returns a shared pointer to the framebuffer that is currently set as the render target
         * for the rendering pipeline. The render target is where the final rendered image or output will be
         * directed. If no render target has been set, this method may return a null pointer.
         *
         * @return A shared pointer to the current render target framebuffer, or nullptr if none is set.
         */
        [[nodiscard]] std::shared_ptr<NxFramebuffer> getRenderTarget() const;

        /**
         * @brief Sets the PassId of the final output render pass in the pipeline.
         *
         * This method designates a specific render pass as the final output pass in the rendering pipeline.
         * The final output pass is typically responsible for producing the final rendered image or result
         * that will be presented to the screen or further processed. By setting this pass, you ensure that
         * it is recognized as the last step in the rendering sequence.
         *
         * @param id The PassId of the render pass to be set as the final output pass.
         */
        void setFinalOutputPass(PassId id);

        /**
         * @brief Retrieves the PassId of the final output render pass in the pipeline.
         *
         * This method returns the unique identifier (PassId) of the render pass that is designated
         * as the final output pass in the rendering pipeline. The final output pass is typically
         * responsible for producing the final rendered image or result that will be presented to
         * the screen or further processed.
         *
         * @return The PassId of the final output render pass.
         */
        [[nodiscard]] PassId getFinalOutputPass() const
        {
            return finalOutputPass;
        }

        /**
         * @brief Creates an execution plan for the render pipeline.
         *
         * This method generates a valid execution order for the render passes in the pipeline,
         * taking into account their prerequisites and effects. The execution plan ensures that
         * all dependencies are respected, allowing for correct rendering results.
         *
         * The method uses a topological sorting algorithm to determine the order of execution.
         * If a cycle is detected in the dependencies, an exception is thrown to indicate that
         * the pipeline configuration is invalid.
         *
         * @throws Exception if a cycle is detected in the render pass dependencies.
         * @return A vector containing the ordered list of PassId representing the execution plan.
         */
        std::vector<PassId> createExecutionPlan();

        /**
         * @brief Executes the render pipeline according to the established execution plan.
         *
         * This method processes each render pass in the order defined by the execution plan,
         * ensuring that all prerequisites are met before executing each pass. The final output
         * is directed to the designated render target framebuffer.
         *
         * If the execution plan is marked as dirty (i.e., it has changed since the last execution),
         * the method will regenerate the plan before proceeding with execution.
         *
         * @throws Exception if any render pass fails during execution.
         */
        void execute();

        /**
         * @brief Finds all terminal render passes in the pipeline.
         *
         * This method identifies and returns a list of all terminal render passes within the pipeline.
         * A terminal render pass is defined as a pass that does not have any effects, meaning it does
         * not lead to any subsequent passes. Terminal passes are typically the final steps in the
         * rendering process, producing the final output or results.
         *
         * @return A vector containing the unique identifiers (PassId) of all terminal render passes.
         */
        [[nodiscard]] std::vector<PassId> findTerminalPasses() const;

        /**
         * @brief Checks if a render pass has any prerequisites.
         *
         * This method determines whether the specified render pass has any prerequisites associated with it.
         * Prerequisites are other passes that must be executed before the current pass can run. If a pass has
         * prerequisites, it indicates that there are dependencies in the rendering pipeline that need to be
         * resolved first.
         *
         * @param id The unique identifier of the render pass to check.
         * @return True if the render pass has prerequisites, false otherwise.
         */
        [[nodiscard]] bool hasPrerequisites(PassId id) const;

        /**
         * @brief Checks if a render pass has any effects.
         *
         * This method determines whether the specified render pass has any effects associated with it.
         * Effects are subsequent passes that depend on the output of the current pass. If a pass has
         * effects, it indicates that there are other passes in the pipeline that utilize its results.
         *
         * @param id The unique identifier of the render pass to check.
         * @return True if the render pass has effects
         **/
        [[nodiscard]] bool hasEffects(PassId id) const;

        /**
         * @brief Adds multiple draw commands to the render pipeline.
         *
         * This method allows you to add a collection of `DrawCommand` objects to the render pipeline.
         * Each `DrawCommand` represents a specific drawing operation that will be executed during
         * the rendering process. By adding multiple commands at once, you can efficiently batch
         * rendering operations.
         *
         * @param drawCommands A vector of `DrawCommand` objects to be added to the pipeline.
         */
        void addDrawCommands(const std::vector<DrawCommand> &drawCommands);

        /**
         * @brief Adds a single draw command to the render pipeline.
         *
         * This method allows you to add an individual `DrawCommand` object to the render pipeline.
         * The `DrawCommand` represents a specific drawing operation that will be executed during
         * the rendering process. This is useful for adding commands one at a time.
         *
         * @param drawCommand A `DrawCommand` object to be added to the pipeline.
         */
        void addDrawCommand(const DrawCommand &drawCommand);

        /**
         * @brief Retrieves the list of draw commands added to the pipeline.
         *
         * This method returns a constant reference to the vector of `DrawCommand` objects
         * that have been added to the render pipeline. These commands represent the drawing
         * operations that will be executed during the rendering process.
         *
         * @return A constant reference to a vector of `DrawCommand` objects.
         */
        [[nodiscard]] std::vector<DrawCommand> &getDrawCommands();

        /**
         * @brief Sets the clear color used by the camera.
         *
         * This method allows you to specify the RGBA color vector that will be used to clear the screen
         * at the start of each frame. The clear color is typically set to a background color for the scene.
         *
         * @param clearColor A `glm::vec4` representing the clear color in RGBA format.
         */
        void setCameraClearColor(const glm::vec4 &clearColor);

        /**
         * @brief Retrieves the clear color used by the camera.
         *
         * This method returns the RGBA color vector that is used to clear the screen
         * at the start of each frame. The clear color is typically set to a background
         * color for the scene.
         *
         * @return A constant reference to a `glm::vec4` representing the clear color.
         */
        [[nodiscard]] const glm::vec4 &getCameraClearColor() const;

        /**
         * @brief Resizes the render target and all associated render passes.
         *
         * This method adjusts the dimensions of the framebuffer used as the final render target,
         * as well as resizing all render passes that depend on it.
         *
         * @param width The new width for the render target and passes.
         * @param height The new height for the render target and passes.
         * If no render target is set, the method exits early without performing any actions.
         */
        void resize(unsigned int width, unsigned int height) const;

        /**
         * @brief Sets global uniforms that apply to all draw commands in this pipeline.
         *
         * Global uniforms are set once per pipeline execution and include things like
         * camera matrices, lighting data, and other scene-wide parameters.
         *
         * @param uniforms Map of uniform names to values
         */
        void setGlobalUniforms(const std::unordered_map<std::string, UniformValue> &uniforms);

        /**
         * @brief Adds a single global uniform.
         *
         * @param name The uniform name
         * @param value The uniform value
         */
        void setGlobalUniform(const std::string &name, const UniformValue &value);

        /**
         * @brief Gets the current global uniforms.
         *
         * @return Reference to the global uniforms map
         */
        [[nodiscard]] const std::unordered_map<std::string, UniformValue> &getGlobalUniforms() const;

        /**
         * @brief Clears all global uniforms.
         */
        void clearGlobalUniforms();

        void addUniformBuffer(const std::string &name, unsigned int bindingLocation, const std::shared_ptr<renderer::NxShaderUniformBuffer> &buffer);

        void addStorageBuffer(const std::string &name, unsigned int bindingLocation, const std::shared_ptr<renderer::NxShaderStorageBuffer> &buffer);

        void setUniformBufferData(const std::string &name, void *data, unsigned int size);
        void setStorageBufferData(const std::string &name, void *data, unsigned int size);
        void appendStorageBufferData(const std::string &name, void *data, unsigned int size);
        unsigned int getStorageBufferSize(const std::string &name);

        struct RendererPointLight {
            glm::vec3 position;
            float farPlane;
        };
        void setupShadowMaps();
        GpuPointShadowMap &getShadowMap(unsigned int index);
        void addPointLight(const glm::vec3 &pos, float farPlane);
        void resetPointLigths();
        unsigned int getNbPointLights() const;
        const RendererPointLight &getPointLight(unsigned int index) const;

       private:
        std::vector<DrawCommand> m_drawCommands;
        glm::vec4 m_cameraClearColor{};
        std::vector<PassId> m_plan{};
        bool m_isDirty = true;

        // Store all render passes
        std::unordered_map<PassId, std::shared_ptr<RenderPass>> passes;
        std::unordered_map<std::string, UniformValue> m_globalUniforms;

        std::unordered_map<std::string, std::shared_ptr<renderer::NxShaderUniformBuffer>> m_ubos;
        std::unordered_map<std::string, std::shared_ptr<renderer::NxShaderStorageBuffer>> m_ssbos;
        std::unordered_map<std::string, unsigned int> m_bufferBindingLocations;

        static constexpr unsigned int POINT_SHADOW_SIZE = 1024;
        std::array<GpuPointShadowMap, 10> pointShadowMaps;
        std::array<RendererPointLight, 10> pointLights;
        unsigned int nbPointLights = 0;


        // For generating unique IDs
        PassId nextPassId = 0;

        std::shared_ptr<NxFramebuffer> m_renderTarget = nullptr;

        // The final output pass (what gets rendered to screen)
        int finalOutputPass = -1;
    };
} // namespace nexo::renderer
