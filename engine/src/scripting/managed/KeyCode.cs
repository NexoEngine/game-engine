//// KeyCode.cs ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        25/06/2025
//  Description: Key codes and mouse button definitions for input handling
//
///////////////////////////////////////////////////////////////////////////////

namespace Nexo
{
    /// <summary>
    /// Keyboard key codes matching the C++ definitions.
    /// 
    /// These values follow GLFW's key code system exactly to enable direct pass-through
    /// to the underlying windowing API without translation. GLFW uses a hybrid approach:
    /// - Printable ASCII characters (32-126) use their ASCII values directly
    /// - Special keys (arrows, function keys, etc.) use GLFW-specific codes starting from 256
    /// 
    /// This design choice allows the engine to pass key codes directly to GLFW's input
    /// functions without any conversion overhead. When adding new keys, use the corresponding
    /// GLFW key code value.
    /// 
    /// For a complete list of GLFW key codes, see:
    /// https://www.glfw.org/docs/latest/group__keys.html
    /// </summary>
    public enum KeyCode
    {
        Space = 32,
        
        // Number keys
        Key1 = 49,
        Key2 = 50,
        Key3 = 51,
        
        // Letter keys (GLFW uses ASCII values for printable characters)
        Q = 65,
        D = 68,
        E = 69,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        A = 81,
        S = 83,
        Z = 87,
        
        // Special keys
        Tab = 258,
        
        // Arrow keys
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        
        // Modifier keys
        Shift = 340,
        
        // Additional common keys (can be extended as needed)
        Escape = 256,
        Enter = 257,
        Backspace = 259,
        Delete = 261,
        
        // Function keys
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
    }
    
    /// <summary>
    /// Mouse button codes matching the C++ definitions
    /// </summary>
    public enum MouseButton
    {
        Left = 0,
        Right = 1,
        Middle = 2,
    }
}
