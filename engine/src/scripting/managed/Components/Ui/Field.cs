//// Field.cs /////////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Guillaume HEIN
//  Date:        24/06/2025
//  Description: Source file for the Field struct in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System.Numerics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Nexo.Components.Ui;

[StructLayout(LayoutKind.Sequential)]
public unsafe struct Field
{
    // Use IntPtr instead of char* for better marshaling
    public IntPtr Name;
    public FieldType Type;
    public UInt64 Size; // For blank fields or additional size information
    public UInt64 Offset;
    
    public static readonly Dictionary<Type, FieldType> TypeMap = new()
    {
        { typeof(Boolean), FieldType.Bool },
        { typeof(SByte), FieldType.Int8 },
        { typeof(Int16), FieldType.Int16 },
        { typeof(Int32), FieldType.Int32 },
        { typeof(Int64), FieldType.Int64 },
        { typeof(Byte), FieldType.UInt8 },
        { typeof(UInt16), FieldType.UInt16 },
        { typeof(UInt32), FieldType.UInt32 },
        { typeof(UInt64), FieldType.UInt64 },
        { typeof(Single), FieldType.Float },
        { typeof(Double), FieldType.Double },
        { typeof(Vector3), FieldType.Vector3 },
        { typeof(Vector4), FieldType.Vector4 }
    };
    
    public static Field CreateSectionField(String sectionName)
    {
        var field = new Field
        {
            Name = Marshal.StringToHGlobalAnsi(sectionName),
            Type = FieldType.Section,
            Size = 0
        };
        return field;
    }
    
    public static Field CreateFieldFromFieldInfo(FieldInfo fieldInfo)
    {
        var fieldType = TypeMap.GetValueOrDefault(fieldInfo.FieldType, FieldType.Blank);
        var fieldName = fieldInfo.Name;
        var declaringType = fieldInfo.DeclaringType;
        if (declaringType == null)
            throw new InvalidOperationException($"Field '{fieldName}' has no declaring type");
        var offset = Marshal.OffsetOf(declaringType, fieldName);
        
        var size = Marshal.SizeOf(fieldInfo.FieldType);
        Logger.Log(LogLevel.Info, $"Creating field: {fieldName}, Type: {fieldType}, Size: {Marshal.SizeOf(fieldInfo.FieldType)}");
        
        return new Field
        {
            Name = Marshal.StringToHGlobalAnsi(fieldName),
            Type = fieldType,
            Size = Convert.ToUInt64(size),
            Offset = Convert.ToUInt64(offset)
        };
    }
    
    // Property to safely get the name as a string
    public String NameString => Name != IntPtr.Zero ? Marshal.PtrToStringAnsi(Name) ?? string.Empty : string.Empty;
    
    // Clean up allocated memory (call this when done with the struct)
    public void Dispose()
    {
        if (Name != IntPtr.Zero)
        {
            Marshal.FreeHGlobal(Name);
            Name = IntPtr.Zero;
        }
    }
    
    // Override ToString for debugging
    public override string ToString()
    {
        return $"Field {{ Name = \"{NameString}\", Type = {Type}, Size = {Size} }}";
    }
}
