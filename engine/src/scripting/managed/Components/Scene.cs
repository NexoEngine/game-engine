//// Scene.cs /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        03/06/2025
//  Description: Source file for the SceneTag component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo.Components
{
    [StructLayout(LayoutKind.Sequential)]
    public struct SceneTag
    {
        public uint id;

        [MarshalAs(UnmanagedType.I1)]
        public bool isActive;

        [MarshalAs(UnmanagedType.I1)]
        public bool isRendered;
    }
}