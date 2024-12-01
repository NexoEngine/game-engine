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

## Subpages
- \subpage renderer_2d
- \subpage renderer_3d