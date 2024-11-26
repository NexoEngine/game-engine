//// Primitive.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        17/11/2024
//  Description: Primitive functions
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP
#include "EntityFactory2D.hpp"
namespace nexo::editor
{
    using PrimitiveFunction = ecs::Entity(*)(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color);
    inline const char *primitives_names[] = {
        "  Cube", "  Plan", "  Sphere",\
        "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",\
        "  Hemisphere"
    };
    inline PrimitiveFunction add_primitive_fct[] = {
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad,
        &EntityFactory2D::createQuad
    };
}
#endif //PRIMITIVE_HPP