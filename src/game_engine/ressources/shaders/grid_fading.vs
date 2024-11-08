#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matView;
uniform mat4 matProjection;

// Output vertex attributes (to fragment shader)
out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragView;
out mat4 fragProj;
out float near;
out float far;

vec3 gridPlane[6] = vec3[] (
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
);

vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection)
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{   
    fragView = matView;
    fragProj = matProjection;
    near = 0.01;
    far = 100.0;

    // Removing the projection from Z, its like projecting the point in 2D but very close to the camera and very far
    nearPoint = unprojectPoint(vertexPosition.x, vertexPosition.y, 0.0, matView, matProjection).xyz;
    farPoint = unprojectPoint(vertexPosition.x, vertexPosition.y, 1.0, matView, matProjection).xyz;  
    gl_Position = vec4(vertexPosition, 1.0);
}
