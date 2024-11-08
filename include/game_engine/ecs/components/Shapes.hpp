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
