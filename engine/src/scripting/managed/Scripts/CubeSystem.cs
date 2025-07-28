//// CubeSystem.cs ////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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
        Vector3 origin = new Vector3(0, 5, 0);
        
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
    
    protected override void OnUpdate(WorldState worldState)
    {
        Single deltaTime = (Single)worldState.Time.DeltaTime;
        
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
            SpawnCube(position, size, rotation, color);
        }
        
        foreach (var cubeId in _cubes)
        {
            MoveCube(cubeId, deltaTime);
        }
    }
    
    protected override void OnShutdown(WorldState worldState)
    {
        _cubes.Clear();
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
    }
    
}