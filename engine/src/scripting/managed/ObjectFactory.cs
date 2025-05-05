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
    public static class ObjectFactory
    {
        private static Dictionary<int, object> _instances = new Dictionary<int, object>();
        private static int _nextId = 1;
        
        // Generic factory method using Activator
        public static int CreateInstance(string typeName)
        {
            try 
            {
                Type? type = Type.GetType(typeName);
                if (type == null)
                    return -1;
                    
                object? instance = Activator.CreateInstance(type);
                if (instance == null)
                    return -1;
                int id = _nextId++;
                _instances[id] = instance;
                return id;
            }
            catch
            {
                return -1;
            }
        }
        
        // Create with parameters
        public static int CreateInstanceWithParams(string typeName, object[] parameters)
        {
            try 
            {
                Type? type = Type.GetType(typeName);
                if (type == null)
                    return -1;
                    
                object? instance = Activator.CreateInstance(type, parameters);
                if (instance == null)
                    return -1;
                int id = _nextId++;
                _instances[id] = instance;
                return id;
            }
            catch
            {
                return -1;
            }
        }
        
        // Method to release the instance
        public static bool ReleaseInstance(int id)
        {
            return _instances.Remove(id);
        }
        
        // Helper to invoke methods on instances
        public static bool InvokeMethod(int id, string methodName, object[] parameters)
        {
            if (!_instances.TryGetValue(id, out object? instance))
                return false;
            
            try
            {
                if (instance == null)
                    return false;
                instance?.GetType().GetMethod(methodName)?.Invoke(instance, parameters);
                return true;
            }
            catch
            {
                return false;
            }
        }
    }
}
