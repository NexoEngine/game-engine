//// Logger.cs ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/05/2025
//  Description: Source file for the logger class
//
///////////////////////////////////////////////////////////////////////////////

using System.Runtime.InteropServices;

namespace Nexo;

public enum LogLevel : UInt32
{
    Fatal,
    Error,
    Warn,
    Info,
    Debug,
    Dev,
    User
}

public static class Logger
{
    /// <summary>
    /// Logs a message with the specified log level.
    /// </summary>
    /// <param name="level">Specifies the log level (e.g., Fatal, Error, Warn, Info, Debug, Dev, User).</param>
    /// <param name="message">The message to be logged.</param>
    public static void Log(LogLevel level, String message) => NativeInterop.Log((UInt32)level, message);
    
}
