\page renderer_2d 2D Renderer
# Renderer 2D API
## Tables of contents
- [Introduction](#introduction)
- [2D primitives](#2d-primitives)
    - [Quads](#quads)

## Introduction
Here are the default primitives handled by the 2D renderer :
> - Quads

The `beginScene` of the 2D renderer only needs an orthographic projection matrix. You can use our own implementation (see ADD LINK TO CAMERA DOC) or make up your own.  
You can then call the different draw functions
## 2D primitives
### Quads
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