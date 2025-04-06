#pragma once

#include <format>
#include <string>
#include <string_view>
#include <regex>
#include <optional>

#include "Exception.hpp"

namespace nexo::assets {

    /**
     * @class InvalidName
     *
     * @brief Exception thrown when a name fails validation.
     */
    class InvalidName final : public Exception {
    public:
        explicit InvalidName(
            std::string_view name,
            std::string_view message,
            const std::source_location loc = std::source_location::current()
        ) : Exception(std::format("Invalid name '{}': {}", name, message), loc) {}
    };

    template <typename T>
    concept Validator = requires(std::string_view name) {
        { T::validate(name) } -> std::same_as<std::optional<std::string>>; //< The validation method.
    };

    /**
     * @class ValidatedName
     *
     * @brief Base class for validated names.
     */
    template <Validator TValidator>
    class ValidatedName {
        public:
            virtual ~ValidatedName() = default;

            // Constructors
            explicit ValidatedName(const std::string_view name) : _value(name) {
                if (auto errorMessage = TValidator::validate(name); errorMessage.has_value()) {
                    _value = "Unnamed";
                    THROW_EXCEPTION(InvalidName, name, errorMessage.value());
                }
            }
            explicit(false) ValidatedName(const std::string& name) : ValidatedName(std::string_view(name)) {}
            explicit(false) ValidatedName(const char* name) : ValidatedName(std::string_view(name)) {}




            /**
             * @brief Returns the size of the name.
             */
            [[nodiscard]] std::size_t size() const { return _value.size(); }

            /**
             * @brief Implicit conversions for convenience.
             */

            explicit operator std::string() const { return _value; }
            explicit operator std::string_view() const { return _value; }
            explicit operator const char*() const { return _value.c_str(); }

            /**
             * @brief Returns the underlying name as a string.
             */
            [[nodiscard]] const std::string& data() const { return _value; }
            [[nodiscard]] const char* c_str() const { return _value.c_str(); }


            /**
             * @brief Equality and inequality operators.
             */
            bool operator==(const ValidatedName& other) const { return _value == other._value; }
            bool operator!=(const ValidatedName& other) const { return !(*this == other); }

            ValidatedName& operator=(const ValidatedName& other) = default;
            ValidatedName& operator=(std::string_view name);
            ValidatedName& operator=(const std::string& name);

            ValidatedName& operator=(const char* name);

            std::optional<std::string> rename(std::string_view name);

            /**
             * @brief Validates a name.
             * @param name The name to validate.
             * @return An error message if the name is invalid, or std::nullopt if it is valid.
             * @note This function is static and can be used to validate a name without creating an instance.
             */
            static std::optional<std::string> validate(std::string_view name) {
                return TValidator::validate(name);
            }

        private:

            std::string _value; //< The validated name value.
    };

    template<Validator TValidator>
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            THROW_EXCEPTION(InvalidName, name, errorMessage.value());
        _value = name;
        return *this;
    }

    template<Validator TValidator>
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const std::string& name)
    {
        *this = std::string_view(name);
        return *this;
    }

    template<Validator TValidator>
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const char* name)
    {
        this->operator=(std::string_view(name));
        return *this;
    }

    template<Validator TValidator>
    std::optional<std::string> ValidatedName<TValidator>::rename(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            return errorMessage;
        _value = name;
        return std::nullopt;
    }
} // namespace nexo::assets
