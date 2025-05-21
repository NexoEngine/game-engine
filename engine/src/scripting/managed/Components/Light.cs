//// Light.cs /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        20/05/2025
//  Description: Source file for the Light component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using System.Runtime.InteropServices;

namespace Nexo.Components
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AmbientLightComponent
    {
        public Vector3 color;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct DirectionalLightComponent
    {
        public Vector3 direction;
        public Vector3 color;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PointLightComponent
    {
        public Vector3 color;
        public float linear;
        public float quadratic;
        public float maxDistance;
        public float constant;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SpotLightComponent
    {
        public Vector3 direction;
        public Vector3 color;
        public float cutOff;
        public float outerCutoff;
        public float linear;
        public float quadratic;
        public float maxDistance;
        public float constant;
    }
}