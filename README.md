<img src="https://raw.githubusercontent.com/NexoEngine/assets/refs/heads/main/logo_nexo.png" alt="NEXO Logo" width=250 height=250 align="right" />

# NEXO Engine

👋​ Welcome to the NEXO Engine repository! This project is a collaborative effort to create a revolutionnary game engine. Our goal is to provide a robust, user-friendly, and efficient platform for game development in desktop as well as in virtual reality! 🤩✨​🚀​

[![Build, test and Package](https://github.com/NexoEngine/game-engine/actions/workflows/build.yml/badge.svg)](https://github.com/NexoEngine/game-engine/actions/workflows/build.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=NexoEngine_game-engine&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=NexoEngine_game-engine)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=NexoEngine_game-engine&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=NexoEngine_game-engine)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=NexoEngine_game-engine&metric=coverage)](https://sonarcloud.io/summary/new_code?id=NexoEngine_game-engine)
[![Lines of Code](https://sonarcloud.io/api/project_badges/measure?project=NexoEngine_game-engine&metric=ncloc)](https://sonarcloud.io/summary/new_code?id=NexoEngine_game-engine)

## Table of Contents

- [NEXO Engine](#nexo-engine)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
  - [External Dependencies](#external-dependencies)
  - [Build the project](#build-the-project)
    - [Automatically with CMake Presets](#option1-using-cmake-presets-automatic-recommended)
    - [Manually with CMake](#option2-using-cmake-build-manual)
  - [Install NEXO Engine](#install-nexo-engine)
    - [Install via CMake](#install-via-cmake)
    - [Create an installer for Windows (NSIS)](#create-an-installer-for-windows-nsis)
    - [Create an installer for Linux (DEB)](#create-an-installer-for-linux-deb)
  - [Run the tests](#run-the-tests)
  - [The Team](#the-team)
  - [Join the NEXO community](#join-the-nexo-community)
  - [Acknowledgements](#acknowledgements)
  - [License](#license)
    - [How to extract the third-party licenses file](#how-to-extract-the-third-party-licenses-file)
    - [How to generate the COPYRIGHT file](#how-to-generate-the-copyright-file)

> [!NOTE]
> Find the whole documentation on our [website](https://nexoengine.github.io/game-engine/).

## Project Overview

📸​ Interface screenshot

![Editor interface](https://github.com/user-attachments/assets/cb19d00c-56bc-4cdd-9f61-069375fd7b08)

🎥 Quick videos of the game engine in action

https://github.com/user-attachments/assets/a91e61c3-b8a9-43d3-83c4-37fd02de2e7c

https://github.com/user-attachments/assets/281bec66-65cc-4467-aeb7-8f546dc3fb22

https://github.com/user-attachments/assets/ea92807b-da51-4fb6-bd1d-d3c9fadf0c13

<!-- Command used for video compression: ffmpeg -ss 00:00:02 -to 00:00:57 -i .\docs\game_engine\editor_interface_video.mkv -codec copy -an -vcodec libx264 -crf
 4 .\docs\game_engine\editor_interface_video.mp4 -->

> [!WARNING]
> This project is still in development and is not yet ready for production use. Some features may change.

## External Dependencies

To run this project, ensure you have the following:
- **CMake**: Necessary for building the project from source. (version 3.28 or higher) [install here](https://cmake.org/download/)
- **C++ Compiler**: We recommend using GCC or Clang for Linux and MacOS, and MSVC for Windows.
- **Git**: For cloning with submodules. [install here](https://github.com/git-guides/install-git)
- **.NET SDK 9.0**: Required for the C# scripting support. [install here](https://dotnet.microsoft.com/fr-fr/download/dotnet/9.0)

## Build the project

Cloning and building is a breeze:

### 1. Clone the repository with the submodules.
```bash
git clone --recurse-submodules https://github.com/NexoEngine/game-engine.git NEXO-Engine
cd NEXO-Engine
```

### 2. Run cmake to generate the build files.

#### Option1: Using `CMake presets` (automatic) [Recommended]

```bash
cmake --workflow --preset=build-debug
```

> [!NOTE]
> There are several presets available: `build-debug`, `build-release`, `build-coverage`, `minimal-build`, `test-debug`, and `test-coverage`.

#### Option2: Using `CMake Build` (manual)

```bash
cmake -B build
cmake --build build
```

### 3. Launch the engine!

#### For Linux and MacOS:
```bash
./build/nexoEditor
```

#### For Windows:
```bash
./build/nexoEditor.exe
```

> [!NOTE]
> For detailed instructions, visit our [Installation Guide](docs/install/README.md).

## Install NEXO Engine

### Install via CMake

First build the project as seen in the step [Build the project](#build-the-project).
Then run the following command:

```bash
cmake --install build --prefix /path/to/install
```

This will install the NEXO Engine in the directory `/path/to/install` with all the necessary files.

#### Create an installer for Windows (NSIS)

> [!WARNING]
> We assume here that you already ran the cmake command to generate the build files in the `build` directory.

To create an installer for Windows, you can use the [NSIS](https://nsis.sourceforge.io/Main_Page) installer.

1. First install [NSIS](https://nsis.sourceforge.io/Download) on your computer.

2. Execute the following commands:
```bash
cd build
cpack -G NSIS -C Debug
```

Now you can run the generated installer to install the NEXO Engine on your computer.
```bash
./nexoEditor.exe
```

#### Create an installer for Linux (DEB)

> [!WARNING]
> We assume here that you already ran the cmake command to generate the build files in the `build` directory.

```bash
cd build
cpack -G DEB
```

To install the generated package (on Ubuntu/Debian...), run the following command:

```bash
sudo dpkg -i NEXO-Engine-*-Linux.deb
```

## Run the tests

In this project tests use the [gtest](http://google.github.io/googletest/) library.

First build the tests:
```bash
cmake -B build -DNEXO_BUILD_TESTS=ON
cmake --build build
```

Then run the tests:
```bash
cd build
ctest -C Debug
```

## Troubleshooting

If you encounter any issues, please refer to our [Troubleshooting Guide](docs/troubleshooting/troubleshooting.md).

## The Team

NEXO Engine is brought to life by a dedicated team of fifth-year students from EPITECH Strasbourg:
- Guillaume HEIN [@Thyodas](https://www.linkedin.com/in/guillaume-hein/)
- Jean CARDONNE [@jcardonne](https://www.linkedin.com/in/jcardonne/)
- Marie GIACOMEL [@Sauterelle57](https://www.linkedin.com/in/marie-giacomel/)
- Mehdy MORVAN [@iMeaNz](https://www.linkedin.com/in/mehdy-morvan-a80959264/)
- Thomas PARENTEAU [@ThomasParenteau](https://www.linkedin.com/in/thomas-parenteau-0570b120a/)

## Join the NEXO community

Check our [web site](https://www.nexo-engine.com/) to keep you up to date about the latest news about **NEXO**.</br>
Join the [community](https://github.com/orgs/NexoEngine/discussions) to ask your questions or to talk with our developers.</br>
Visit our [blog](https://www.nexo-engine.com/blog) to find out more about how NEXO works.</br>

## Acknowledgements

This project is part of our curriculum and end of studies project, showcasing our collective skills in advanced software development with modern C++.

We thank Epitech for the opportunity to work on such an engaging project and for the support throughout our educational journey.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.<br/>
For more information about the copyright of the project, please refer to the [COPYRIGHT](COPYRIGHT) file.<br/>
You can also find the license of the third-party libraries used in the project in the [external/licenses](external/licenses) directory.

> [!TIP]
> For any license inquiry, please contact us at [nexo.engine@gmail.com](mailto:nexo.engine@gmail.com?subject=[NEXO%20Engine]%20License)

### How to extract the third-party licenses file

You can use the cmake install command:
```bash
cmake --install build --prefix /path/to/install --component generate-licenses
```

This will extract all licenses per third-party library in the `/path/to/install/external/licenses` directory.

> [!NOTE]
> These licenses are automatically extracted from vcpkg, there might be missing third-party libraries.

### How to generate the COPYRIGHT file

You can use the cmake install command:
```bash
cmake --install build --prefix /path/to/install --component generate-copyright
```

This will generate the COPYRIGHT file in the `/path/to/install` directory.

> [!WARNING]
> By default the COPYRIGHT file is generated with some `TODO:`, the generator cannot always determine exact licenses for some files. Please check each entry for errors.
