/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Objects
*/

#pragma once

#include "raymath.h"
#include "my_raylib.h"

namespace common::game {
    struct ObjectParams {
        Vector3 pos;
        float width;
        float height;
        float length;
        Color color;
        bool toggleWire;
        Color wireColor;
        Vector3 rotation;
        Vector3 scale;
    };

    enum class ObjectName : uint32_t {
        DualStriker, // ALLY
        MicroRecon, // --
        RedFighter, // --
        UltravioletIntruder, // --
        GunBullet, // BULLET
        Transtellar, // ENEMY
        DefaultSkybox,
        None,
    };

    enum ObjectType {
        Cube,
        Model3D,
        SkyBox,
    };

    enum ObjectFormat {
        GLB,
        OBJ,
        PNG,
        F_NONE
    };
}
