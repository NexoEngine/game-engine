//// CameraLinearMoveSystem.cs ////////////////////////////////////////////////
//
//  Moves "Demo_camera" linearly in direction (-X, +Y, 0) with ease-in/out.
//  Stops once it reaches MaxDistance. Rotation is preserved.
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts
{
    public class CameraLinearMoveSystem : SystemBase
    {
        private const string TargetName   = "Demo_camera";

        // Motion parameters
        private static readonly Vector3 Direction = Vector3.Normalize(new Vector3(-1f, 1f, 0f)); // -X, +Y
        private const float Speed       = 2.0f;   // nominal average speed (m/s)
        private const float MaxDistance = 8.0f;   // meters to travel along the direction

        private uint    _entityId = UInt32.MaxValue;
        private bool    _found;
        private bool    _finished;
        private Vector3 _startPos;

        // Time-based easing
        private float _elapsed;   // seconds
        private float _duration;  // seconds = MaxDistance / Speed

        protected override void OnInitialize(WorldState worldState)
        {
            var id = NativeInterop.FindEntityByName(TargetName);
            if (id == UInt32.MaxValue)
            {
                _found = false;
                Logger.Log(LogLevel.Error, $"Entity \"{TargetName}\" not found.");
                return;
            }

            _entityId = id;
            _found = true;
            _finished = false;
            _elapsed = 0f;
            _duration = MathF.Max(1e-4f, MaxDistance / MathF.Max(1e-4f, Speed)); // guard against div-by-zero

            try
            {
                ref Transform t = ref NativeInterop.GetComponent<Transform>(_entityId);
                _startPos = t.pos;
                Logger.Log(LogLevel.Info, $"Found \"{TargetName}\" (id={_entityId}) at {_startPos}. Duration={_duration:0.###}s");
            }
            catch (Exception ex)
            {
                _found = false;
                Logger.Log(LogLevel.Error, $"Could not read Transform: {ex.Message}");
            }
        }

        protected override void OnUpdate(WorldState worldState)
        {
            if (!_found || _finished) return;

            float dt = (float)worldState.Time.DeltaTime;
            _elapsed += dt;

            float t = MathF.Min(1f, _elapsed / _duration); // normalized time [0..1]

            // EASING (pick one)
            float eased = EaseInOutSine(t);
            // float eased = SmoothStep(t);

            Vector3 pos = _startPos + Direction * (MaxDistance * eased);

            try
            {
                ref Transform tr = ref NativeInterop.GetComponent<Transform>(_entityId);
                tr.pos = pos;   // rotation unchanged
                tr.dirty = true;
                NativeInterop.MarkHierarchyDirty(_entityId);
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, $"Error moving \"{TargetName}\" (id={_entityId}): {ex.Message}");
            }

            if (t >= 1f)
                _finished = true;
        }

        protected override void OnShutdown(WorldState worldState)
        {
            _found = false;
            _finished = false;
        }

        // --- Easing functions ---

        // Smooth and natural ease-in/out (sine curve)
        private static float EaseInOutSine(float t)
        {
            return 0.5f * (1f - MathF.Cos(MathF.PI * t));
        }

        // Classic smoothstep: 3t^2 - 2t^3
        private static float SmoothStep(float t)
        {
            return t * t * (3f - 2f * t);
        }
    }
}
