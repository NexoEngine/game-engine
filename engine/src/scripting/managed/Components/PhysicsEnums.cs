//// PhysicsEnums.cs /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        29/26/2025
//  Description: Enums for physics C#
//
///////////////////////////////////////////////////////////////////////////////

using System;

namespace Nexo.Components
{
    /// <summary>
    /// Available physics shape types
    /// </summary>
    public enum ShapeType : UInt32
    {
        Box = 0,
        Sphere = 1,
        Cylinder = 2,
        Tetrahedron = 3,
        Pyramid = 4
    }

    /// <summary>
    /// Motion types for physics bodies
    /// </summary>
    public enum MotionType : UInt32
    {
        Static = 0,
        Kinematic = 1,
        Dynamic = 2
    }
} 