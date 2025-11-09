//// EarthRotationSystem.cs ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      PARENTEAU Thomas
//  Date:        09/11/2025
//  Description: System that rotates the Earth entity on itself
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts;

/// <summary>
/// System that makes the Earth rotate on its Y axis (like the real Earth!)
/// </summary>
public class EarthRotationSystem : SystemBase
{
    private readonly List<UInt32> _earthEntities = new();
    UInt32 earthEntity = 0;
    private Boolean _earthFound = false;

    private const Single RotationSpeed = 0.5f;

    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} system");

        for (int i = 0; i < 3; i++)
        {
            var earthName = $"Low Poly Earth-{i}";
            var earthId = NativeInterop.FindEntityByName(earthName);

            if (earthId != UInt32.MaxValue)
            {
                _earthEntities.Add(earthId);
                Logger.Log(LogLevel.Info, $"Found Earth mesh '{earthName}' with ID: {earthId}");
            }
            else
            {
                Logger.Log(LogLevel.Warn, $"Could not find Earth mesh '{earthName}'");
            }
        }

        if (_earthEntities.Count > 0)
        {
            _earthFound = true;
            Logger.Log(LogLevel.Info, $"Successfully found {_earthEntities.Count} Earth mesh(es)");
        }
        else
        {
            Logger.Log(LogLevel.Error, "No Earth meshes found! Make sure the entities exist in the scene.");
            _earthFound = false;
        }
    }

    protected override void OnUpdate(WorldState worldState)
    {
        if (!_earthFound)
            return;

        Single deltaTime = (Single)worldState.Time.DeltaTime;

        if (worldState.Time.TotalTime % 2.0 < deltaTime)
        {
            Logger.Log(LogLevel.Debug, $"[EarthRotation] Rotating {_earthEntities.Count} Earth meshes! Delta: {deltaTime:F4}s");
        }

        Quaternion rotation = Quaternion.CreateFromAxisAngle(Vector3.UnitY, deltaTime * RotationSpeed);

        foreach (var earthId in _earthEntities)
        {
            try
            {
                ref Transform transform = ref NativeInterop.GetComponent<Transform>(earthId);

                transform.quat = rotation * transform.quat;
                transform.dirty = true;
                NativeInterop.MarkHierarchyDirty(earthId);
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, $"Error rotating Earth mesh {earthId}: {ex.Message}");
            }
        }
    }

    protected override void OnShutdown(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
        _earthEntities.Clear();
        _earthFound = false;
    }
}
