#### CMakeLists.txt ###########################################################
#
#  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
#  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
#  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
#  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
#  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
#
#  Author:      Guillaume HEIN
#  Date:        09/05/2025
#  Description: CMakeLists.txt for the NEXO managed library
#
###############################################################################

cmake_minimum_required(VERSION 3.20)

# Set project name
project(nexoManaged LANGUAGES NONE)

# Define variables for configuration
set(NEXO_FRAMEWORK "net9.0")

set(SOURCES
    Lib.cs
    ObjectFactory.cs
    NativeInterop.cs
    Logger.cs
    Components/Light.cs
    Components/Transform.cs
    Components/ComponentBase.cs
    Components/PhysicsBodyComponent.cs
    Components/PhysicsEnums.cs
    Components/Ui/Field.cs
    Components/Ui/FieldArray.cs
    Components/Ui/FieldType.cs
    Systems/SystemBase.cs
    Systems/WorldState.cs
    Scripts/CubeSystem.cs
)

# Locate the dotnet executable
find_program(DOTNET_EXECUTABLE dotnet HINTS ENV PATH)

if(NOT DOTNET_EXECUTABLE)
    message(FATAL_ERROR "The .NET SDK (dotnet CLI) is not installed or not in the PATH. Please install it from https://dotnet.microsoft.com/download.")
endif()

message(STATUS "Using dotnet executable: ${DOTNET_EXECUTABLE}")

# Generate Nexo.csproj from a template (Nexo.csproj.in)
configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/Nexo.csproj.in  # Input template
    ${CMAKE_CURRENT_LIST_DIR}/Nexo.csproj     # Output file
    @ONLY                                     # Only replace variables in @VAR@ format
)

if(CMAKE_GENERATOR_PLATFORM)
    set(ARCH_ARG --arch ${CMAKE_GENERATOR_PLATFORM})
else()
    set(ARCH_ARG "")
endif()


# Build step with OutputPath passed as parameter to support multi-config builds
add_custom_target(nexoManaged ALL
                  COMMAND ${DOTNET_EXECUTABLE} build Nexo.csproj
                  ${ARCH_ARG}
                  -c $<CONFIG>                        # Matches Debug/Release configuration
                  /p:OutputPath=${CMAKE_BINARY_DIR}/  # Pass OutputPath dynamically based on build directory
                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}      # Working directory for the build
                  COMMENT "Building .NET managed project (Nexo.csproj) for $<CONFIG> configuration..."
)

# Clean step
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    ${CMAKE_BINARY_DIR}/Nexo.dll
    ${CMAKE_BINARY_DIR}/Nexo.pdb
    ${CMAKE_BINARY_DIR}/Nexo.runtimeconfig.json
    ${CMAKE_BINARY_DIR}/Nexo.deps.json
    ${CMAKE_CURRENT_LIST_DIR}/obj
    ${CMAKE_CURRENT_LIST_DIR}/bin
)
