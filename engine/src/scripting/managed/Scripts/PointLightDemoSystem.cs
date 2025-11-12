//// PointLightMoveEaseInSystem.cs ////////////////////////////////////////////
//
//  Moves "Demo_point_light" along +Z with ease-in (accelerates then stops).
//  Rotation is preserved; stops once MaxDistance is reached.
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Numerics;
using Nexo.Components;
using Nexo.Systems;

namespace Nexo.Scripts
{
    public class PointLightMoveEaseInSystem : SystemBase
    {
        private const string TargetName = "Demo_point_light";

        // Motion parameters
        private static readonly Vector3 Direction = -Vector3.UnitZ; // -Z
        private const float Speed       = 2.0f;   // nominal avg speed (m/s)
        private const float MaxDistance = 10.0f;   // meters along +Z

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

            _entityId  = id;
            _found     = true;
            _finished  = false;
            _elapsed   = 0f;
            _duration  = MathF.Max(1e-4f, MaxDistance / MathF.Max(1e-4f, Speed));

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

            // Ease-in only (starts slow, accelerates): choose curve
            float eased = EaseInCubic(t); // alternatives: EaseInQuad(t), EaseInQuint(t)

            Vector3 pos = _startPos + Direction * (MaxDistance * eased);

            try
            {
                ref Transform tr = ref NativeInterop.GetComponent<Transform>(_entityId);
                tr.pos = pos;   // keep rotation as-is
                tr.dirty = true;
                NativeInterop.MarkHierarchyDirty(_entityId);
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, $"Error moving \"{TargetName}\" (id={_entityId}): {ex.Message}");
            }

            if (t >= 1f) _finished = true;
        }

        protected override void OnShutdown(WorldState worldState)
        {
            _found = false;
            _finished = false;
        }

        // --- Easing (ease-in) ---
        private static float EaseInQuad(float t)   => t * t;
        private static float EaseInCubic(float t)  => t * t * t;
        private static float EaseInQuint(float t)  => t * t * t * t * t;
    }
}
