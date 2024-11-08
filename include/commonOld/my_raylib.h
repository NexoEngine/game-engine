/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen [WSL: Ubuntu]
** File description:
** my_raylib
*/

#define NOGDI             // Excludes GDI (Graphics Device Interface) definitions
#define NOUSER            // Excludes User API definitions
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOGDI               // Exclude GDI definitions
#define NOUSER              // Exclude User definitions

#ifndef NO_RAYLIB_COLORS
#define WHITE CLITERAL(Color){ 255, 255, 255, 255 }
#define BLACK CLITERAL(Color){ 0, 0, 0, 255 }

#include "raylib.h"
