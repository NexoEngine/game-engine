//// FieldArray.cs ////////////////////////////////////////////////////////////
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
//  Description: Source file for the FieldArray struct in C#.
//
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Nexo.Components.Ui;

public unsafe class FieldArray : IDisposable
{
    private Field* _fields;
    private Boolean _disposed;
    
    public FieldArray(Int32 capacity)
    {
        Count = capacity;
        if (capacity == 0)
        {
            _fields = null;
            return;
        }

        _fields = (Field*)Marshal.AllocHGlobal(capacity * sizeof(Field));

        for (var i = 0u; i < capacity; i++)
        {
            _fields[i] = default;
        }
    }

    public FieldArray(Field[] fields)
    {
        if (fields == null)
            throw new ArgumentNullException(nameof(fields));

        Count = fields.Length;

        if (Count == 0)
        {
            _fields = null;
            return;
        }

        _fields = (Field*)Marshal.AllocHGlobal(Count * sizeof(Field));

        for (int i = 0; i < Count; i++)
        {
            _fields[i] = fields[i];
        }
    }
    
    public Field* GetPointer() => _fields;
    public Int32 Count { get; }

    public Field this[Int32 index]
    {
        get
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(FieldArray));
            if (index < 0 || index >= Count)
                throw new IndexOutOfRangeException();
            return _fields[index];
        }
        set
        {
            if (_disposed)
                throw new ObjectDisposedException(nameof(FieldArray));
            if (index < 0 || index >= Count)
                throw new IndexOutOfRangeException();
            _fields[index] = value;
        }
    }

    protected virtual void Dispose(Boolean disposing)
    {
        if (_disposed || _fields == null)
            return;

        if (disposing)
        {
            for (int i = 0; i < Count; i++)
            {
                _fields[i].Dispose();
            }
        }

        Marshal.FreeHGlobal((IntPtr)_fields);
        _fields = null;
        _disposed = true;
    }
    
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }

    ~FieldArray()
    {
        Dispose(false);
    }
    
    public static FieldArray CreateFieldArrayFromType(Type type, Boolean flatten = true)
    {
        if (type == null)
            throw new ArgumentNullException(nameof(type));
        var flattenedFields = flatten ? GetFlattenedFields(type) : GetDirectFields(type);
        var fieldArray = new FieldArray(flattenedFields.Count);

        try
        {
            for (var i = 0; i < flattenedFields.Count; i++)
            {
                fieldArray[i] = Field.CreateFieldFromFieldInfo(flattenedFields[i].FieldInfo);
            }
        }
        catch (Exception ex)
        {
            fieldArray.Dispose();
            throw new InvalidOperationException($"Failed to create FieldArray from type {type.Name}: {ex.Message}", ex);
        }
        
        return fieldArray;
    }
    
    private static List<(FieldInfo FieldInfo, String Name)> GetDirectFields(Type type)
    {
        if (type == null)
            throw new ArgumentNullException(nameof(type));
        return type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
            .Where(f => !f.IsStatic && !f.IsLiteral)
            .Select(f => (f, f.Name))
            .ToList();
    }
    
    private static List<(FieldInfo FieldInfo, String Name)> GetFlattenedFields(Type type, String prefix = "",
        HashSet<Type>? visitedTypes = null)
    {
        if (type == null)
            throw new ArgumentNullException(nameof(type));

        visitedTypes ??= new HashSet<Type>();

        if (!visitedTypes.Add(type))
        {
            // Prevent infinite recursion for circular references
            return new List<(FieldInfo, String)>();
        }

        var result = new List<(FieldInfo, String)>();
        
        var fields = type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
            .Where(f => !f.IsStatic && !f.IsLiteral);
        
        foreach (var field in fields)
        {
            String fieldName = String.IsNullOrEmpty(prefix) ? field.Name : $"{prefix}.{field.Name}";
            
            if (Field.TypeMap.ContainsKey(field.FieldType))
            {
                // If the field type is a known primitive or struct, add it directly
                result.Add((field, fieldName));
                continue;
            }
            
            // If it's a struct (but not a primitive or string), flatten it
            if (field.FieldType.IsValueType && 
                !field.FieldType.IsPrimitive && 
                !field.FieldType.IsEnum &&
                field.FieldType != typeof(IntPtr) &&
                field.FieldType != typeof(UIntPtr) &&
                !field.FieldType.IsPointer)
            {
                // Recursively flatten nested struct
                result.AddRange(GetFlattenedFields(field.FieldType, fieldName, visitedTypes));
            }
            else
            {
                // It's unknown, let Field handle it
                result.Add((field, fieldName));
            }
        }
        
        return result;
    }
}
