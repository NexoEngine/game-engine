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
        /**
         * @brief Constructs an InvalidName exception.
         *
         * Formats an error message incorporating the invalid name and an explanatory message.
         * An optional source location parameter can be provided to indicate where the exception was thrown.
         *
         * @param name The invalid name that triggered the exception.
         * @param message A descriptive message explaining why the name is invalid.
         * @param loc The source location where the exception was thrown (defaults to the current location).
         */
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
            /**
 * @brief Default virtual destructor for the ValidatedName class.
 *
 * Ensures proper cleanup of derived classes when deleting objects through a pointer to ValidatedName.
 */
virtual ~ValidatedName() = default;

            /**
             * @brief Constructs a ValidatedName object from a given name.
             *
             * Validates the input name using TValidator::validate. If the name is invalid,
             * the stored value is reset to "Unnamed" and an InvalidName exception is thrown
             * with the corresponding error message.
             *
             * @param name The name to be validated.
             *
             * @throws InvalidName if the name fails validation.
             */
            explicit ValidatedName(const std::string_view name) : _value(name) {
                if (auto errorMessage = TValidator::validate(name); errorMessage.has_value()) {
                    _value = "Unnamed";
                    THROW_EXCEPTION(InvalidName, name, errorMessage.value());
                }
            }
            /**
 * @brief Constructs a ValidatedName from a std::string.
 *
 * This convenience constructor converts the provided std::string to a std::string_view and forwards it
 * to the primary constructor for validation using the associated Validator. If the name fails validation,
 * "Unnamed" is assigned and an InvalidName exception is thrown.
 *
 * @param name The string containing the name to be validated.
 *
 * @throws InvalidName if the name does not meet the validation criteria.
 */
explicit(false) ValidatedName(const std::string& name) : ValidatedName(std::string_view(name)) {}
            /**
 * @brief Constructs a ValidatedName from a C-style string.
 *
 * Converts the provided null-terminated string to a std::string_view and validates it using the configured validator.
 * If the name fails validation, the name is set to "Unnamed" and an InvalidName exception is thrown.
 *
 * @param name A pointer to a null-terminated C-string representing the name to validate.
 *
 * @throws nexo::assets::InvalidName if the name does not meet validation criteria.
 */
explicit(false) ValidatedName(const char* name) : ValidatedName(std::string_view(name)) {}




            /**
 * @brief Retrieves the length of the validated name.
 *
 * This method returns the number of characters in the name stored by the object.
 *
 * @return std::size_t The length of the validated name.
 */
            [[nodiscard]] std::size_t size() const { return _value.size(); }

            /**
 * @brief Converts the validated name to a std::string.
 *
 * This operator returns the underlying validated name stored in the instance,
 * allowing the object to be used as a std::string in contexts that require a string representation.
 */

            explicit operator std::string() const { return _value; }
            /**
 * @brief Explicit conversion operator to std::string_view.
 *
 * Converts the ValidatedName instance into a std::string_view, providing a read-only view
 * of the internally stored validated name.
 *
 * @return std::string_view A view of the validated name.
 */
explicit operator std::string_view() const { return _value; }
            /**
 * @brief Explicitly converts the validated name to a C-style string.
 *
 * Returns a pointer to the null-terminated character array of the underlying name.
 *
 * @return const char* Pointer to the C-string representation of the validated name.
 */
explicit operator const char*() const { return _value.c_str(); }

            /**
             * @brief Returns the underlying name as a string.
             */
            [[nodiscard]] const std::string& data() const { return _value; }
            /**
 * @brief Returns the validated name as a null-terminated C-style string.
 *
 * This function provides a pointer to the internal string's data, enabling compatibility
 * with APIs that require a pointer to a null-terminated character array.
 *
 * @return const char* Pointer to the validated name.
 */
[[nodiscard]] const char* c_str() const { return _value.c_str(); }


            /**
 * @brief Compares two ValidatedName instances for equality.
 *
 * Compares the underlying name stored in each object to determine if they are identical.
 *
 * @param other Another ValidatedName instance to compare against.
 * @return true if both instances have the same name; false otherwise.
 */
            bool operator==(const ValidatedName& other) const { return _value == other._value; }
            /**
 * @brief Compares two ValidatedName instances for inequality.
 *
 * This operator returns the negated result of the equality operator.
 *
 * @param other The ValidatedName instance to compare against.
 * @return true if the current instance is not equal to other; false otherwise.
 */
bool operator!=(const ValidatedName& other) const { return !(*this == other); }

            /**
 * @brief Default copy assignment operator.
 *
 * Copies the validated name from the given instance.
 *
 * @param other The instance to copy from.
 * @return ValidatedName& A reference to the updated instance.
 */
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
    /**
     * @brief Assigns a new validated name to the object.
     *
     * Validates the provided name using the TValidator's static validate method. If validation fails,
     * it throws an InvalidName exception containing the error message. Upon successful validation,
     * the internal name value is updated and a reference to the current object is returned.
     *
     * @param name The new name to assign, as a std::string_view.
     * @return ValidatedName& Reference to the updated object.
     *
     * @throws InvalidName if the name fails validation.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            THROW_EXCEPTION(InvalidName, name, errorMessage.value());
        _value = name;
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Assigns a new validated name from a std::string.
     *
     * Converts the given std::string to a std::string_view and delegates
     * validation and assignment to the corresponding operator. If the new name
     * is invalid, an InvalidName exception is thrown.
     *
     * @param name The new name to assign.
     * @return Reference to the updated ValidatedName object.
     *
     * @throws InvalidName if the provided name does not pass validation.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const std::string& name)
    {
        *this = std::string_view(name);
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Assigns a new validated name from a null-terminated string.
     *
     * Converts the provided C-string into a std::string_view and delegates the assignment
     * to the corresponding operator=. If the new name fails validation, an InvalidName
     * exception is thrown.
     *
     * @param name The C-string representing the new name.
     * @return Reference to the updated ValidatedName instance.
     * @throws InvalidName If the provided name is invalid.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const char* name)
    {
        this->operator=(std::string_view(name));
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Attempts to rename the asset by validating the new name.
     *
     * Validates the provided name using the associated validator. If validation fails,
     * returns an error message describing the failure and leaves the current name unchanged.
     * On successful validation, updates the internal name and returns std::nullopt.
     *
     * @param name The new name to be applied.
     * @return std::optional<std::string> An error message if the new name is invalid; otherwise, std::nullopt.
     */
    std::optional<std::string> ValidatedName<TValidator>::rename(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            return errorMessage;
        _value = name;
        return std::nullopt;
    }
} // namespace nexo::assets
