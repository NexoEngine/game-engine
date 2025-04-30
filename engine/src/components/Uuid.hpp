//// Uuid.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the uuid metadata component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <random>

namespace nexo::components {
	//TODO: Implement a safer uuid generator
	inline std::string genUuid()
	{
		static std::random_device dev;
		static std::mt19937 rng(dev());

		std::uniform_int_distribution dist(0, 15);

		const char *v = "0123456789abcdef";
		constexpr bool dash[] = { false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false };

		std::string res;
		for (const bool i : dash) {
			if (i) res += "-";
			res += v[dist(rng)];
			res += v[dist(rng)];
		}
		return res;
	}

    struct UuidComponent {
        struct Memento {
            std::string uuid;
        };

        void restore(const Memento &memento)
        {
            uuid = memento.uuid;
        }

        Memento save() const
        {
            return {uuid};
        }

        std::string uuid = genUuid();
    };
}
