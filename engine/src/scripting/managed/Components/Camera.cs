//// Camera.cs ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        01/06/2025
//  Description: Source file for the Camera component in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using System.Runtime.InteropServices;

namespace Nexo.Components
{
    public enum CameraType
    {
        Perspective = 0,
        Orthographic = 1
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct CameraComponent
    {
        public uint width;
        public uint height;
        [MarshalAs(UnmanagedType.I1)] public bool viewportLocked;
        public float fov;
        public float nearPlane;
        public float farPlane;
        public CameraType type;
        public Vector4 clearColor;
        [MarshalAs(UnmanagedType.I1)] public bool active;
        [MarshalAs(UnmanagedType.I1)] public bool render;
        [MarshalAs(UnmanagedType.I1)] public bool main;
        [MarshalAs(UnmanagedType.I1)] public bool resizing;
        // Note: m_renderTarget is excluded (pointer to shared_ptr)
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PerspectiveCameraController
    {
        public Vector2 lastMousePosition;
        public float mouseSensitivity;
        public float translationSpeed;
        [MarshalAs(UnmanagedType.I1)] public bool wasMouseReleased;
        [MarshalAs(UnmanagedType.I1)] public bool wasActiveLastFrame;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct PerspectiveCameraTarget
    {
        public Vector2 lastMousePosition;
        public float mouseSensitivity;
        public float distance;
        public uint targetEntity;
    }
}
