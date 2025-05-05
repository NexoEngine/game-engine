# How to use scripting

## Dependencies

You first need to install .NET 9.0 SDK. You can download it from the official Microsoft website.

## Build the managed C# NEXO Library

1. Open a terminal and navigate to the `engine/src/scripting/managed` directory.
2. Run the following command to build the C# NEXO library:

```bash
dotnet build
```

This will create a `Nexo.dll` file in the `bin/Debug/net9.0` directory.
It is important that the `Nexo.dll` file is in this directory, as the C++ code will look for it there.

TODO: this should be done automatically when building the project, for now it's done manually.

## Build the C++ NEXO Library

The scripting C++ library is built using CMake when you build the project.

## Run the project

When running `nexoEditor` the script samples are automatically loaded and executed. You should see the output in the console.
