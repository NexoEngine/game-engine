//// FanRotationSystem.cs ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      PARENTEAU Thomas
//  Date:        09/11/2025
//  Description: System that rotates the fan propeller vertically
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts;

/// <summary>
/// System that makes a fan propeller rotate on its Z axis (like a real fan!)
/// </summary>
public class FanRotationSystem : SystemBase
{
    private UInt32 _fanEntityId = UInt32.MaxValue;
    private Boolean _fanFound = false;

    private const Single RotationSpeed = 5.0f;

    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} system");

        _fanEntityId = NativeInterop.FindEntityByName("polySurface6_MAT_FanBase_0.001");

        if (_fanEntityId != UInt32.MaxValue)
        {
            _fanFound = true;
            Logger.Log(LogLevel.Info, $"Fan entity found with ID: {_fanEntityId}");
        }
        else
        {
            Logger.Log(LogLevel.Error, "Fan entity 'polySurface6_MAT_FanBase_0.001' not found!");
            _fanFound = false;
        }
    }

    protected override void OnUpdate(WorldState worldState)
    {
        if (!_fanFound)
            return;

        Single deltaTime = (Single)worldState.Time.DeltaTime;

        try
        {
            ref Transform transform = ref NativeInterop.GetComponent<Transform>(_fanEntityId);

            Quaternion rotation = Quaternion.CreateFromAxisAngle(Vector3.UnitZ, deltaTime * RotationSpeed);
            transform.quat = rotation * transform.quat;
            transform.dirty = true;
            NativeInterop.MarkHierarchyDirty(_fanEntityId);
        }
        catch (Exception ex)
        {
            Logger.Log(LogLevel.Error, $"Error rotating fan: {ex.Message}");
            _fanFound = false;
        }
    }

    protected override void OnShutdown(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
        _fanEntityId = UInt32.MaxValue;
        _fanFound = false;
    }
}
