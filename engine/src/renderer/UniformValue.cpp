#include "UniformValue.hpp"
#include "Shader.hpp"

namespace nexo::renderer {

    // Template specializations for applying different types to shader
    template<>
    void UniformValue::Holder<float>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformFloat(name, value);
    }

    template<>
    void UniformValue::Holder<int>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformInt(name, value);
    }

    template<>
    void UniformValue::Holder<bool>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformBool(name, value);
    }

    template<>
    void UniformValue::Holder<glm::vec2>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformFloat2(name, value);
    }

    template<>
    void UniformValue::Holder<glm::vec3>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformFloat3(name, value);
    }

    template<>
    void UniformValue::Holder<glm::vec4>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformFloat4(name, value);
    }

    template<>
    void UniformValue::Holder<glm::mat4>::applyToShader(NxShader* shader, const std::string& name) const {
        shader->setUniformMatrix(name, value);
    }
}
