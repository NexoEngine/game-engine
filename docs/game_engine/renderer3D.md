\page renderer_3d 3D Renderer
# Renderer 3D API
## Tables of contents
- [Introduction](#introduction)
- [3D primitives](#3d-primitives)
    - [Cubes](#cubes)
    - [Meshes](#meshes)

## Introduction
Here are the default primitives handled by the 3D renderer :
> - Cubes
> - Meshes

The `beginScene` of the 3D renderer needs a perspective projection matrix and the camera position (for specular lighthing handling). You can use our own implementation or make up your own.  
You can then call the different draw functions.
## 3D primitives
### Cubes
```c++
void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) const;
```
Renders a cube at the designated position with the designated color and size.  
[!NOTE]
Rotation is not yet handled for cubes
```c++
void drawCube(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture) const;
```
Renders a cube at the designated position with the designated size and texture, for more information on textures, see the Texture documentation.  
[!NOTE]
Rotation is not yet handled for cubes
### Meshes
```c++
void drawMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::shared_ptr<Texture2D>& texture) const;
```
Renders a mesh with the designated vertices, indices and texture.  
[!NOTE]
The texture parameter correspond to the diffuse texture, at some point we will implement the material system.  
[!IMPORTANT]
For now, you have to process yourself the transformation of the vertices before calling the `drawMesh` function.

Here is the vertex struct :
```c++
struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texIndex; // Used internally, useless to set
    glm::vec3 normal;
};
```
[!NOTE]
This struct will probably change to handle more type of textures.