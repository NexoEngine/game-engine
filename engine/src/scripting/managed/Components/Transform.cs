//// Transform.cs /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        03/05/2025
//  Description: Source file for the Transform component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using System.Runtime.InteropServices;

namespace Nexo.Components
{

    [StructLayout(LayoutKind.Sequential)]
    public struct Transform
    {
        public Vector3 pos;
        public Vector3 size;
        public Quaternion quat;
        
        public Matrix4x4 worldMatrix;
        public Matrix4x4 localMatrix;

        public Vector3 localCenter;

        // Note: children is a C++ vector class, it cannot be directly represented in C#.
        // TODO: Find a way to handle vectors and lists between C++ and C#.
    }

}
