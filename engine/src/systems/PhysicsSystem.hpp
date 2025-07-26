//// PhysicsSystem.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        02/03/2025
//  Description: Header file for the physics system
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <Coordinator.hpp>
#include <iostream>
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <Entity.hpp>
#include <GroupSystem.hpp>
#include <QuerySystem.hpp>
#include <components/PhysicsBodyComponent.hpp>
#include <components/Transform.hpp>
#include <vector>

namespace nexo::system
{
    namespace Layers
    {
        constexpr JPH::ObjectLayer NON_MOVING = 0;
        constexpr JPH::ObjectLayer MOVING = 1;
        constexpr JPH::ObjectLayer NUM_LAYERS = 2;
    }

    namespace BroadPhaseLayers
    {
        constexpr JPH::BroadPhaseLayer NON_MOVING(0);
        constexpr JPH::BroadPhaseLayer MOVING(1);
        constexpr JPH::uint NUM_LAYERS = 2;
    }

    class MyContactListener final : public JPH::ContactListener
    {
    public:
        // See: ContactListener
        JPH::ValidateResult OnContactValidate(
            [[maybe_unused]] const JPH::Body& inBody1,
            [[maybe_unused]] const JPH::Body& inBody2,
            [[maybe_unused]] JPH::RVec3Arg inBaseOffset,
            [[maybe_unused]] const JPH::CollideShapeResult& inCollisionResult) override
        {
            //cout << "Contact validate callback" << endl;

            // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
            return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
        }

        void OnContactAdded(
            [[maybe_unused]] const JPH::Body& inBody1,
            [[maybe_unused]] const JPH::Body& inBody2,
            [[maybe_unused]] const JPH::ContactManifold& inManifold,
            [[maybe_unused]] JPH::ContactSettings& ioSettings) override
        {
            //cout << "A contact was added" << endl;
        }

        void OnContactPersisted(
            [[maybe_unused]] const JPH::Body& inBody1,
            [[maybe_unused]] const JPH::Body& inBody2,
            [[maybe_unused]] const JPH::ContactManifold& inManifold,
            [[maybe_unused]] JPH::ContactSettings& ioSettings) override
        {
            //cout << "A contact was persisted" << endl;
        }

        void OnContactRemoved([[maybe_unused]] const JPH::SubShapeIDPair& inSubShapePair) override
        {
            //cout << "A contact was removed" << endl;
        }
    };


    class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
    {
    public:
        BPLayerInterfaceImpl()
        {
            // Create a mapping table from object to broad phase layer
            mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
            mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
        }

        [[nodiscard]] JPH::uint GetNumBroadPhaseLayers() const override
        {
            return BroadPhaseLayers::NUM_LAYERS;
        }

        [[nodiscard]] JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
        {
            JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
            return mObjectToBroadPhase[inLayer];
        }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        virtual const char *			GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
        {
            switch ((BroadPhaseLayer::Type)inLayer)
            {
                case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
                case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
                default:													JPH_ASSERT(false); return "INVALID";
            }
        }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

    private:
        JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS]{};
    };

    class ObjectVsBroadPhaseLayerFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
        {
            switch (inLayer1)
            {
            case Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
            }
        }
    };

    class ObjectLayerPairFilterImpl final : public JPH::ObjectLayerPairFilter
    {
    public:
        [[nodiscard]] bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
        {
            switch (inObject1)
            {
            case Layers::NON_MOVING:
                return inObject2 == Layers::MOVING; // Non moving only collides with moving
            case Layers::MOVING:
                return true; // Moving collides with everything
            default:
                JPH_ASSERT(false);
                return false;
            }
        }
    };

    enum class ShapeType { Box, Sphere, Cylinder, Tetrahedron, Pyramid };

    class PhysicsSystem final : public ecs::QuerySystem<
            ecs::Write<components::TransformComponent>,
            ecs::Write<components::PhysicsBodyComponent>
        >
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;

        PhysicsSystem(const PhysicsSystem&) = delete;
        PhysicsSystem& operator=(const PhysicsSystem&) = delete;

        void init();
        void update();

        JPH::BodyID createDynamicBody(ecs::Entity entity, const components::TransformComponent& transform) const;
        JPH::BodyID createStaticBody(ecs::Entity entity, const components::TransformComponent& transform) const;

        JPH::BodyID createBody(const components::TransformComponent& transform, JPH::EMotionType motionType) const;
        JPH::BodyID createBodyFromShape(ecs::Entity entity, const components::TransformComponent& transform,
                                        ShapeType shapeType, JPH::EMotionType motionType) const;


        void syncTransformsToBodies(
            const std::vector<ecs::Entity>& entities,
            ecs::Coordinator& coordinator
        ) const;

        void applyForce(JPH::BodyID bodyID, const JPH::Vec3& force) const;
        void setGravity(const JPH::Vec3& gravity) const;
        void activateBody(JPH::BodyID bodyID) const;
        void deactivateBody(JPH::BodyID bodyID) const;

        JPH::BodyInterface* getBodyInterface() const { return bodyInterface; }
        const JPH::BodyLockInterface* getBodyLockInterface() const { return bodyLockInterface; }

    private:
        JPH::TempAllocatorImpl* tempAllocator{};
        JPH::JobSystemThreadPool* jobSystem{};
        JPH::PhysicsSystem* physicsSystem{};
        JPH::BodyInterface* bodyInterface{};
        const JPH::BodyLockInterface* bodyLockInterface{};

        BPLayerInterfaceImpl broadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
        ObjectLayerPairFilterImpl objectLayerPairFilter;
        MyContactListener contactListener;

        double m_lastPhysicsTime = 0.0;

        // Using hard value because Jolt documentation advice that the physics simulation should be able to be at 60fps all the time
        constexpr static float fixedTimestep = 1.0f / 60.0f;
    };
}

#endif // PHYSICS_SYSTEM_HPP
