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
         * @brief Constructs an InvalidName exception with a formatted error message.
         *
         * Creates an exception object using a formatted string that includes the invalid name and a descriptive error message.
         * An optional source location can be provided for more precise debugging, defaulting to the current location.
         *
         * @param name The name that failed validation.
         * @param message A descriptive message explaining why the name is considered invalid.
         * @param loc The source location where the exception is created (defaults to the current location).
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
 * @brief Virtual destructor for ValidatedName.
 *
 * Enables safe polymorphic deletion of derived objects.
 */
virtual ~ValidatedName() = default;

            /**
             * @brief Constructs a ValidatedName and validates the provided name.
             *
             * This constructor initializes a ValidatedName object with the given name by validating it
             * using TValidator::validate. If the validation fails, the internal name is set to "Unnamed"
             * and an InvalidName exception is thrown containing the original name and a descriptive error message.
             *
             * @param name The initial name to be validated.
             */
            explicit ValidatedName(const std::string_view name) : _value(name) {
                if (auto errorMessage = TValidator::validate(name); errorMessage.has_value()) {
                    _value = "Unnamed";
                    THROW_EXCEPTION(InvalidName, name, errorMessage.value());
                }
            }
            /**
 * @brief Constructs a ValidatedName from an std::string.
 *
 * Converts the given string into a std::string_view and delegates initialization to the primary constructor.
 * The name is validated using TValidator's static validate method. If the provided name is invalid, the instance
 * is initialized with "Unnamed" and an InvalidName exception is thrown.
 *
 * @param name The name string to validate.
 */
explicit(false) ValidatedName(const std::string& name) : ValidatedName(std::string_view(name)) {}
            /**
 * @brief Constructs a ValidatedName object from a C-style string.
 *
 * Converts the given null-terminated string to a std::string_view and delegates to the corresponding
 * constructor for validation. If the string fails validation, the name is set to "Unnamed" and an
 * InvalidName exception is thrown.
 *
 * @param name A C-style string representing the name to be validated.
 */
explicit(false) ValidatedName(const char* name) : ValidatedName(std::string_view(name)) {}




            /**
             * @brief Returns the underlying name as a string.
             */
            [[nodiscard]] const std::string& getName() const { return _value; }

            /**
             * @brief Returns the size of the name.
             */
            [[nodiscard]] std::size_t size() const { return _value.size(); }

            /**
 * @brief Converts the validated name to a standard string.
 *
 * This explicit conversion operator returns the underlying string value of the instance,
 * allowing for a convenient conversion to a std::string.
 */

            explicit operator std::string() const { return _value; }
            /**
 * @brief Converts the validated name to a std::string_view.
 *
 * Provides an explicit conversion operator that returns a non-owning view of the underlying
 * validated name string.
 *
 * @return A std::string_view representing the validated name.
 */
explicit operator std::string_view() const { return _value; }
            /**
 * @brief Explicit conversion operator to obtain a C-style string representation.
 *
 * Converts the validated name to a null-terminated character array.
 *
 * @return const char* Pointer to the C-style string representing the validated name.
 */
explicit operator const char*() const { return _value.c_str(); }

            /**
 * @brief Returns a constant reference to the validated name.
 *
 * This method provides direct access to the internal string holding the validated name.
 *
 * @return const std::string& A reference to the validated name string.
 */
[[nodiscard]] const std::string& data() const { return _value; }
            /**
 * @brief Retrieves the validated name as a C-style string.
 *
 * This function returns a pointer to the null-terminated string representing the validated name.
 *
 * @return const char* Pointer to the underlying C-style string.
 */
[[nodiscard]] const char* c_str() const { return _value.c_str(); }


            /**
 * @brief Compares two ValidatedName objects for equality.
 *
 * This operator checks if the underlying name strings of both objects are identical.
 *
 * @param other The ValidatedName object to compare with.
 * @return true if the names are equal; otherwise, false.
 */
            bool operator==(const ValidatedName& other) const { return _value == other._value; }
            /**
 * @brief Compares two ValidatedName objects for inequality.
 *
 * Returns true if the underlying names of the two ValidatedName instances differ.
 *
 * @param other The ValidatedName object to compare against.
 * @return true if the instances are not equal, false otherwise.
 */
bool operator!=(const ValidatedName& other) const { return !(*this == other); }

            /**
 * @brief Default copy assignment operator.
 *
 * Copies the validated name from another instance.
 *
 * @param other The instance to copy from.
 * @return ValidatedName& Reference to this instance.
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
     * @brief Assigns a new validated name.
     *
     * Validates the provided name using the TValidator's static validation method. If the name is invalid,
     * an InvalidName exception is thrown with a descriptive error message. On successful validation, the
     * internal value is updated to the new name and a reference to the current instance is returned.
     *
     * @param name The new name to assign.
     * @return ValidatedName<TValidator>& Reference to the object with the updated name.
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
     * @brief Assigns a new name after validating it.
     *
     * Converts the given std::string to a std::string_view and delegates to the corresponding
     * assignment operator. If the new name is invalid, an InvalidName exception is thrown.
     *
     * @param name The new name to assign.
     * @return ValidatedName& A reference to this validated name object.
     *
     * @throws nexo::assets::InvalidName if the provided name does not pass validation.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const std::string& name)
    {
        *this = std::string_view(name);
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Assigns a new name from a C-style string.
     *
     * Converts the provided C-string to a std::string_view and delegates the assignment
     * to the corresponding operator= overload. The new name is validated using TValidator,
     * throwing an InvalidName exception if validation fails.
     *
     * @param name The C-string representing the new name.
     * @return A reference to the updated ValidatedName instance.
     *
     * @throws nexo::assets::InvalidName if the provided name is invalid.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const char* name)
    {
        this->operator=(std::string_view(name));
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Attempts to update the validated name.
     *
     * Validates the provided name using the associated validator. If the name is invalid, returns an 
     * error message describing the issue; otherwise, updates the name and returns std::nullopt.
     *
     * @param name The new name to assign.
     * @return std::optional<std::string> An error message if validation fails, or std::nullopt if the new name is valid.
     */
    std::optional<std::string> ValidatedName<TValidator>::rename(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            return errorMessage;
        _value = name;
        return std::nullopt;
    }
} // namespace nexo::assets
