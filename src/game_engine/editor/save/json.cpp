//// json.cpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        12/11/2024
//  Description: JSON utils for save system.
//
///////////////////////////////////////////////////////////////////////////////


#include "game_engine/save/json.hpp"

namespace engine::save {

	json createJsonWithMagic(const char* magic, const json& data) {
		return json::array({
			magic,
			data
		});
	}

	json parseJsonWithMagic(const char* magic, const json& data) {
		auto foundMagic = data.at(0).get<std::string>();
		if (foundMagic != magic) {
			throw JsonMagicMismatch(std::format("Magic mismatch, found '{}' expected '{}'", foundMagic, magic));
		}
		return data.at(1);
	}
} // namespace engine::editor