//// Attributes.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025a
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

        bool compatibleWith(RequiredAttributes const& o) const {
            return (bitsUnion.bits & o.bitsUnion.bits) == bitsUnion.bits;
        }
	};

}
