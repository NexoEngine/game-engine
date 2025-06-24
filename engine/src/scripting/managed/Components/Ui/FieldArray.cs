//// FieldArray.cs ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
    private readonly int _count;
    private bool _disposed;
    
    public FieldArray(int capacity)
    {
        _count = capacity;
        _fields = (Field*)Marshal.AllocHGlobal(capacity * sizeof(Field));
        
        for (int i = 0; i < capacity; i++)
        {
            _fields[i] = default;
        }
    }
    
    public FieldArray(Field[] fields)
    {
        _count = fields.Length;
        _fields = (Field*)Marshal.AllocHGlobal(_count * sizeof(Field));
        
        for (int i = 0; i < _count; i++)
        {
            _fields[i] = fields[i];
        }
    }
    
    public Field* GetPointer() => _fields;
    public int Count => _count;
    
    public Field this[int index]
    {
        get
        {
            if (index < 0 || index >= _count)
                throw new IndexOutOfRangeException();
            return _fields[index];
        }
        set
        {
            if (index < 0 || index >= _count)
                throw new IndexOutOfRangeException();
            _fields[index] = value;
        }
    }
    
    public void Dispose()
    {
        if (!_disposed && _fields != null)
        {
            for (int i = 0; i < _count; i++)
            {
                _fields[i].Dispose();
            }
            
            Marshal.FreeHGlobal((IntPtr)_fields);
            _fields = null;
            _disposed = true;
        }
    }
    
    public static FieldArray CreateFieldArrayFromType(Type type, Boolean flatten = true)
    {
        var flattenedFields = flatten ? GetFlattenedFields(type) : GetDirectFields(type);
        var fieldArray = new FieldArray(flattenedFields.Count);
        
        for (int i = 0; i < flattenedFields.Count; i++)
        {
            fieldArray[i] = Field.CreateFieldFromFieldInfo(type, flattenedFields[i].FieldInfo);
        }
        
        return fieldArray;
    }
    
    private static List<(FieldInfo FieldInfo, String Name)> GetDirectFields(Type type)
    {
        return type.GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance)
            .Where(f => !f.IsStatic && !f.IsLiteral)
            .Select(f => (f, f.Name))
            .ToList();
    }
    
    private static List<(FieldInfo FieldInfo, String Name)> GetFlattenedFields(Type type, String prefix = "")
    {
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
                result.AddRange(GetFlattenedFields(field.FieldType, fieldName));
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
