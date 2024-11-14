//// json.hpp /////////////////////////////////////////////////////////////////
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

#pragma once

#include <nlohmann/json.hpp>
#include <stdexcept>

namespace engine::save {

	using json = nlohmann::json;

	template <typename T>
	concept JSONSerializable = requires(T obj, json& j) {
		{ to_json(j, obj) } -> std::same_as<void>;
		{ from_json(j, obj) } -> std::same_as<void>;
	};

	class JsonMagicMismatch : public std::runtime_error {
	public:
		JsonMagicMismatch(const std::string& msg) : runtime_error(msg) {}
	};


	json createJsonWithMagic(const char* magic, const json& data);
	json parseJsonWithMagic(const char* magic, const json& data);
} // namespace engine::editor
