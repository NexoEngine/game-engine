//// CubeSystem.cs ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/06/2025
//  Description: Source file for the Cube system in NEXO's ECS framework
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts;

public struct TestComponent() : IComponentBase
{
    public struct Nested()
    {
        public Int32 NestedValue = 42;
        public Vector3 NestedVector = new Vector3(1, 2, 3);
    }
    public Nested NestedComponent = new Nested();
    public Double RootValue = 84;
}

public class CubeSystem : SystemBase
{
    private struct CubeAnimationState : IComponentBase
    {
        public Single Angle;
        public Single BreathingPhase;
    }

    private readonly List<UInt32> _cubes = [];
    private readonly List<UInt32> _lights = [];
    private readonly List<Vector4> _colors = new(){
         new Vector4(1.0f, 0.0f, 1.0f, 1.0f), // Magenta
         new Vector4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
         new Vector4(1.0f, 0.5f, 0.0f, 1.0f), // Orange
         new Vector4(0.0f, 1.0f, 0.0f, 1.0f), // Green
         new Vector4(1.0f, 1.0f, 0.0f, 1.0f)  // Yellow
     };
    private static int _colorIndex = 0;
    private static bool _spawnLightOrCube = true;
    
    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} system");
    }
    
    private void MoveCube(UInt32 cubeId, Single deltaTime)
    {
        ref Transform transform = ref NativeInterop.GetComponent<Transform>(cubeId);
        ref CubeAnimationState state = ref NativeInterop.GetComponent<CubeAnimationState>(cubeId);
 
        // Rotating cube effect
        float rotationSpeed = 1.0f;
        transform.quat = Quaternion.CreateFromAxisAngle(Vector3.UnitY, deltaTime * rotationSpeed) * transform.quat;
        
        // Circling cube effect
        float speed = 1.0f;
        float radius = 7.0f;
        Vector3 origin = new Vector3(50.0f, 5.0f, 0.0f);
        
        state.Angle += speed * deltaTime;
        if (state.Angle > MathF.PI * 2.0f)
        {
            state.Angle -= MathF.PI * 2.0f;
        }
        
        transform.pos = origin + new Vector3(
            MathF.Cos(state.Angle) * radius,
            0,
            MathF.Sin(state.Angle) * radius
        );
        
        // Breathing cube effect
        float startScale = 1.0f;
        float endScale = 2.0f;
        float breathingSpeed = 0.5f;
        
        state.BreathingPhase += breathingSpeed * deltaTime * MathF.PI * 2.0f;
        if (state.BreathingPhase > MathF.PI * 2.0f)
        {
            state.BreathingPhase -= MathF.PI * 2.0f;
        }
        
        transform.size.Z = startScale + ((MathF.Sin(state.BreathingPhase) * 0.5f + 0.5f) * (endScale - startScale));
    }

    private void MoveLight(UInt32 lightId, Single deltaTime)
    {
        ref Transform transform = ref NativeInterop.GetComponent<Transform>(lightId);
        ref CubeAnimationState state = ref NativeInterop.GetComponent<CubeAnimationState>(lightId);

        // Circling light effect
        float speed = 1.0f;
        float radius = 7.0f;
        Vector3 origin = new Vector3(50.0f, 5.0f, 0.0f);

        state.Angle += speed * deltaTime;
        if (state.Angle > MathF.PI * 2.0f)
        {
            state.Angle -= MathF.PI * 2.0f;
        }

        transform.pos = origin + new Vector3(
            MathF.Cos(state.Angle) * radius,
            0,
            MathF.Sin(state.Angle) * radius
        );
    }
    
    private void SpawnCube(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color)
    {
        var cubeId = NativeInterop.CreateCube(position, size, rotation, color);
        var state = new CubeAnimationState
        {
            Angle = Random.Shared.NextSingle() * MathF.PI * 2.0f, 
            BreathingPhase = Random.Shared.NextSingle() * MathF.PI * 2.0f
        };
        NativeInterop.AddComponent<CubeAnimationState>(cubeId, ref state);
        var testComponent = new TestComponent();
        NativeInterop.AddComponent<TestComponent>(cubeId, ref testComponent);
        _cubes.Add(cubeId);
    }

    private void SpawnLight(Vector3 position, Vector4 color)
    {
        var lightId = NativeInterop.CreatePointLight(position, color);
        var state = new CubeAnimationState
        {
            Angle = Random.Shared.NextSingle() * MathF.PI * 2.0f,
            BreathingPhase = Random.Shared.NextSingle() * MathF.PI * 2.0f
        };
        NativeInterop.AddComponent<CubeAnimationState>(lightId, ref state);
        _lights.Add(lightId);
    }
    
    protected override void OnUpdate(WorldState worldState)
    {
        Single deltaTime = (Single)worldState.Time.DeltaTime;

        Byte maxCubes = 10;
        Byte maxLights = 5;
        
        // If 2 seconds have passed since last spawn, spawn a new cube
        if (worldState.Time.TotalTime % 2.0 < deltaTime)
        {
            Vector3 position = new Vector3(1, 4.2f, 3);
            Vector3 size = new Vector3(1, 1, 1);
            Vector3 rotation = new Vector3(7, 8, 9);
            Vector4 color = new Vector4(
                Random.Shared.NextSingle(),
                Random.Shared.NextSingle(),
                Random.Shared.NextSingle(),
                1.0f
            );

            if (_spawnLightOrCube)
            {
                if (_lights.Count < maxLights)
                {
                    SpawnLight(position, _colors[_colorIndex]);
                    _colorIndex = (_colorIndex + 1) % _colors.Count;
                }
            } else
            {
                if (_cubes.Count < maxCubes)
                {
                    SpawnCube(position, size, rotation, color);
                }
            }
            Logger.Log(LogLevel.Info, $"Spawned new {(_spawnLightOrCube ? "light" : "cube")}, total cubes: {_cubes.Count}, total lights: {_lights.Count}");
            _spawnLightOrCube ^= true;
        }
        
        foreach (var cubeId in _cubes)
        {
            MoveCube(cubeId, deltaTime);
        }
        foreach (var lightId in _lights)
        {
            MoveLight(lightId, deltaTime);
        }
    }
    
    protected override void OnShutdown(WorldState worldState)
    {
        _cubes.Clear();
        _lights.Clear();
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
    }
    
}