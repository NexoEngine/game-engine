//// Input.cs //////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        25/06/2025
//  Description: Static input class for easy keyboard and mouse input access
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;

namespace Nexo
{
    /// <summary>
    /// Provides static methods for querying keyboard and mouse input states.
    /// Similar to Unity's Input class for familiar API usage.
    /// </summary>
    public static class Input
    {
        /// <summary>
        /// Checks if the specified key is currently pressed.
        /// </summary>
        /// <param name="key">The key to check</param>
        /// <returns>true if the key is pressed, false otherwise</returns>
        public static bool IsKeyPressed(KeyCode key)
        {
            return NativeInterop.IsKeyPressed((Int32)key);
        }
        
        /// <summary>
        /// Checks if the specified key is currently released.
        /// </summary>
        /// <param name="key">The key to check</param>
        /// <returns>true if the key is released, false otherwise</returns>
        public static bool IsKeyReleased(KeyCode key)
        {
            return NativeInterop.IsKeyReleased((Int32)key);
        }
        
        /// <summary>
        /// Checks if the specified mouse button is currently pressed.
        /// </summary>
        /// <param name="button">The mouse button to check</param>
        /// <returns>true if the mouse button is pressed, false otherwise</returns>
        public static bool IsMouseDown(MouseButton button)
        {
            return NativeInterop.IsMouseDown((Int32)button);
        }
        
        /// <summary>
        /// Checks if the specified mouse button is currently released.
        /// </summary>
        /// <param name="button">The mouse button to check</param>
        /// <returns>true if the mouse button is released, false otherwise</returns>
        public static bool IsMouseReleased(MouseButton button)
        {
            return NativeInterop.IsMouseReleased((Int32)button);
        }
        
        /// <summary>
        /// Gets the current mouse position in screen coordinates.
        /// </summary>
        /// <returns>The current mouse position as a Vector2</returns>
        public static Vector2 GetMousePosition()
        {
            return NativeInterop.GetMousePosition();
        }
        
        /// <summary>
        /// Checks if any key is currently pressed.
        /// Useful for "Press any key to continue" scenarios.
        /// </summary>
        /// <returns>true if any key is pressed</returns>
        public static bool IsAnyKeyPressed()
        {
            // Check common keys
            foreach (KeyCode key in Enum.GetValues(typeof(KeyCode)))
            {
                if (IsKeyPressed(key))
                    return true;
            }
            return false;
        }
        
        /// <summary>
        /// Checks if any mouse button is currently pressed.
        /// </summary>
        /// <returns>true if any mouse button is pressed</returns>
        public static bool IsAnyMouseButtonPressed()
        {
            return IsMouseDown(MouseButton.Left) || 
                   IsMouseDown(MouseButton.Right) || 
                   IsMouseDown(MouseButton.Middle);
        }
    }
}