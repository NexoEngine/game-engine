//// SystemBase.cs ///////////////////////////////////////////////////////////////
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
//  Date:        11/06/2025
//  Description: Interface for the user's systems in NEXO's ECS framework
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.InteropServices;

namespace Nexo.Systems
{

    public abstract class SystemBase
    {
        private static readonly List<SystemBase> AllSystems = [];
        private static Boolean _isInitialized = false;

        protected String Name => GetType().Name;
        protected Boolean IsActive { get; set; } = true;

        [UnmanagedCallersOnly]
        public static unsafe Int32 InitializeSystems(WorldState.NativeWorldState *nativeWorldState, UInt32 size)
        {
            if (_isInitialized) return 0;
            
            if (size != Marshal.SizeOf<WorldState.NativeWorldState>())
            {
                Logger.Log(LogLevel.Fatal, $"Struct size mismatch between C++ and C# for {nameof(WorldState.NativeWorldState)}, expected {Marshal.SizeOf<WorldState.NativeWorldState>()}, got {size}");
                return 1;
            }

            try
            {
                // Find all types that derive from SystemBase
                var systemTypes = AppDomain.CurrentDomain.GetAssemblies()
                    .SelectMany(assembly => assembly.GetTypes())
                    .Where(type => type.IsSubclassOf(typeof(SystemBase)) &&
                                   !type.IsAbstract)
                    .ToList();

                // Create instances of all system types
                foreach (var systemType in systemTypes)
                {
                    var instance = Activator.CreateInstance(systemType);
                    if (instance == null)
                    {
                        Logger.Log(LogLevel.Error, $"Failed to create instance of {systemType.Name}");
                        return 1;
                    }
                    AllSystems.Add((SystemBase)instance);
                }

                // Initialize all systems
                foreach (var system in AllSystems)
                {
                    system.OnInitialize(new WorldState(nativeWorldState));
                }

                _isInitialized = true;
                return 0;
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, ex.Message);
                return 1;
            }
        }

        [UnmanagedCallersOnly]
        public static unsafe Int32 UpdateSystems(WorldState.NativeWorldState *nativeWorldState, UInt32 size)
        {
            if (!_isInitialized)
            {
                Logger.Log(LogLevel.Error, "Systems not initialized. Call InitializeSystems first.");
                return 1;
            }
            if (size != Marshal.SizeOf<WorldState.NativeWorldState>())
            {
                Logger.Log(LogLevel.Fatal, $"Struct size mismatch between C++ and C# for {nameof(WorldState.NativeWorldState)}, expected {Marshal.SizeOf<WorldState.NativeWorldState>()}, got {size}");
                return 1;
            }
            
            try 
            {
                // Update all active systems
                foreach (var system in AllSystems.Where(s => s.IsActive))
                {
                    system.OnUpdate(new WorldState(nativeWorldState));
                }
            }
            catch (Exception ex)
            {
                Logger.Log(LogLevel.Error, ex.Message);
                return 1;
            }

            return 0;
        }

        [UnmanagedCallersOnly]
        public static unsafe Int32 ShutdownSystems(WorldState.NativeWorldState *nativeWorldState, UInt32 size)
        {
            if (size != Marshal.SizeOf<WorldState.NativeWorldState>())
            {
                Logger.Log(LogLevel.Fatal, $"Struct size mismatch between C++ and C# for {nameof(WorldState.NativeWorldState)}, expected {Marshal.SizeOf<WorldState.NativeWorldState>()}, got {size}");
                return 1;
            }
            
            foreach (var system in AllSystems)
            {
                system.OnShutdown(new WorldState(nativeWorldState));
            }

            AllSystems.Clear();
            _isInitialized = false;
            return 0;
        }

        protected virtual void OnInitialize(WorldState worldState)
        {
        }

        protected abstract void OnUpdate(WorldState worldState);

        protected virtual void OnShutdown(WorldState worldState)
        {
        }
        
    }

}