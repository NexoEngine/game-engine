/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Shapes.cpp
*/

#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "rlgl.h"
#include "raymath.h"

namespace ecs {
    namespace components {
        Model &IShape::getModel()
        {
            return _model;
        }

        BoundingBox IShape::getBoundingBox(physics::collider_t &collider) const
        {
            return collider.box;
            BoundingBox aabb = collider.box;
            // BoundingBox aabb = GetMeshBoundingBox(_model.meshes[0]);
            Vector3 corners[8] = {
                aabb.min,
                {aabb.max.x, aabb.min.y, aabb.min.z},
                {aabb.min.x, aabb.max.y, aabb.min.z},
                {aabb.min.x, aabb.min.y, aabb.max.z},
                {aabb.min.x, aabb.max.y, aabb.max.z},
                {aabb.max.x, aabb.min.y, aabb.max.z},
                {aabb.max.x, aabb.max.y, aabb.min.z},
                aabb.max
            };
            Matrix transformMatrix = MatrixMultiply(MatrixMultiply(collider.matScale, collider.matRotate), collider.matTranslate);
            for (int i = 0; i < 8; i++) {
                corners[i] = Vector3Transform(corners[i], transformMatrix);
            }

            Vector3 min = corners[0];
            Vector3 max = corners[0];
            for (int i = 1; i < 8; i++) {
                min = Vector3Min(min, corners[i]);
                max = Vector3Max(max, corners[i]);
            }

            BoundingBox obb = {min, max};
            return obb;
        }

        Cube::Cube(
            float width,
            float height,
            float length,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _width(width),
            _height(height),
            _length(length),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            _model = LoadModelFromMesh(GenMeshCube(_width, _height, _length));
            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Cube::draw(physics::transform_t &transf) const
        {
            DrawModel(_model, transf.pos, 1, _color);
            if (false)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Model3D::Model3D(const char *filename, Color color)
        {
            _model = LoadModel(filename);
            _color = color;
        }

        void Model3D::draw(physics::transform_t &transf) const
        {
            DrawModel(_model, transf.pos, 1, _color);
        }

        Skybox::Skybox(const char *filename)
        {
            Texture2D texture = LoadTexture(filename);
            Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
            _model = LoadModelFromMesh(cube);
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        }

        void Skybox::draw(physics::transform_t &transf) const
        {
            rlDisableBackfaceCulling();
            rlDisableDepthMask();
            DrawModel(_model, transf.pos, 50.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
        }
    }
}