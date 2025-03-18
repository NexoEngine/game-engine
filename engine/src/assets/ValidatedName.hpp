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
         * This constructor creates an exception indicating that the provided name failed validation.
         * It formats an error message by embedding the invalid name and a custom message, then passes
         * it along with the source location to the base Exception class.
         *
         * @param name The name that failed validation.
         * @param message A message describing why the name is considered invalid.
         * @param loc The source location where the exception is thrown (defaults to the current location).
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
 * Ensures proper cleanup of the object and supports polymorphic deletion in derived classes.
 */
virtual ~ValidatedName() = default;

            /**
             * @brief Constructs a ValidatedName object by validating the provided name.
             *
             * Initializes the object with the given name and validates it using TValidator::validate.
             * If validation fails, the name is set to "Unnamed" and an InvalidName exception is thrown
             * with the corresponding error message.
             *
             * @param name The string view representing the name to be validated.
             *
             * @throws InvalidName if the name does not pass validation.
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
 * Converts the provided std::string to a std::string_view and delegates to the corresponding constructor,
 * performing name validation using the associated validator. If the name is invalid, the ValidatedName is set
 * to "Unnamed" and an InvalidName exception is thrown.
 *
 * @param name The name to validate and store.
 * @throws InvalidName if the provided name is invalid.
 */
explicit(false) ValidatedName(const std::string& name) : ValidatedName(std::string_view(name)) {}
            /**
 * @brief Constructs a ValidatedName using a C-style string.
 *
 * This constructor converts the provided null-terminated string into a std::string_view
 * and delegates validation to the corresponding string_view constructor. If the name fails
 * validation, the instance is set to "Unnamed" and an InvalidName exception is thrown.
 *
 * @param name A null-terminated C-string representing the name.
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
 * @brief Converts the instance to a std::string.
 *
 * Returns the underlying validated name as a standard string, enabling convenient use in contexts where a std::string is required.
 */

            explicit operator std::string() const { return _value; }
            /**
 * @brief Explicitly converts the ValidatedName instance to a std::string_view.
 *
 * Returns a non-owning view of the underlying validated name.
 *
 * @return std::string_view A view of the stored name.
 */
explicit operator std::string_view() const { return _value; }
            /**
 * @brief Converts the validated name to a C-style string.
 *
 * This explicit conversion operator returns a pointer to the internal null-terminated
 * character array that represents the validated name.
 *
 * @return const char* Pointer to the underlying C-style string.
 */
explicit operator const char*() const { return _value.c_str(); }

            /**
 * @brief Retrieves the stored validated name.
 *
 * This method returns a constant reference to the underlying string that holds the validated name.
 *
 * @return const std::string& A reference to the validated name.
 */
[[nodiscard]] const std::string& data() const { return _value; }
            /**
 * @brief Retrieves a C-style string representation of the validated name.
 *
 * Returns a pointer to a null-terminated character array representing the validated name.
 * The returned pointer remains valid as long as the underlying string is not modified.
 *
 * @return const char* Pointer to the null-terminated character array.
 */
[[nodiscard]] const char* c_str() const { return _value.c_str(); }


            /**
 * @brief Checks if two ValidatedName instances have the same validated name.
 *
 * Compares the underlying string values of the two ValidatedName objects.
 *
 * @param other The ValidatedName instance to compare with.
 * @return true if both instances represent the same name, false otherwise.
 */
            bool operator==(const ValidatedName& other) const { return _value == other._value; }
            /**
 * @brief Determines if two ValidatedName objects are not equal.
 *
 * Compares this instance with another ValidatedName and returns true if they represent different names.
 *
 * @param other The ValidatedName instance to compare against.
 * @return true if the objects are not equal, false otherwise.
 */
bool operator!=(const ValidatedName& other) const { return !(*this == other); }

            /**
 * @brief Default copy assignment operator.
 *
 * Uses the compiler-generated default implementation to copy the value from another ValidatedName.
 *
 * @param other The ValidatedName instance to copy from.
 * @return Reference to this instance.
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
     * Validates the provided string view using the associated validator. If the name fails validation,
     * an InvalidName exception is thrown. On successful validation, the internal name is updated.
     *
     * @param name The new name to assign.
     * @return ValidatedName<TValidator>& A reference to the updated object.
     * @throws InvalidName If the provided name does not pass validation.
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
     * Converts the provided string into a string view and delegates the assignment,
     * ensuring the name is validated according to the TValidator rules. If the new name 
     * fails validation, an InvalidName exception is thrown.
     *
     * @param name New name to assign.
     * @return Reference to the updated ValidatedName instance.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const std::string& name)
    {
        *this = std::string_view(name);
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Assigns a new validated name from a C-string.
     *
     * Converts the provided C-string to a std::string_view and delegates the assignment to the
     * corresponding overload. If the new name is invalid, the underlying assignment operator will
     * handle the error, potentially throwing an InvalidName exception.
     *
     * @param name The new name as a C-string.
     * @return A reference to the current instance with the updated name.
     */
    ValidatedName<TValidator>& ValidatedName<TValidator>::operator=(const char* name)
    {
        this->operator=(std::string_view(name));
        return *this;
    }

    template<Validator TValidator>
    /**
     * @brief Attempts to rename the validated name.
     *
     * Validates the provided name. If the new name is invalid, the function returns an error message
     * without updating the current name. When the name is valid, it updates the internal value and returns std::nullopt.
     *
     * @param name New name to assign.
     * @return std::optional<std::string> An error message if validation fails, or std::nullopt on success.
     */
    std::optional<std::string> ValidatedName<TValidator>::rename(std::string_view name)
    {
        if (auto errorMessage = validate(name); errorMessage.has_value())
            return errorMessage;
        _value = name;
        return std::nullopt;
    }
} // namespace nexo::assets
