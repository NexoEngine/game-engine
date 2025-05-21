//// NativeComponents.cs //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        20/05/2025
//  Description: Enum for NativeComponents, used in GetComponent
//
///////////////////////////////////////////////////////////////////////////////

namespace Nexo;

/// <summary>
/// Enum representing the components available in C++ accessible via interop.
/// Must exactly match the NativeComponents enum defined in ManagedTypedef.hpp on the C++ side.
/// </summary>

public enum NativeComponents : uint
{
    Transform = 0,
    AmbientLight = 1,
    DirectionalLight = 2,
    PointLight = 3,
    SpotLight = 4,
    RenderComponent = 5,
    SceneTag = 6,
    CameraComponent = 7,
    UuidComponent = 8,
    PerspectiveCameraController = 9,
    PerspectiveCameraTarget = 10,
    
}