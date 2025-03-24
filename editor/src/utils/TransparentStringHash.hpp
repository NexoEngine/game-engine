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

	    // Hash a std::string.
	    size_t operator()(const std::string &s) const noexcept {
	        return std::hash<std::string>{}(s);
	    }

	    // Hash a std::string_view.
	    size_t operator()(std::string_view s) const noexcept {
	        return std::hash<std::string_view>{}(s);
	    }

	    // Hash a C-string.
	    size_t operator()(const char* s) const noexcept {
	        return std::hash<std::string_view>{}(s);
	    }
	};
}
