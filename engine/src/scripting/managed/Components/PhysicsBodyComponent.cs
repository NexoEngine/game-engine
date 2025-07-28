//// PhysicsBodyComponent.cs ///////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
        public MotionType type;

        /// <summary>
        /// Creates a new PhysicsBodyComponent
        /// </summary>
        /// <param name="bodyID">Physics body ID</param>
        /// <param name="type">Type of the physics body</param>
        public PhysicsBodyComponent(UInt32 bodyID, MotionType type)
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
            return new PhysicsBodyComponent(bodyID, MotionType.Static);
        }

        /// <summary>
        /// Creates a dynamic physics body component
        /// </summary>
        /// <param name="bodyID">Physics body ID</param>
        /// <returns>New dynamic PhysicsBodyComponent</returns>
        public static PhysicsBodyComponent CreateDynamic(UInt32 bodyID)
        {
            return new PhysicsBodyComponent(bodyID, MotionType.Dynamic);
        }
    }
} 