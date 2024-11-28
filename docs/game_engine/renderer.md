\page renderer_readme Renderer API
# The Rendering API
The Nexo rendering api is fully abstracted in order to handle different types of graphics API.  
Here are the API graphics that are supported :
> - OpenGL (>=v4.0)

Our 3D and 2D renderers use a system of batch rendering, which means if possible only a single draw call is made for each scene.
The `beginScene` function is responsible for initializing the beginning of the draw call, setting the camera projection in the shader, binding the VAO, VBO and IBO and reseting the internal.  
You can then call the different `drawXXX` function to fill the different buffers.  
Finally, the `endScene` function will flush all the data and send it directly to the GPU.

[!IMPORTANT]
For now, we only support indexed drawing.

[!NOTE]
In the future, we will implement the material system allowing you to setup different shaders for each objects. But for now we are only using a default Uber shader with a maximum of 8 directionals lights and 8 points lights.  
Only the diffuse texture for models is handled for now.  
We also aim to implement different type of rendering in the near future (static, instanced...)

## Tables of contents
- [The 2D renderer](#the-2d-renderer)
    - [2D primitives](#2d-primitives)
      - [Quads](#quads)
- [The 3D renderer](#the-3d-renderer)
  - [3D primitives](#3d-primitives)
      - [Cubes](#cubes)
      - [Meshes](#meshes)

## The 2D renderer
Here are the default primitives handled by the 2D renderer :
> - Quads

The `beginScene` of the 2D renderer only needs an orthographic projection matrix. You can use our own implementation (see ADD LINK TO CAMERA DOC) or make up your own.  
You can then call the different draw functions
### 2D primitives
#### Quads
```c++
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color) const;
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color) const;
```
Renders a quad at the designated position with the specified color.  
[!TIP]
If rotation is not necessary, call one of the first two, it will avoid an extra useless matrix multiplication.  
[!NOTE]
For 2D rendering, Z is the main rotation axis.
```c++
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture) const;
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture) const;
```
Renders a quad with the designated texture, for more information on texture, see the Texture documentation.  
[!TIP]
If rotation is not necessary, call one of the first two, it will avoid an extra useless matrix multiplication  
[!NOTE]
For 2D rendering, Z is the main rotation axis
```c++
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture) const;
void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture) const;
void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture) const;
```
Renders a quad with the designated sub-texture, for more information on subtexture, see the SubTexture documentation.  
[!TIP]
If rotation is not necessary, call one of the first two, it will avoid an extra useless matrix multiplication.  
[!NOTE]
For 2D rendering, Z is the main rotation axis

## The 3D renderer
Here are the default primitives handled by the 3D renderer :
> - Cubes
> - Meshes

The `beginScene` of the 3D renderer needs a perspective projection matrix and the camera position (for specular lighthing handling). You can use our own implementation or make up your own.  
You can then call the different draw functions.
### 3D primitives
#### Cubes
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
#### Meshes
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