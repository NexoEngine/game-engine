//// TransparentStringHash.hpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Header file containing the transparent string hash used for maps
//
///////////////////////////////////////////////////////////////////////////////

#include <string>

namespace nexo::editor {
	// Custom transparent heterogeneous hasher for strings.
	struct TransparentStringHash {
	    // This alias tells the container that our hasher supports heterogeneous lookup.
	    using is_transparent = void;

	    /**
	     * @brief Computes the hash value of a std::string.
	     *
	     * This function returns the hash value computed by the standard library's hash function for std::string,
	     * enabling its use in hash-based containers. It guarantees no exceptions are thrown.
	     *
	     * @param s The string to hash.
	     * @return The computed hash value.
	     */
	    size_t operator()(const std::string &s) const noexcept {
	        return std::hash<std::string>{}(s);
	    }

	    /**
	     * @brief Computes the hash value from a std::string_view.
	     *
	     * This function overload uses the standard library's std::hash for std::string_view to compute
	     * and return the hash value for the provided string view.
	     *
	     * @param s The std::string_view to hash.
	     * @return size_t The computed hash value.
	     */
	    size_t operator()(std::string_view s) const noexcept {
	        return std::hash<std::string_view>{}(s);
	    }

	    /**
	     * @brief Computes the hash of a null-terminated C-string.
	     *
	     * This function constructs a std::string_view from the given C-string and computes its hash
	     * using the standard hash function for string views.
	     *
	     * @param s A pointer to a null-terminated character array.
	     * @return size_t The computed hash value.
	     */
	    size_t operator()(const char* s) const noexcept {
	        return std::hash<std::string_view>{}(s);
	    }
	};
}
