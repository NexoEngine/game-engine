//// Attributes.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the attributes definitions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>

namespace nexo::renderer {

   	struct RequiredAttributes {
		union BitFields {
			struct Flags {
				bool position : 1;
				bool normal : 1;
				bool tangent : 1;
				bool bitangent : 1;
				bool uv0 : 1;
				bool lightmapUV : 1;
				bool padding : 2;
			} flags;
			uint8_t bits = 0;
		} bitsUnion;

		bool operator==(RequiredAttributes const& o) const {
            return bitsUnion.bits == o.bitsUnion.bits;
        }

        [[nodiscard]] bool compatibleWith(RequiredAttributes const& o) const {
            return (bitsUnion.bits & o.bitsUnion.bits) == bitsUnion.bits;
        }
	};

}
