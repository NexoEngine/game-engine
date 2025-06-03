//// Uuid.cs //////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        03/06/2025
//  Description: Source file for the Uuid component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo.Components
{
    [StructLayout(LayoutKind.Sequential)]
    public struct UuidComponent
    {
        public IntPtr uuidPtr;

        public string Uuid => Marshal.PtrToStringAnsi(uuidPtr) ?? string.Empty;
    }
}