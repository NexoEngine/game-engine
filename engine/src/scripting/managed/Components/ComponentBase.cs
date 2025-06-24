//// ComponentBase.cs /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        22/06/2025
//  Description: Interface for the user's components in NEXO's ECS framework
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Reflection;
using System.Runtime.InteropServices;
using Nexo;

namespace Nexo.Components;

public interface IComponentBase
{
    private static readonly List<IComponentBase> AllComponents = [];
    
    [UnmanagedCallersOnly]
    public static Int32 InitializeComponents()
    {
        try
        {
            // Find all types that derive from IComponentBase
            var componentTypes = AppDomain.CurrentDomain.GetAssemblies()
                .SelectMany(assembly => assembly.GetTypes())
                .Where(type => typeof(IComponentBase).IsAssignableFrom(type) &&
                               !type.IsAbstract &&
                               !type.IsInterface &&
                               type != typeof(IComponentBase))  // Exclude the interface itself
                .ToList();
            
            Logger.Log(LogLevel.Info, $"Found {componentTypes.Count} component types to register.");
            foreach (var componentType in componentTypes)
            {
                Logger.Log(LogLevel.Info, $"Component: {componentType.Name}");
            }
            
            // Register each component type
            foreach (var componentType in componentTypes)
            {
                if (NativeInterop.RegisterComponent(componentType) < 0)
                {
                    Logger.Log(LogLevel.Error, $"Failed to register component {componentType.Name}");
                    return 1;
                }
            }

            return 0;
        }
        catch (Exception ex)
        {
            Logger.Log(LogLevel.Fatal, $"Error initializing components: {ex.Message}");
            return 1;
        }
    }

}