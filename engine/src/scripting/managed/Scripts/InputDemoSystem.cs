//// InputDemoSystem.cs ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        25/06/2025
//  Description: Demo system showing input handling capabilities
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts;

/// <summary>
/// Demonstration system that shows various input handling capabilities:
/// - Spawning cubes with keyboard input
/// - Deleting cubes with keyboard input
/// - Changing cube colors based on mouse position
/// - Different behaviors for key combinations
/// </summary>
public class InputDemoSystem : SystemBase
{
    private readonly List<UInt32> _cubes = [];
    private bool _spaceWasPressed = false;
    private bool _deleteWasPressed = false;
    private float _timeSinceLastSpawn = 0.0f;
    private const float SpawnCooldown = 0.5f; // Prevent spam spawning
    
    protected override void OnInitialize(WorldState worldState)
    {
        Logger.Log(LogLevel.Info, $"Initializing {Name} - Press SPACE to spawn cubes, DELETE to remove them!");
        Logger.Log(LogLevel.Info, "Hold SHIFT while pressing SPACE for rainbow cubes!");
        Logger.Log(LogLevel.Info, "Left click makes cubes jump, right click shows mouse position");
    }
    
    protected override void OnUpdate(WorldState worldState)
    {
        Single deltaTime = (Single)worldState.Time.DeltaTime;
        _timeSinceLastSpawn += deltaTime;
        
        // Handle cube spawning with SPACE key
        HandleCubeSpawning();
        
        // Handle cube deletion with DELETE key
        HandleCubeDeletion();
        
        // Handle mouse-based color changes
        HandleMouseColorChange();
        
        // Handle arrow keys for moving last spawned cube
        HandleCubeMovement(deltaTime);
        
        // Display help message when TAB is pressed
        if (Input.IsKeyPressed(KeyCode.Tab))
        {
            Logger.Log(LogLevel.Info, "=== Input Demo Controls ===");
            Logger.Log(LogLevel.Info, "SPACE: Spawn a cube");
            Logger.Log(LogLevel.Info, "SHIFT+SPACE: Spawn a rainbow cube");
            Logger.Log(LogLevel.Info, "DELETE: Remove last cube");
            Logger.Log(LogLevel.Info, "Arrow Keys: Move last cube");
            Logger.Log(LogLevel.Info, "Left Click: Make cubes jump");
            Logger.Log(LogLevel.Info, "Right Click: Log mouse position");
        }
    }
    
    private void HandleCubeSpawning()
    {
        bool spacePressed = Input.IsKeyPressed(KeyCode.Space);
        
        // Detect key press (transition from released to pressed)
        if (spacePressed && !_spaceWasPressed && _timeSinceLastSpawn >= SpawnCooldown)
        {
            bool shiftHeld = Input.IsKeyPressed(KeyCode.Shift);
            SpawnCube(shiftHeld);
            _timeSinceLastSpawn = 0.0f;
        }
        
        _spaceWasPressed = spacePressed;
    }
    
    private void HandleCubeDeletion()
    {
        bool deletePressed = Input.IsKeyPressed(KeyCode.Delete);
        
        // Detect key press (transition from released to pressed)
        if (deletePressed && !_deleteWasPressed && _cubes.Count > 0)
        {
            UInt32 lastCube = _cubes[_cubes.Count - 1];
            _cubes.RemoveAt(_cubes.Count - 1);
            Logger.Log(LogLevel.Info, $"Deleted cube {lastCube}. Remaining cubes: {_cubes.Count}");
        }
        
        _deleteWasPressed = deletePressed;
    }
    
    private void HandleMouseColorChange()
    {
        Vector2 mousePos = Input.GetMousePosition();
        
        // Left click makes cubes jump
        if (Input.IsMouseDown(MouseButton.Left))
        {
            foreach (var cubeId in _cubes)
            {
                ref Transform transform = ref NativeInterop.GetComponent<Transform>(cubeId);
                // Add a small upward impulse effect
                transform.pos.Y = MathF.Max(transform.pos.Y, 5.0f + MathF.Sin((float)_timeSinceLastSpawn * 10.0f) * 2.0f);
            }
        }
        
        // Log mouse position when right clicking
        if (Input.IsMouseDown(MouseButton.Right))
        {
            Logger.Log(LogLevel.Info, $"Mouse position: {mousePos.X}, {mousePos.Y}");
        }
    }
    
    private void HandleCubeMovement(float deltaTime)
    {
        if (_cubes.Count == 0) return;
        
        // Move the last spawned cube with arrow keys
        UInt32 lastCube = _cubes[_cubes.Count - 1];
        ref Transform transform = ref NativeInterop.GetComponent<Transform>(lastCube);
        
        float moveSpeed = 5.0f * deltaTime;
        
        if (Input.IsKeyPressed(KeyCode.Left))
            transform.pos.X -= moveSpeed;
        if (Input.IsKeyPressed(KeyCode.Right))
            transform.pos.X += moveSpeed;
        if (Input.IsKeyPressed(KeyCode.Up))
            transform.pos.Z -= moveSpeed;
        if (Input.IsKeyPressed(KeyCode.Down))
            transform.pos.Z += moveSpeed;
    }
    
    private void SpawnCube(bool rainbow)
    {
        // Random position around the center
        Vector3 position = new Vector3(
            Random.Shared.NextSingle() * 10.0f - 5.0f,
            Random.Shared.NextSingle() * 5.0f + 2.0f,
            Random.Shared.NextSingle() * 10.0f - 5.0f
        );
        
        Vector3 size = Vector3.One;
        Vector3 rotation = Vector3.Zero;
        
        Vector4 color;
        if (rainbow)
        {
            // Create rainbow effect
            float hue = Random.Shared.NextSingle();
            color = HsvToRgb(hue, 1.0f, 1.0f);
        }
        else
        {
            // Random color
            color = new Vector4(
                Random.Shared.NextSingle(),
                Random.Shared.NextSingle(),
                Random.Shared.NextSingle(),
                1.0f
            );
        }
        
        UInt32 cubeId = NativeInterop.CreateCube(position, size, rotation, color);
        _cubes.Add(cubeId);
        
        Logger.Log(LogLevel.Info, $"Spawned {(rainbow ? "rainbow" : "regular")} cube {cubeId} at {position}. Total cubes: {_cubes.Count}");
    }
    
    private static Vector4 HsvToRgb(float h, float s, float v)
    {
        float c = v * s;
        float x = c * (1 - MathF.Abs((h * 6) % 2 - 1));
        float m = v - c;
        
        float r, g, b;
        if (h < 1.0f / 6.0f)
        {
            r = c; g = x; b = 0;
        }
        else if (h < 2.0f / 6.0f)
        {
            r = x; g = c; b = 0;
        }
        else if (h < 3.0f / 6.0f)
        {
            r = 0; g = c; b = x;
        }
        else if (h < 4.0f / 6.0f)
        {
            r = 0; g = x; b = c;
        }
        else if (h < 5.0f / 6.0f)
        {
            r = x; g = 0; b = c;
        }
        else
        {
            r = c; g = 0; b = x;
        }
        
        return new Vector4(r + m, g + m, b + m, 1.0f);
    }
    
    protected override void OnShutdown(WorldState worldState)
    {
        _cubes.Clear();
        Logger.Log(LogLevel.Info, $"Shutting down {Name} system");
    }
}
