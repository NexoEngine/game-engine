using System;
using System.Runtime.InteropServices;

namespace Nexo
{
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
            public delegate void CreateCubeDelegate();

            // Function pointers
            public HelloFromNativeDelegate HelloFromNative;
            public AddNumbersDelegate AddNumbers;
            public GetNativeMessageDelegate GetNativeMessage;
            public NxLogDelegate NxLog;
            public CreateCubeDelegate CreateCube;
        }

        private static NativeApiCallbacks s_callbacks;

        /// <summary>
        /// Initialize the native API with the provided struct pointer and size.
        /// </summary>
        /// <param name="structPtr">Pointer to the struct</param>
        /// <param name="structSize">Size of the struct</param>
        [UnmanagedCallersOnly]
        public static void Initialize(IntPtr structPtr, Int32 structSize)
        {
            if (structSize != Marshal.SizeOf<NativeApiCallbacks>())
            {
                throw new ArgumentException($"Struct size mismatch between C++ and C# for {nameof(NativeApiCallbacks)}, expected {Marshal.SizeOf<NativeApiCallbacks>()}, got {structSize}");
            }

            // Marshal the struct from the IntPtr to the managed struct
            s_callbacks = Marshal.PtrToStructure<NativeApiCallbacks>(structPtr);
            Console.WriteLine("Native API initialized.");
        }

        /// <summary>
        /// Calls the HelloFromNative function in the native library
        /// </summary>
        public static void HelloFromNative()
        {
            try
            {
                s_callbacks.HelloFromNative?.Invoke();
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
                return s_callbacks.AddNumbers?.Invoke(a, b) ?? 0;
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
                IntPtr messagePtr = s_callbacks.GetNativeMessage?.Invoke() ?? IntPtr.Zero;
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
                s_callbacks.NxLog?.Invoke(level, message);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling NxLog: {ex.Message}");
            }
        }
        
        public static void CreateCube()
        {
            try
            {
                s_callbacks.CreateCube?.Invoke();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling CreateCube: {ex.Message}");
            }
        }

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
            CreateCube();
            
            Console.WriteLine("=== Native Call Demonstration Complete ===");
        }
        
    }
}
