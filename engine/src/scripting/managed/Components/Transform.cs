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
    }

}
