#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <string>

namespace nexo::renderer {

    // Type-erased uniform value holder
    class UniformValue {
    public:
        // Default constructor - creates empty/invalid uniform
        UniformValue() : m_holder(nullptr) {}

        template<typename T>
        UniformValue(T&& value) : m_holder(std::make_unique<Holder<std::decay_t<T>>>(std::forward<T>(value))) {}

        UniformValue(const UniformValue& other) : m_holder(other.m_holder ? other.m_holder->clone() : nullptr) {}
        UniformValue(UniformValue&&) = default;

        UniformValue& operator=(const UniformValue& other) {
            if (this != &other) {
                m_holder = other.m_holder ? other.m_holder->clone() : nullptr;
            }
            return *this;
        }
        UniformValue& operator=(UniformValue&&) = default;

        // Check if the uniform value is valid
        bool isValid() const { return m_holder != nullptr; }

        // Apply the uniform to a shader
        void applyToShader(class NxShader* shader, const std::string& name) const {
            if (m_holder) {
                m_holder->applyToShader(shader, name);
            }
        }

    private:
        struct HolderBase {
            virtual ~HolderBase() = default;
            virtual std::unique_ptr<HolderBase> clone() const = 0;
            virtual void applyToShader(NxShader* shader, const std::string& name) const = 0;
        };

        template<typename T>
        struct Holder : HolderBase {
            T value;

            explicit Holder(T v) : value(std::move(v)) {}

            std::unique_ptr<HolderBase> clone() const override {
                return std::make_unique<Holder<T>>(value);
            }

            void applyToShader(NxShader* shader, const std::string& name) const override;
        };

        std::unique_ptr<HolderBase> m_holder;
    };
}
