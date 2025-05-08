//// Lib.cs ///////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/05/2025
//  Description: Source file for the NEXO managed library
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.InteropServices;

namespace Nexo
{
    public static class Lib
    {
        private static int _sCallCount = 1;

        [StructLayout(LayoutKind.Sequential)]
        public struct LibArgs
        {
            public IntPtr Message;
            public int Number;
        }

        public static int Hello(IntPtr arg, int argLength)
        {
            if (argLength < System.Runtime.InteropServices.Marshal.SizeOf(typeof(LibArgs)))
            {
                return 1;
            }

            LibArgs libArgs = Marshal.PtrToStructure<LibArgs>(arg);
            Console.WriteLine($"Hello, world! from {nameof(Lib)} [count: {_sCallCount++}]");
            PrintLibArgs(libArgs);
            return 0;
        }

        [UnmanagedCallersOnly]
        public static int Add(int a, int b)
        {
            return a + b;
        }
        
        [UnmanagedCallersOnly]
        public static int AddToPtr(int a, int b, IntPtr result)
        {
            if (result == IntPtr.Zero)
            {
                return 1;
            }

            Marshal.WriteInt32(result, a + b);
            return 0;
        }

        [UnmanagedCallersOnly]
        public static int AddNexoDllDirectory(IntPtr pPathString)
        {
            if (pPathString == IntPtr.Zero)
            {
                return 1;
            }
            string? pathString = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? Marshal.PtrToStringUni(pPathString)
                : Marshal.PtrToStringUTF8(pPathString);
            if (string.IsNullOrEmpty(pathString))
            {
                return 1;
            }
            string? path = System.IO.Path.GetDirectoryName(pathString);
            if (string.IsNullOrEmpty(path))
            {
                return 1;
            }

            return 0;
        }

        public delegate void CustomEntryPointDelegate(LibArgs libArgs);
        public static void CustomEntryPoint(LibArgs libArgs)
        {
            Console.WriteLine($"Hello, world! from {nameof(CustomEntryPoint)} in {nameof(Lib)}");
            PrintLibArgs(libArgs);
        }

        [UnmanagedCallersOnly]
        public static void CustomEntryPointUnmanagedCallersOnly(LibArgs libArgs)
        {
            Console.WriteLine($"Hello, world! from {nameof(CustomEntryPointUnmanagedCallersOnly)} in {nameof(Lib)}");
            PrintLibArgs(libArgs);
        }

        private static void PrintLibArgs(LibArgs libArgs)
        {
            string? message = RuntimeInformation.IsOSPlatform(OSPlatform.Windows)
                ? Marshal.PtrToStringUni(libArgs.Message)
                : Marshal.PtrToStringUTF8(libArgs.Message);
            
            message = message ?? "[ERROR] Could not convert message";

            Console.WriteLine($"-- message: {message}");
            Console.WriteLine($"-- number: {libArgs.Number}");
        }
    }
}
