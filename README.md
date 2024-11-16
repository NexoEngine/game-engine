<img src="https://raw.githubusercontent.com/NexoEngine/assets/refs/heads/main/logo_nexo.png" alt="NEXO Logo" width=250 height=250 align="right" />

# NEXO Engine

Welcome to the NEXO Engine repository! This project is a collaborative effort to create a revolutionnary game engine. Our goal is to provide a robust, user-friendly, and efficient platform for game development in desktop as well as in virtual reality!

## Table of Contents

- [NEXO Engine](#nexo-engine)
  - [Table of Contents](#table-of-contents)
  - [Project Overview](#project-overview)
    - [Game Engine](#game-engine)
    - [Client](#client)
  - [External Dependencies](#external-dependencies)
  - [Installation](#installation)
  - [The Team](#the-team)
  - [Acknowledgements](#acknowledgements)

> [!NOTE]  
> Find the whole documentation on our [website](https://nexoengine.github.io/game-engine/).

## Project Overview

### Game Engine

Quick video of the game engine in action with Jolt Physics

![Editor interface video](docs/game_engine/editor_interface_video.mp4)


Interface screenshot

![Editor interface](docs/game_engine/editor_interface.png)

### Client

https://github.com/Thyodas/rtype/assets/53176398/2d6a7a5f-9ad2-435a-88c5-36cfdcd72b80

## External Dependencies

To run this project, ensure you have the following:
- **CMake**: Necessary for building the project from source.
- **C++ Compiler**: We recommend using GCC or Clang for Linux and MacOS, and MSVC for Windows.
- **X11**: Required for Linux and MacOS.

## Installation

Cloning and building is a breeze:
1. Clone the repository.
2. Run cmake to generate the build files.
```bash
cmake -B build
cmake --build build
```
3. Launch the engine!

For Linux and MacOS:
```bash
./build/nexoEditor
```

For Windows:
```bash
./build/nexoEditor.exe
```

> [!NOTE]
> For detailed instructions, visit our [Installation Guide](docs/install/README.md).

## The Team

NEXO Engine is brought to life by a dedicated team of fourth-year students from EPITECH Strasbourg:
- Guillaume HEIN (@Thyodas)
- Jean CARDONNE (@jcardonne)
- Marie GIACOMEL (@Sauterelle57)
- Mehdy MORVAN (@iMeaNz)
- Thomas PARENTEAU (@ThomasParenteau)

## Acknowledgements

This project is part of our curriculum and end of studies project, showcasing our collective skills in advanced software development with modern C++.

We thank Epitech for the opportunity to work on such an engaging project and for the support throughout our educational journey.
