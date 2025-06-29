//// PhysicsBodyComponent.cs ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        06/01/2025
//  Description: C# component for PhysicsBodyComponent
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Runtime.InteropServices;

namespace Nexo.Components
{
    /// <summary>
    /// Type of physics body
    /// </summary>
    public enum PhysicsBodyType : UInt32
    {
        Static = 0,
        Dynamic = 1
    }

    /// <summary>
    /// Component for physics bodies
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct PhysicsBodyComponent
    {
        /// <summary>
        /// ID of the physics body in the Jolt engine
        /// </summary>
        public UInt32 bodyID;

        /// <summary>
        /// Type of the physics body (static or dynamic)
        /// </summary>
        public PhysicsBodyType type;

        /// <summary>
        /// Creates a new PhysicsBodyComponent
        /// </summary>
        /// <param name="bodyID">Physics body ID</param>
        /// <param name="type">Type of the physics body</param>
        public PhysicsBodyComponent(UInt32 bodyID, PhysicsBodyType type)
        {
            this.bodyID = bodyID;
            this.type = type;
        }

        /// <summary>
        /// Creates a static physics body component
        /// </summary>
        /// <param name="bodyID">Physics body ID</param>
        /// <returns>New static PhysicsBodyComponent</returns>
        public static PhysicsBodyComponent CreateStatic(UInt32 bodyID)
        {
            return new PhysicsBodyComponent(bodyID, PhysicsBodyType.Static);
        }

        /// <summary>
        /// Creates a dynamic physics body component
        /// </summary>
        /// <param name="bodyID">Physics body ID</param>
        /// <returns>New dynamic PhysicsBodyComponent</returns>
        public static PhysicsBodyComponent CreateDynamic(UInt32 bodyID)
        {
            return new PhysicsBodyComponent(bodyID, PhysicsBodyType.Dynamic);
        }
    }
} 