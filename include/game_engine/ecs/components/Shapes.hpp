/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Shapes
*/

#pragma once

#include "my_raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

namespace ecs {
    namespace components {
        namespace physics {
            typedef struct transform_s transform_t;
            typedef struct collider_s collider_t;
        }
    }
}

namespace ecs {
    namespace components {
        /**
         * @enum ShapeType
         * @brief Enumerates different types of shapes available for rendering.
         */
        enum class ShapeType {
            POLYGON,
            HEMISPHERE,
            CONE,
            TORUS,
            KNOT,
            CUBE,
            LINE,
            POINT,
            CIRCLE,
            TRIANGLE,
            TRIANGLE_STRIP,
            SPHERE,
            CYLINDER,
            CAPSULE,
            PLANE,
            GRID,
            MODEL,
        };

        /**
         * @class IShape
         * @brief Interface for shape components used in rendering entities.
         *
         * Provides a common interface for different types of shapes, allowing them to be
         * used interchangeably in the rendering system. Each shape should implement its own
         * drawing logic.
         */
        class IShape {
            public:
                ~IShape() = default;
                /**
                 * @brief Pure virtual function for drawing the shape.
                 * @param transf Reference to the transformation component.
                 */
                virtual void draw(physics::transform_t &transf) const = 0;
                BoundingBox getBoundingBox(void) const;
                void drawBoundingBox(void) const;
                /**
                 * @brief Gets the model associated with the shape.
                 * @return Reference to the Model object.
                 */
                Model &getModel();
            protected:

                void initBoundingBox(void);

                Model _model;
                Vector3 boundingBoxCorners[8];
        };

        /**
         * @class Cube
         * @brief Represents a cube shape with customizable dimensions and colors.
         *
         * Inherits from IShape and implements the drawing logic for a cube. It allows for
         * the specification of dimensions, color, and an option to toggle wireframe rendering.
         */
        class Cube : public IShape {
            public:
                /**
                 * @brief Constructs a Cube object with specified dimensions and colors.
                 * @param width Width of the cube.
                 * @param height Height of the cube.
                 * @param length Length of the cube.
                 * @param toggleWire Flag to toggle wireframe rendering.
                 * @param color Color of the cube.
                 * @param wireColor Color of the wireframe.
                 */
                Cube(
                    float width = 2,
                    float height = 2,
                    float length = 2,
                    bool toggleWire = false,
                    Color color = RED,
                    Color wireColor = BLACK);
                /**
                 * @brief Draws the cube with the specified transformations.
                 * @param transf Reference to the transformation component.
                 */
                void draw(physics::transform_t &transf) const override;
            private:
                float _width;
                float _height;
                float _length;
                bool _toggleWire;
                Color _color;
                Color _wireColor;
        };

        /**
         * @class Sphere
         * @brief Represents a sphere shape with customizable radius and colors.
         *
         * Inherits from IShape and implements the drawing logic for a sphere. It allows for
         * the specification of the radius, color, and an option to toggle wireframe rendering.
         */
        class Sphere : public IShape {
        public:
            /**
             * @brief Constructs a Sphere object with specified radius and colors.
             * @param radius Radius of the sphere.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the sphere.
             * @param wireColor Color of the wireframe.
             */
            Sphere(float radius = 1.0f,
                bool toggleWire = false,
                Color color = RED,
                Color wireColor = BLACK);

            /**
             * @brief Draws the sphere with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Plane
         * @brief Represents a sphere shape with customizable radius and colors.
         *
         * Inherits from IShape and implements the drawing logic for a plane. It allows for
         * the specification of the width, length, color and an option to toggle wireframe rendering.
         */
        class Plane : public IShape {
        public:
            /**
             * @brief Constructs a Plane object with specified width, length, and colors.
             * @param width Width of the plane.
             * @param length Length of the plane.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the plane.
             * @param wireColor Color of the wireframe.
             */
            Plane(float width = 10.0f, float length = 10.0f, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the plane with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _width;
            float _length;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Cylinder
         * @brief Represents a sphere shape with customizable radius and colors.
         *
         * Inherits from IShape and implements the drawing logic for a cylinder. It allows for
         * the specification of the radiusTop, radiusBottom, height, color and an option to toggle wireframe rendering.
         */
        class Cylinder : public IShape {
        public:
            /**
             * @brief Constructs a Cylinder object with specified dimensions and colors.
             * @param radius Radius of the top of the cylinder.
             * @param height Height of the cylinder.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the cylinder.
             * @param wireColor Color of the wireframe.
             */
            Cylinder(float radius = 1.0f, float height = 2.0f, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the cylinder with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            float _height;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Polygon
         * @brief Represents a polygon shape with customizable number of sides, radius, height, and colors.
         *
         * Inherits from IShape and implements the drawing logic for a polygon. It allows for
         * the specification of the number of sides, radius, height, color, and an option to toggle wireframe rendering.
         */
        class Polygon : public IShape {
        public:
            /**
             * @brief Constructs a Polygon object with specified parameters.
             * @param sides Number of sides of the polygon.
             * @param radius Radius of the polygon.
             * @param height Height of the extrusion (to make it 3D).
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the polygon.
             * @param wireColor Color of the wireframe.
             */
            Polygon(int sides = 6, float radius = 1.0f, float height = 1.0f, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the polygon with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            int _sides;
            float _radius;
            float _height;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Hemisphere
         * @brief Represents a hemisphere shape with customizable radius, rings, slices, and colors.
         *
         * Inherits from IShape and implements the drawing logic for a hemisphere. It allows for
         * the specification of the radius, number of rings and slices (for smoothness), color, and an option to toggle wireframe rendering.
         */
        class Hemisphere : public IShape {
        public:
            /**
             * @brief Constructs a Hemisphere object with specified radius and colors.
             * @param radius Radius of the hemisphere.
             * @param rings Number of rings for smoothness.
             * @param slices Number of slices for smoothness.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the hemisphere.
             * @param wireColor Color of the wireframe.
             */
            Hemisphere(float radius = 1.0f, int rings = 16, int slices = 16, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the hemisphere with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            int _rings;
            int _slices;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Cone
         * @brief Represents a cone shape with customizable radius, height, slices, and colors.
         *
         * Inherits from IShape and implements the drawing logic for a cone. It allows for
         * the specification of the radius, height, number of slices (for smoothness), color, and an option to toggle wireframe rendering.
         */
        class Cone : public IShape {
        public:
            /**
             * @brief Constructs a Cone object with specified radius, height, and colors.
             * @param radius Radius of the cone's base.
             * @param height Height of the cone.
             * @param slices Number of slices for smoothness.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the cone.
             * @param wireColor Color of the wireframe.
             */
            Cone(float radius = 1.0f, float height = 2.0f, int slices = 16, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the cone with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            float _height;
            int _slices;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Torus
         * @brief Represents a cone shape with customizable radius, height, slices, and colors.
         *
         * Inherits from IShape and implements the drawing logic for a cone. It allows for
         * the specification of the radius, size, radSeg, sides, color, and an option to toggle wireframe rendering.
         */
        class Torus : public IShape {
        public:
            /**
             * @brief Constructs a Torus object with specified radius, size, and colors.
             * @param radius Radius of the torus (distance from the center of the torus to the center of the tube).
             * @param size Size of the tube (radius of the tube itself).
             * @param radSeg Number of radial segments for smoothness.
             * @param sides Number of sides for smoothness of the tube.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the torus.
             * @param wireColor Color of the wireframe.
             */
            Torus(float radius = 1.0f, float size = 0.5f, int radSeg = 16, int sides = 16, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the torus with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            float _size;
            int _radSeg;
            int _sides;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Knot
         * @brief Represents a knot shape with customizable radius, size, segments, and colors.
         *
         * Inherits from IShape and implements the drawing logic for a knot. It allows for
         * the specification of the radius, size, number of radial segments, number of sides, color, and an option to toggle wireframe rendering.
         */
        class Knot : public IShape {
        public:
            /**
             * @brief Constructs a Knot object with specified radius, size, and colors.
             * @param radius Radius of the knot (distance from the center of the knot to the center of the tube).
             * @param size Size of the tube (radius of the tube itself).
             * @param radSeg Number of radial segments for smoothness.
             * @param sides Number of sides for smoothness of the tube.
             * @param toggleWire Flag to toggle wireframe rendering.
             * @param color Color of the knot.
             * @param wireColor Color of the wireframe.
             */
            Knot(float radius = 1.0f, float size = 0.5f, int radSeg = 16, int sides = 16, bool toggleWire = false, Color color = WHITE, Color wireColor = BLACK);

            /**
             * @brief Draws the knot with the specified transformations.
             * @param transf Reference to the transformation component.
             */
            void draw(physics::transform_t &transf) const override;

        private:
            float _radius;
            float _size;
            int _radSeg;
            int _sides;
            bool _toggleWire;
            Color _color;
            Color _wireColor;
        };

        /**
         * @class Model3D
         * @brief Represents a 3D model loaded from a file.
         *
         * Inherits from IShape and implements the drawing logic for a 3D model. It allows for
         * loading a model from a file and specifying its color.
         */
        class Model3D : public IShape {
            public:
                /**
                 * @brief Constructs a Model3D object from a file.
                 * @param filename Path to the file from which the model is loaded.
                 * @param color Color to apply to the model.
                 */
                Model3D(const char *filename, Color color = WHITE);
                /**
                 * @brief Draws the 3D model with the specified transformations.
                 * @param transf Reference to the transformation component.
                 */
                void draw(physics::transform_t &transf) const override;
            private:
                Color _color;
        };

        /**
         * @class Skybox
         * @brief Represents a skybox using a 3D model.
         *
         * Inherits from IShape and implements the drawing logic for a skybox. It allows for
         * loading a skybox from a file.
         */
        class Skybox : public IShape {
            public:
                /**
                 * @brief Constructs a Skybox object from a file.
                 * @param filename Path to the file from which the skybox is loaded.
                 */
                Skybox(const char *filename);
                /**
                 * @brief Draws the skybox with the specified transformations.
                 * @param transf Reference to the transformation component.
                 */
                void draw(physics::transform_t &transf) const override;
            private:
                Color _color;
        };
    }
}
