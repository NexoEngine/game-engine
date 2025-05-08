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
    public static void Log(LogLevel level, string message) => NativeInterop.NxLog((uint)level, message);
    
}
