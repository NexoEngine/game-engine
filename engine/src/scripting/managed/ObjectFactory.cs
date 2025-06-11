//// ObjectFactory.cs /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        03/05/2025
//  Description: Source file for the scripting object factory
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo
{
    
    class ExampleClass(Int32 value)
    {
        public Int32 Value { get; set; } = value;
    }
    
    public static class ObjectFactory
    {
        // Generic factory method using Activator
        
        [UnmanagedCallersOnly]
        public static IntPtr CreateInstance(IntPtr typeName, IntPtr parametersTypes, IntPtr parameters, UInt32 parametersCount)
        {
            try 
            {
                if (typeName == IntPtr.Zero) {
                    Logger.Log(LogLevel.Error, "typeName is IntPtr.Zero");
                    return IntPtr.Zero;
                }
                
                string? typeNameStr = Marshal.PtrToStringAnsi(typeName);
                if (string.IsNullOrEmpty(typeNameStr)) {
                    Logger.Log(LogLevel.Error, "typeNameStr is null or empty");
                    return IntPtr.Zero;
                }
                
                Type? type = Type.GetType(typeNameStr);
                if (type == null) {
                    Logger.Log(LogLevel.Error, $"Type not found: {typeNameStr}");
                    return IntPtr.Zero;
                }
                
                Type[] parameterTypes = new Type[parametersCount];
                object[] parametersArray = new object[parametersCount];
                for (UInt32 i = 0; i < parametersCount; i++)
                {
                    IntPtr paramTypePtr = Marshal.ReadIntPtr(parametersTypes, (int)(i * IntPtr.Size));
                    string? paramTypeStr = Marshal.PtrToStringAnsi(paramTypePtr);
                    if (string.IsNullOrEmpty(paramTypeStr)) {
                        Logger.Log(LogLevel.Error, $"Parameter type string at index {i} is null or empty");
                        return IntPtr.Zero;
                    }
                    
                    Type? paramType = Type.GetType(paramTypeStr);
                    if (paramType == null) {
                        Logger.Log(LogLevel.Error, $"Parameter type not found: {paramTypeStr} at index {i}");
                        return IntPtr.Zero;
                    }
                    parameterTypes[i] = paramType;
                    
                    IntPtr paramPtr = Marshal.ReadIntPtr(parameters, (int)(i * IntPtr.Size));
                    if (paramPtr == IntPtr.Zero)
                    {
                        Logger.Log(LogLevel.Error, $"Parameter pointer at index {i} is IntPtr.Zero");
                        return IntPtr.Zero;
                    }
                    
                    if (paramType.IsByRef)
                    {
                        // If it's a reference type, we can just use the pointer directly
                        parametersArray[i] = paramPtr;
                    } else
                    {
                        // If it's a value type, we need to marshal it correctly
                        object? val = Marshal.PtrToStructure(paramPtr, paramType);
                        if (val == null)
                        {
                            Logger.Log(LogLevel.Error, $"Failed to marshal value type at index {i}");
                            return IntPtr.Zero;
                        }
                        parametersArray[i] = val;
                    } 
                }
                
                object? instance = Activator.CreateInstance(type, parametersArray);
                if (instance == null)
                {
                    Logger.Log(LogLevel.Error, "Failed to create instance of the type");
                    return IntPtr.Zero;
                }
                
                GCHandle handle = GCHandle.Alloc(instance, GCHandleType.Pinned);
                try
                {
                    IntPtr pointer = GCHandle.ToIntPtr(handle);
                    return pointer;
                }
                catch
                {
                    handle.Free();
                    Logger.Log(LogLevel.Error, "Failed to pin the handle");
                    return IntPtr.Zero;
                }
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, $"Exception in CreateInstance: {ex.Message}");
                return IntPtr.Zero;
            }
        }
        
        // // Create with parameters
        // public static int CreateInstanceWithParams(string typeName, object[] parameters)
        // {
        //     try 
        //     {
        //         Type? type = Type.GetType(typeName);
        //         if (type == null)
        //             return -1;
        //             
        //         object? instance = Activator.CreateInstance(type, parameters);
        //         if (instance == null)
        //             return -1;
        //         int id = _nextId++;
        //         _instances[id] = instance;
        //         return id;
        //     }
        //     catch
        //     {
        //         return -1;
        //     }
        // }
        //
        // public static 
        //
        // // Method to release the instance
        // public static bool ReleaseInstance(int id)
        // {
        //     return _instances.Remove(id);
        // }
        //
        // // Helper to invoke methods on instances
        // public static bool InvokeMethod(int id, string methodName, object[] parameters)
        // {
        //     if (!_instances.TryGetValue(id, out object? instance))
        //         return false;
        //     
        //     try
        //     {
        //         if (instance == null)
        //             return false;
        //         instance?.GetType().GetMethod(methodName)?.Invoke(instance, parameters);
        //         return true;
        //     }
        //     catch
        //     {
        //         return false;
        //     }
        // }
    }
}
