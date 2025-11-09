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
    private readonly List<UInt32> _planeEntities = new();
    private Boolean _planeFound = false;

    // Flight parameters
    private Single _angle = 0.0f;
    private const Single FlightSpeed = 0.5f;)
    private const Single FlightRadius = 15.0f;
    private readonly Vector3 _centerPosition = new Vector3(0.0f, 0.0f, 20.0f);

    private const Single BankAngle = 0.3f;

    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} system");

        for (int i = 0; i < 4; i++)
        {
            var planeName = $"Low Poly Biplane Propeller-{i}";
            var planeId = NativeInterop.FindEntityByName(planeName);

            if (planeId != UInt32.MaxValue)
            {
                _planeEntities.Add(planeId);
                Logger.Log(LogLevel.Info, $"Found plane mesh '{planeName}' with ID: {planeId}");
            }
            else
            {
                Logger.Log(LogLevel.Warn, $"Could not find plane mesh '{planeName}'");
            }
        }

        if (_planeEntities.Count > 0)
        {
            _planeFound = true;
            Logger.Log(LogLevel.Info, $"Successfully found {_planeEntities.Count} plane mesh(es)");
        }
        else
        {
            Logger.Log(LogLevel.Error, "No plane meshes found! Make sure the entities exist in the scene.");
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
            0.0f, // Keep constant height
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

        Quaternion bankRotation = Quaternion.CreateFromAxisAngle(Vector3.UnitZ, -BankAngle);

        Quaternion finalRotation = yawRotation * bankRotation;

        foreach (var planeId in _planeEntities)
        {
            try
            {
                ref Transform transform = ref NativeInterop.GetComponent<Transform>(planeId);

                transform.pos = newPosition;

                transform.quat = finalRotation;
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, $"Error updating plane mesh {planeId}: {ex.Message}");
            }
        }
    }

    protected override void OnShutdown(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
        _planeEntities.Clear();
        _planeFound = false;
    }
}
