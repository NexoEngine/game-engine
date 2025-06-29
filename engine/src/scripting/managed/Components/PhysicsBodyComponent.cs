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
    /// Type de corps physique
    /// </summary>
    public enum PhysicsBodyType : UInt32
    {
        Static = 0,
        Dynamic = 1
    }

    /// <summary>
    /// Composant pour les corps physiques
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct PhysicsBodyComponent
    {
        /// <summary>
        /// ID du corps physique dans le moteur Jolt
        /// </summary>
        public UInt32 bodyID;

        /// <summary>
        /// Type du corps physique (statique ou dynamique)
        /// </summary>
        public PhysicsBodyType type;

        /// <summary>
        /// Crée un nouveau composant PhysicsBodyComponent
        /// </summary>
        /// <param name="bodyID">ID du corps physique</param>
        /// <param name="type">Type du corps physique</param>
        public PhysicsBodyComponent(UInt32 bodyID, PhysicsBodyType type)
        {
            this.bodyID = bodyID;
            this.type = type;
        }

        /// <summary>
        /// Crée un corps physique statique
        /// </summary>
        /// <param name="bodyID">ID du corps physique</param>
        /// <returns>Nouveau composant PhysicsBodyComponent statique</returns>
        public static PhysicsBodyComponent CreateStatic(UInt32 bodyID)
        {
            return new PhysicsBodyComponent(bodyID, PhysicsBodyType.Static);
        }

        /// <summary>
        /// Crée un corps physique dynamique
        /// </summary>
        /// <param name="bodyID">ID du corps physique</param>
        /// <returns>Nouveau composant PhysicsBodyComponent dynamique</returns>
        public static PhysicsBodyComponent CreateDynamic(UInt32 bodyID)
        {
            return new PhysicsBodyComponent(bodyID, PhysicsBodyType.Dynamic);
        }
    }
} 