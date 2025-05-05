cmake_minimum_required(VERSION 3.20)

# Set project name
project(nexoManaged LANGUAGES NONE)

# Define variables for configuration
set(NEXO_MANAGED_OUTPUT_DIR ${CMAKE_BINARY_DIR})
set(NEXO_FRAMEWORK "net9.0")
set(NEXO_PLATFORM "x64")

set(SOURCES
    Lib.cs
    ObjectFactory.cs
    NativeInterop.cs
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

# Build step
add_custom_target(nexoManaged ALL
                  COMMAND ${DOTNET_EXECUTABLE} build Nexo.csproj
                  -c $<CONFIG>                        # Matches Debug/Release configuration
                  --output ${NEXO_MANAGED_OUTPUT_DIR} # Custom output directory
                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}      # Working directory for the build
                  COMMENT "Building .NET managed project (Nexo.csproj)..."
)

# Clean step
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    ${NEXO_MANAGED_OUTPUT_DIR}/Nexo.dll
    ${NEXO_MANAGED_OUTPUT_DIR}/Nexo.pdb
    ${NEXO_MANAGED_OUTPUT_DIR}/Nexo.runtimeconfig.json
    ${NEXO_MANAGED_OUTPUT_DIR}/Nexo.deps.json
)
