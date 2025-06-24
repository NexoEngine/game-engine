//// SystemBase.cs ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        11/06/2025
//  Description: Interface for the user's systems in NEXO's ECS framework
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo.Systems;

public unsafe class WorldState
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeWorldState
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct WorldTime {
            public Double DeltaTime; // Time since last update
            public Double TotalTime; // Total time since the start of the world
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct WorldStats
        {
            public UInt64 FrameCount; // Number of frames rendered
        }

        public WorldTime Time;
        public WorldStats Stats;
    }

    private readonly NativeWorldState* _nativePtr;
    
    internal WorldState(IntPtr nativePtr)
    {
        _nativePtr = (NativeWorldState*)nativePtr.ToPointer();
    }
    
    internal WorldState(NativeWorldState* nativePtr)
    {
        _nativePtr = nativePtr;
    }

    // Direct access to native structs (no copying)
    public ref NativeWorldState.WorldTime Time => ref _nativePtr->Time;
    public ref NativeWorldState.WorldStats Stats => ref _nativePtr->Stats;
}
