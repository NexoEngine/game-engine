/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#include "common/game/entities/EntityFactory.hpp"
#include <string>
#include <EntityHandling.hpp>

ecs::Entity common::game::EntityFactory::createEntity(
    common::game::ObjectType type,
    common::game::ObjectName name,
    common::game::ObjectParams params,
    common::game::ObjectFormat format,
    std::string path
)
{
    switch (type) {
        case common::game::ObjectType::Cube:
            return createCube(name, params, format);
        case common::game::ObjectType::Model3D:
            return createObject3D(name, params, format, path);
        case common::game::ObjectType::SkyBox:
            return createSkybox(name, params, format, path);
        default:
            throw std::invalid_argument("Invalid entity type");
    }
}
ecs::Entity common::game::EntityFactory::createCube(
    ObjectName name,
    ObjectParams params,
    ObjectFormat format
)
{
    auto shape = engine::createCube(
        params.pos,
        params.width,
        params.height,
        params.length,
        params.color,
        params.toggleWire,
        params.wireColor
    );
    engine::rotate(shape, params.rotation);
    engine::scale(shape, params.scale);

    return shape;
}

ecs::Entity common::game::EntityFactory::createObject3D(
    ObjectName name,
    ObjectParams params,
    ObjectFormat format,
    std::string path
)
{
    std::string objectName = this->objectNameToString(name);
    std::string objectFormat = this->objectFormatToString(format);
    std::string assetPath = path + "ressources/client/Objects3D/" + objectName + "/" + objectName + objectFormat;

    auto shape = engine::createModel3D(
        assetPath.c_str(),
        params.pos,
        params.color
    );
    engine::rotate(shape, params.rotation);
    engine::scale(shape, params.scale);

    return shape;
}

ecs::Entity common::game::EntityFactory::createSkybox(
    common::game::ObjectName name,
    common::game::ObjectParams params,
    common::game::ObjectFormat format,
    std::string path
)
{
    std::string objectName = this->objectNameToString(name);
    std::string objectFormat = this->objectFormatToString(format);
    std::string assetPath = path + "ressources/client/Skybox/" + objectName + "/" + objectName + objectFormat;

    auto shape = engine::createSkybox(
        assetPath.c_str(),
        params.pos,
        params.color
    );
    engine::rotate(shape, params.rotation);
    engine::scale(shape, params.scale);

    return shape;
}

std::string common::game::EntityFactory::objectNameToString(common::game::ObjectName name)
{
    switch (name) {
        case common::game::ObjectName::DualStriker:           return "DualStriker";
        case common::game::ObjectName::MicroRecon:            return "MicroRecon";
        case common::game::ObjectName::RedFighter:            return "RedFighter";
        case common::game::ObjectName::UltravioletIntruder:   return "UltravioletIntruder";
        case common::game::ObjectName::GunBullet:             return "GunBullet";
        case common::game::ObjectName::Transtellar:           return "Transtellar";
        case common::game::ObjectName::DefaultSkybox:         return "DefaultSkybox";
        default:                            throw std::invalid_argument("Invalid object name");
    }
}

std::string common::game::EntityFactory::objectFormatToString(common::game::ObjectFormat format)
{
    switch (format) {
        case OBJ: return ".obj";
        case GLB: return ".glb";
        case PNG: return ".png";
        default:
            throw std::invalid_argument("Invalid format format");
    }
}
