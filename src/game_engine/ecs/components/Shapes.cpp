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

        BoundingBox IShape::getBoundingBox() const
        {
            return BoundingBox{boundingBoxCorners[0], boundingBoxCorners[1]};
        }

        void IShape::drawBoundingBox(void) const
        {
            Matrix transform = _model.transform;
            Vector3 transformedCorners[8] = {};
            int index = 0;
            for (auto corner : boundingBoxCorners) {
                transformedCorners[index] = Vector3Transform(corner, transform);
                index++;
            }
            DrawLine3D(transformedCorners[0], transformedCorners[1], RED);
            DrawLine3D(transformedCorners[1], transformedCorners[2], RED);
            DrawLine3D(transformedCorners[2], transformedCorners[3], RED);
            DrawLine3D(transformedCorners[3], transformedCorners[0], RED);
            
            DrawLine3D(transformedCorners[4], transformedCorners[5], RED);
            DrawLine3D(transformedCorners[5], transformedCorners[6], RED);
            DrawLine3D(transformedCorners[6], transformedCorners[7], RED);
            DrawLine3D(transformedCorners[7], transformedCorners[4], RED);

            DrawLine3D(transformedCorners[0], transformedCorners[4], RED);
            DrawLine3D(transformedCorners[1], transformedCorners[5], RED);
            DrawLine3D(transformedCorners[2], transformedCorners[6], RED);
            DrawLine3D(transformedCorners[3], transformedCorners[7], RED);
        }

        void IShape::initBoundingBox(void)
        {
            BoundingBox AABB = GetModelBoundingBox(_model);
            boundingBoxCorners[0] = AABB.min;
            boundingBoxCorners[1] = AABB.max;
            boundingBoxCorners[2] = {AABB.min.x, AABB.min.y, AABB.max.z};
            boundingBoxCorners[3] = {AABB.min.x, AABB.max.y, AABB.min.z};
            boundingBoxCorners[4] = {AABB.max.x, AABB.min.y, AABB.min.z};
            boundingBoxCorners[5] = {AABB.max.x, AABB.max.y, AABB.min.z};
            boundingBoxCorners[6] = {AABB.max.x, AABB.min.y, AABB.max.z};
            boundingBoxCorners[7] = {AABB.min.x, AABB.max.y, AABB.max.z};
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
            initBoundingBox();
        }

        void Cube::draw(physics::transform_t &transf) const
        {
            DrawModel(_model, {0}, 1, _color);
            if (false)
                DrawModelWires(_model, {0}, 1, _wireColor);
        }

        Model3D::Model3D(const char *filename, Color color)
        {
            _model = LoadModel(filename);
            _color = color;
            initBoundingBox();
        }

        void Model3D::draw(physics::transform_t &transf) const
        {
            DrawModel(_model, {0}, 1, _color);
        }

        Skybox::Skybox(const char *filename)
        {
            Texture2D texture = LoadTexture(filename);
            Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
            _model = LoadModelFromMesh(cube);
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
            initBoundingBox();
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