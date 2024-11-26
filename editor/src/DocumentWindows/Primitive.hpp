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
#include "EntityFactory3D.hpp"

namespace nexo::editor
{
    using Primitive2DFunction = ecs::Entity(*)(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color);
    inline const char* primitives2DNames[] = {
        "  Cube", "  Plan", "  Sphere",
        "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",
        "  Hemisphere"
    };
    inline Primitive2DFunction addPrimitive2D[] = {
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

    using Primitive3DFunction = ecs::Entity(*)(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color);
    inline const char* primitives3DNames[] = {
        "  Cube", "  Plan", "  Sphere",
        "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",
        "  Hemisphere"
    };
    inline Primitive3DFunction addPrimitive3D[] = {
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube,
        &EntityFactory3D::createCube
    };
}
#endif //PRIMITIVE_HPPd
