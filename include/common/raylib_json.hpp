//// raylib_json.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        31/10/2024
//  Description: nohlmann::json serialization for raylib types.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <nlohmann/json.hpp>

#include "my_raylib.h"

// Vector3
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z)

// Color
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Color, r, g, b, a)

// Matrix
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Matrix, m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15)

// BoundingBox
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BoundingBox, min, max)

// Ray
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Ray, position, direction)

// Rectangle
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Rectangle, x, y, width, height)

// TODO: this serialization is incomplete, just temporary for tests
// Sound
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Sound, frameCount)

// TODO: this serialization is incomplete, just temporary for tests
// Music
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Music, ctxType)
