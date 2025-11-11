//// PlaneRotationSystem.cs //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      PARENTEAU Thomas
//  Date:        09/11/2025
//  Description: System that makes the plane fly in circles with proper orientation
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts;

/// <summary>
/// System that makes a plane fly in circles with the nose always pointing forward
/// </summary>
public class PlaneRotationSystem : SystemBase
{
    private UInt32 _planeId;
    private Boolean _planeFound = false;
    private UInt32 _propellerId;

    // Flight parameters
    private Single _angle = 0.0f;
    private const Single FlightSpeed = 0.5f;
    private const Single FlightRadius = 2.5f;
    private readonly Vector3 _centerPosition = new Vector3(4.5f, 7.0f, -3.0f);

    private const Single BankAngle = 0.3f;

    private const Single PropellerSpinSpeed = 25.0f;

    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} system");
        var planeName = "Low Poly Biplane Body";
        var planeId = NativeInterop.FindEntityByName(planeName);
        var propellerName = "Low Poly Biplane Propeller";
        var propellerId = NativeInterop.FindEntityByName(propellerName);

        if (planeId != UInt32.MaxValue && propellerId != UInt32.MaxValue)
        {
            _planeFound = true;
            _planeId = planeId;
            _propellerId = propellerId;
            Logger.Log(LogLevel.Info, $"Successfully found plane model with id {planeId}");
        }
        else
        {
            Logger.Log(LogLevel.Error, "No plane model found! Make sure the entities exist in the scene.");
            _planeFound = false;
        }
    }

    protected override void OnUpdate(WorldState worldState)
    {
        if (!_planeFound)
            return;

        Single deltaTime = (Single)worldState.Time.DeltaTime;

        _angle += FlightSpeed * deltaTime;
        if (_angle > MathF.PI * 2.0f)
        {
            _angle -= MathF.PI * 2.0f;
        }

        Vector3 newPosition = _centerPosition + new Vector3(
            MathF.Cos(_angle) * FlightRadius,
            0.0f,
            MathF.Sin(_angle) * FlightRadius
        );
        Vector3 forwardDirection = new Vector3(
            -MathF.Sin(_angle),
            0.0f,
            MathF.Cos(_angle)
        );
        forwardDirection = Vector3.Normalize(forwardDirection);

        float yawAngle = MathF.Atan2(forwardDirection.X, forwardDirection.Z);

        Quaternion yawRotation = Quaternion.CreateFromAxisAngle(Vector3.UnitY, yawAngle);

        Quaternion bankRotation = Quaternion.CreateFromAxisAngle(Vector3.UnitZ, BankAngle);

        Quaternion finalRotation = yawRotation * bankRotation;

        try
        {
            ref Transform transform = ref NativeInterop.GetComponent<Transform>(_planeId);

            transform.pos = newPosition;

            transform.quat = finalRotation;
            transform.dirty = true;
            NativeInterop.MarkHierarchyDirty(_planeId);
        }
        catch (Exception ex)
        {
            Logger.Log(LogLevel.Error, $"Error updating plane mesh {_planeId}: {ex.Message}");
        }

        try
        {
            ref Transform propTransform = ref NativeInterop.GetComponent<Transform>(_propellerId);

            float spinDelta = PropellerSpinSpeed * deltaTime;

            Quaternion spin = Quaternion.CreateFromAxisAngle(Vector3.UnitZ, spinDelta);

            propTransform.quat = Quaternion.Normalize(propTransform.quat * spin);

            propTransform.dirty = true;
            NativeInterop.MarkHierarchyDirty(_propellerId);
        }
        catch (Exception ex)
        {
            Logger.Log(LogLevel.Error, $"Error spinning propeller {_propellerId}: {ex.Message}");
        }
    }

    protected override void OnShutdown(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
        _planeFound = false;
    }
}
