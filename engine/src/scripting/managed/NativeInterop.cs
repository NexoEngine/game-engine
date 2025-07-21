//// NativeInterop.cs /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/05/2025
//  Description: Source file for the NativeInterop class in C#.
//               This class provides interop functionality for calling native
//               C++ functions.
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Nexo.Components;

namespace Nexo
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ComponentTypeIds
    {
        public UInt32 Transform;
        public UInt32 AmbientLight;
        public UInt32 DirectionalLight;
        public UInt32 PointLight;
        public UInt32 SpotLight;
        public UInt32 RenderComponent;
        public UInt32 SceneTag;
        public UInt32 CameraComponent;
        public UInt32 UuidComponent;
        public UInt32 PerspectiveCameraController;
        public UInt32 PerspectiveCameraTarget;
    }
    
    /// <summary>
    /// Provides interop functionality for calling native C++ functions from C# using function pointers.
    /// </summary>
    public static class NativeInterop
    {
        /// <summary>
        /// Native API struct that matches the C++ struct
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        private struct NativeApiCallbacks
        {
            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate void HelloFromNativeDelegate();

            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate Int32 AddNumbersDelegate(Int32 a, Int32 b);

            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate IntPtr GetNativeMessageDelegate();

            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate void NxLogDelegate(UInt32 level, String message);

            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate UInt32 CreateCubeDelegate(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color);
            
            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate ref Transform GetTransformDelegate(UInt32 entityId);
            
            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate IntPtr NxGetComponentDelegate(UInt32 typeId, UInt32 entityId);
            
            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate ComponentTypeIds NxGetComponentTypeIdsDelegate();
            
            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate void NxAddComponentDelegate(UInt32 typeId, UInt32 entityId);

            [UnmanagedFunctionPointer(CallingConvention.Winapi, CharSet = CharSet.Ansi)]
            public delegate bool NxHasComponentDelegate(UInt32 typeId, UInt32 entityId);

            // Function pointers
            public HelloFromNativeDelegate HelloFromNative;
            public AddNumbersDelegate AddNumbers;
            public GetNativeMessageDelegate GetNativeMessage;
            public NxLogDelegate NxLog;
            public CreateCubeDelegate CreateCube;
            public GetTransformDelegate GetTransform;
            public NxGetComponentDelegate NxGetComponent;
            public NxGetComponentTypeIdsDelegate NxGetComponentTypeIds;
            public NxAddComponentDelegate NxAddComponent;
            public NxHasComponentDelegate NxHasComponent;
        }

        private static NativeApiCallbacks s_callbacks;
        private static ComponentTypeIds _componentTypeIds;
        private static readonly Dictionary<Type, UInt32> _typeToNativeIdMap = new();
        
        /// <summary>
        /// Initialize the native API with the provided struct pointer and size.
        /// </summary>
        /// <param name="structPtr">Pointer to the struct</param>
        /// <param name="structSize">Size of the struct</param>
        [UnmanagedCallersOnly]
        public static Int32 Initialize(IntPtr structPtr, UInt32 structSize)
        {
            if (structSize != Marshal.SizeOf<NativeApiCallbacks>())
            {
                Logger.Log(LogLevel.Fatal, $"Struct size mismatch between C++ and C# for {nameof(NativeApiCallbacks)}, expected {Marshal.SizeOf<NativeApiCallbacks>()}, got {structSize}");
                return 1;
            }

            // Marshal the struct from the IntPtr to the managed struct
            s_callbacks = Marshal.PtrToStructure<NativeApiCallbacks>(structPtr);
            _componentTypeIds = s_callbacks.NxGetComponentTypeIds.Invoke();
            if (InitializeTypeMap() != 0)
            {
                Logger.Log(LogLevel.Fatal, "Failed to initialize type map for component type IDs.");
                return 1;
            }
            
            Logger.Log(LogLevel.Info, "Native API initialized.");
            return 0;
        }
        
        private static Int32 InitializeTypeMap()
        {
            var fields = typeof(ComponentTypeIds).GetFields();
            foreach (var field in fields)
            {
                var expectedTypeName = $"Nexo.Components.{field.Name}";
                var type = Type.GetType(expectedTypeName);

                if (type != null)
                {
                    var value = field.GetValue(_componentTypeIds);
                    if (value == null)
                    {
                        Logger.Log(LogLevel.Warn, $"Field {field.Name} in ComponentTypeIds is null");
                        return 1;
                    }
                    _typeToNativeIdMap[type] = (UInt32)value;
                    Logger.Log(LogLevel.Debug, $"[Interop] Mapped {expectedTypeName} => {value}");
                }
                else
                {
                    Logger.Log(LogLevel.Warn, $"[Interop] Type not found for field {field.Name} (expected {expectedTypeName})");
                }
            }

            return 0;
        }

        /// <summary>
        /// Calls the HelloFromNative function in the native library
        /// </summary>
        public static void HelloFromNative()
        {
            try
            {
                s_callbacks.HelloFromNative.Invoke();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling HelloFromNative: {ex.Message}");
            }
        }

        /// <summary>
        /// Calls the AddNumbers function in the native library
        /// </summary>
        public static Int32 AddNumbers(Int32 a, Int32 b)
        {
            try
            {
                return s_callbacks.AddNumbers.Invoke(a, b);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling AddNumbers: {ex.Message}");
                return 0;
            }
        }

        /// <summary>
        /// Calls the GetNativeMessage function in the native library
        /// </summary>
        public static String GetNativeMessage()
        {
            try
            {
                IntPtr messagePtr = s_callbacks.GetNativeMessage.Invoke();
                return messagePtr != IntPtr.Zero ? Marshal.PtrToStringAnsi(messagePtr) ?? string.Empty : string.Empty;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling GetNativeMessage: {ex.Message}");
                return string.Empty;
            }
        }

        /// <summary>
        /// Logs a message using the native NxLog function
        /// </summary>
        /// <param name="level">The level of the log message</param>
        /// <param name="message">The message to log</param>
        public static void NxLog(UInt32 level, String message)
        {
            try
            {
                s_callbacks.NxLog.Invoke(level, message);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling NxLog: {ex.Message}");
                Console.WriteLine($"Fallback to WriteLine: Log Level: {level}, Message: {message}");
            }
        }
        
        public static UInt32 CreateCube(in Vector3 position, in Vector3 size, in Vector3 rotation, in Vector4 color)
        {
            try
            {
                return s_callbacks.CreateCube.Invoke(position, size, rotation, color);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling CreateCube: {ex.Message}");
                return UInt32.MaxValue;
            }
        }
        
        public static ref Transform GetTransform(UInt32 entityId)
        {
            try
            {
                return ref s_callbacks.GetTransform.Invoke(entityId);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling GetTransform: {ex.Message}");
                throw new InvalidOperationException($"Failed to get transform for entity {entityId}", ex);
            }
        }
        
        public static unsafe ref T GetComponent<T>(UInt32 entityId) where T : unmanaged
        {
            if (!_typeToNativeIdMap.TryGetValue(typeof(T), out var typeId))
                throw new InvalidOperationException($"Unsupported component type: {typeof(T)}");

            IntPtr ptr = s_callbacks.NxGetComponent(typeId, entityId);
            if (ptr == IntPtr.Zero)
                throw new InvalidOperationException($"Component {typeof(T)} not found on entity {entityId}");

            return ref Unsafe.AsRef<T>((void*)ptr);
        }
        
        public static void AddComponent<T>(UInt32 entityId)
        {
            if (!_typeToNativeIdMap.TryGetValue(typeof(T), out var typeId))
                throw new InvalidOperationException($"Unsupported component type: {typeof(T)}");

            try
            {
                s_callbacks.NxAddComponent.Invoke(typeId, entityId);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling AddComponent<{typeof(T)}>: {ex.Message}");
            }
        }
        
        public static bool HasComponent<T>(UInt32 entityId)
        {
            if (!_typeToNativeIdMap.TryGetValue(typeof(T), out var typeId))
                throw new InvalidOperationException($"Unsupported component type: {typeof(T)}");

            try
            {
                return s_callbacks.NxHasComponent.Invoke(typeId, entityId);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling HasComponent<{typeof(T)}>: {ex.Message}");
                return false;
            }
        }

        
        private static UInt32 _cubeId = 0;

        /// <summary>
        /// Demonstrates calling native functions from C#
        /// </summary>
        [UnmanagedCallersOnly]
        public static void DemonstrateNativeCalls()
        {
            Console.WriteLine("=== Starting Native Call Demonstration ===");
            
            // Call the void function
            Console.WriteLine("Calling HelloFromNative:");
            HelloFromNative();
            
            // Call the function that returns an int
            const Int32 a = 42;
            const Int32 b = 123;
            Console.WriteLine($"Calling AddNumbers({a}, {b}):");
            Int32 result = AddNumbers(a, b);
            Console.WriteLine($"Result: {result}");
            
            // Call the function that returns a string
            Console.WriteLine("Calling GetNativeMessage:");
            String message = GetNativeMessage();
            Logger.Log(LogLevel.Info, $"Logging from C# :) got native message: {message}");
            
            // Call the function that creates a cube
            Console.WriteLine("Calling CreateCube:");
            UInt32 cubeId = CreateCube(new Vector3(1, 4.2f, 3), new Vector3(1, 1, 1), new Vector3(7, 8, 9), new Vector4(1, 0, 0, 1));
            _cubeId = cubeId;
            Console.WriteLine($"Created cube with ID: {cubeId}");
            
            // AddComponent test
            AddComponent<UuidComponent>(cubeId);
            
            try {
                ref UuidComponent uuid = ref GetComponent<UuidComponent>(cubeId);
                Console.WriteLine($"Successfully got UuidComponent for cube");
            }
            catch (Exception e) {
                Console.WriteLine($"Failed to get UuidComponent: {e.Message}");
            }
            
            // HasComponent test
            if (HasComponent<CameraComponent>(cubeId))
                Console.WriteLine("Entity has a camera!");
            else
                Console.WriteLine("Entity does NOT have a camera.");
            
            if (HasComponent<Transform>(cubeId))
                Console.WriteLine("Entity has a Transform!");
            else
                Console.WriteLine("Entity does NOT have a Transform.");
            
            if (HasComponent<AmbientLight>(cubeId))
                Console.WriteLine("Entity has a AmbientLight!");
            else
                Console.WriteLine("Entity does NOT have a AmbientLight.");
            
            // Call the function that gets a transform
            Console.WriteLine($"Calling GetComponent({cubeId}):");
            ref Transform transform = ref GetComponent<Transform>(cubeId);
            Console.WriteLine($"Transform for cube {cubeId}: Position: {transform.pos}, Scale: {transform.size}, Rotation Quat: {transform.quat}");

            // Write the localMatrix for the cube
            Console.WriteLine($"Local Matrix for cube {cubeId}:\n{transform.localMatrix}");
            
            
            Console.WriteLine("=== Native Call Demonstration Complete ===");
        }
        
        private static float _angle = 0.0f;
        private static float _breathingScale = 1.0f;

        [UnmanagedCallersOnly]
        public static void Update(Double deltaTime)
        {
            ref Transform transform = ref GetComponent<Transform>(_cubeId);
            
            // Rotating cube effect
            float rotationSpeed = 1.0f; // radians per second
            transform.quat = Quaternion.CreateFromAxisAngle(Vector3.UnitY, (float)deltaTime * rotationSpeed) * transform.quat;
            
            // Circling cube effect
            float speed = 1.0f;
            float radius = 7.0f;
            Vector3 origin = new Vector3(0, 5, 0);

            _angle += (float)(speed * deltaTime);
            
            if (_angle > MathF.PI * 2.0f)
            {
                _angle = 0.0f;
            }
            
            transform.pos = origin + new Vector3(
                (float)Math.Cos(_angle) * radius,
                0,
                (float)Math.Sin(_angle) * radius
            );

            // Breathing cube effect
            float startScale = 1.0f;
            float endScale = 2.0f;
            float breathingSpeed = 0.5f;

            _breathingScale += (float)(breathingSpeed * deltaTime * MathF.PI * 2.0f);
            if (_breathingScale > MathF.PI * 2.0f)
            {
                _breathingScale -= MathF.PI * 2.0f;
            }

            // Update the size of the cube based on the breathing effect
            transform.size.Z = startScale + ((MathF.Sin(_breathingScale) * 0.5f + 0.5f) * (endScale - startScale));
        }

    }
}
