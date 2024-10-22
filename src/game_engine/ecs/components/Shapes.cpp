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

        Sphere::Sphere(
            float radius,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _radius(radius),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            _model = LoadModelFromMesh(GenMeshSphere(_radius, 32, 32));
            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Sphere::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Plane::Plane(
            float width,
            float length,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _width(width),
            _length(length),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            _model = LoadModelFromMesh(GenMeshPlane(_width, _length, 10, 10));
            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Plane::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Cylinder::Cylinder(
            float radius,
            float height,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _radius(radius),
            _height(height), _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            _model = LoadModelFromMesh(GenMeshCylinder(radius, _height, 32));
            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Cylinder::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Mesh GenMeshPoly3D(int sides, float radius, float height)
        {
            Mesh mesh = {0};

            mesh.vertexCount = sides * 6;
            mesh.triangleCount = sides * 4 + sides * 2;

            mesh.vertices = (float *)RL_MALLOC(mesh.vertexCount * 3 * sizeof(float));
            mesh.indices = (unsigned short *)RL_MALLOC(mesh.triangleCount * 3 * sizeof(unsigned short));

            for (int i = 0; i < sides; i++) {
                float angle = (float)i / sides * PI * 2;
                float x = cos(angle) * radius;
                float z = sin(angle) * radius;

                mesh.vertices[i * 6 + 0] = x;
                mesh.vertices[i * 6 + 1] = 0;
                mesh.vertices[i * 6 + 2] = z;

                // Base supérieure
                mesh.vertices[i * 6 + 3] = x;
                mesh.vertices[i * 6 + 4] = height;
                mesh.vertices[i * 6 + 5] = z;
            }

            int index = 0;
            for (int i = 0; i < sides; i++) {
                int nextIndex = (i + 1) % sides;

                // Face latérale, deux triangles
                mesh.indices[index++] = i * 2;
                mesh.indices[index++] = i * 2 + 1;
                mesh.indices[index++] = nextIndex * 2;

                mesh.indices[index++] = nextIndex * 2;
                mesh.indices[index++] = i * 2 + 1;
                mesh.indices[index++] = nextIndex * 2 + 1;
            }

            for (int i = 1; i < sides - 1; i++) {
                mesh.indices[index++] = 0;
                mesh.indices[index++] = i * 2;
                mesh.indices[index++] = (i + 1) * 2;
            }

            for (int i = 1; i < sides - 1; i++) {
                mesh.indices[index++] = 1;
                mesh.indices[index++] = (i + 1) * 2 + 1;
                mesh.indices[index++] = i * 2 + 1;
            }

            UploadMesh(&mesh, false);
            return mesh;
        }

        Polygon::Polygon(
            int sides,
            float radius,
            float height,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _sides(sides),
            _radius(radius),
            _height(height),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            Mesh mesh = {0};
            mesh = GenMeshPoly3D(_sides, _radius, _height);
            _model = LoadModelFromMesh(mesh);

            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Polygon::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Hemisphere::Hemisphere(
            float radius,
            int rings,
            int slices,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _radius(radius),
            _rings(rings),
            _slices(slices),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            Mesh mesh = GenMeshHemiSphere(_radius, _rings, _slices);
            _model = LoadModelFromMesh(mesh);

            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Hemisphere::draw(physics::transform_t &transf) const {
            //rlDisableBackfaceCulling();
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Cone::Cone(float radius, float height, int slices, bool toggleWire, Color color, Color wireColor)
            : _radius(radius), _height(height), _slices(slices), _toggleWire(toggleWire), _color(color), _wireColor(wireColor)
        {
            // Utilise la fonction native Raylib pour générer un cône
            Mesh mesh = GenMeshCone(_radius, _height, _slices);
            _model = LoadModelFromMesh(mesh);

            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Cone::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Torus::Torus(
            float radius,
            float size,
            int radSeg,
            int sides,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _radius(radius),
            _size(size),
            _radSeg(radSeg),
            _sides(sides),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            Mesh mesh = GenMeshTorus(_radius, _size, _radSeg, _sides);
            _model = LoadModelFromMesh(mesh);

            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Torus::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
                DrawModelWires(_model, transf.pos, 1, _wireColor);
        }

        Knot::Knot(
            float radius,
            float size,
            int radSeg,
            int sides,
            bool toggleWire,
            Color color,
            Color wireColor) :
            _radius(radius),
            _size(size),
            _radSeg(radSeg),
            _sides(sides),
            _toggleWire(toggleWire),
            _color(color),
            _wireColor(wireColor)
        {
            Mesh mesh = GenMeshKnot(_radius, _size, _radSeg, _sides);
            _model = LoadModelFromMesh(mesh);

            _model.materials[0] = LoadMaterialDefault();
            _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = _color;
        }

        void Knot::draw(physics::transform_t &transf) const {
            DrawModel(_model, transf.pos, 1, _color);
            if (_toggleWire)
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