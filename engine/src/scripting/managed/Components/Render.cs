//// Render.cs ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        01/06/2025
//  Description: Source file for the Render component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo.Components
{
    public enum RenderType
    {
        Render2D = 0,
        Render3D = 1
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct RenderComponent
    {
        [MarshalAs(UnmanagedType.I1)] public bool isRendered;
        public RenderType type;
        // renderable is ignored (unmanaged/shared_ptr)
    }
}
