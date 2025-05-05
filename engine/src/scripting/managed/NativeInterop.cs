using System;
using System.Runtime.InteropServices;

namespace Nexo
{
    /// <summary>
    /// Provides P/Invoke declarations for calling native C++ functions from C#
    /// </summary>
    public static class NativeInterop
    {
        // The name of the native library that contains our exported functions
        // On Windows, this would typically be "engine.dll"
        // On macOS, it would be "libengine.dylib"
        // On Linux, it would be "libengine.so"
        // Since we're using dynamic loading, we'll set this at runtime
        private static string s_nativeLibraryName;

        /// <summary>
        /// Sets the native library name to use for P/Invoke calls
        /// </summary>
        public static void SetNativeLibraryName(string libraryName)
        {
            s_nativeLibraryName = libraryName;
            Console.WriteLine($"Native library name set to: {libraryName}");
        }

        /// <summary>
        /// Import of the HelloFromNative function from the native library
        /// </summary>
        [DllImport("nexoApi", EntryPoint = "HelloFromNative")]
        private static extern void HelloFromNativeInternal();

        /// <summary>
        /// Import of the AddNumbers function from the native library
        /// </summary>
        [DllImport("nexoApi", EntryPoint = "AddNumbers")]
        private static extern int AddNumbersInternal(int a, int b);

        /// <summary>
        /// Import of the GetNativeMessage function from the native library
        /// </summary>
        [DllImport("nexoApi", EntryPoint = "GetNativeMessage")]
        private static extern IntPtr GetNativeMessageInternal();

        /// <summary>
        /// Calls the HelloFromNative function in the native library
        /// </summary>
        public static void HelloFromNative()
        {
            try
            {
                HelloFromNativeInternal();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling HelloFromNative: {ex.Message}");
            }
        }

        /// <summary>
        /// Calls the AddNumbers function in the native library
        /// </summary>
        public static int AddNumbers(int a, int b)
        {
            try
            {
                return AddNumbersInternal(a, b);
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
        public static string GetNativeMessage()
        {
            try
            {
                IntPtr messagePtr = GetNativeMessageInternal();
                if (messagePtr == IntPtr.Zero)
                    return string.Empty;
                
                return Marshal.PtrToStringAnsi(messagePtr);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error calling GetNativeMessage: {ex.Message}");
                return string.Empty;
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
            int a = 42;
            int b = 123;
            Console.WriteLine($"Calling AddNumbers({a}, {b}):");
            int result = AddNumbers(a, b);
            Console.WriteLine($"Result: {result}");
            
            // Call the function that returns a string
            Console.WriteLine("Calling GetNativeMessage:");
            string message = GetNativeMessage();
            Console.WriteLine($"Received message: {message}");
            
            Console.WriteLine("=== Native Call Demonstration Complete ===");
        }
    }
}
