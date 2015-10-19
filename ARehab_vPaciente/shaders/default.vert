#version 430 core

layout (location = 0) in vec3 VertexPosition;
//layout (location = 1) in vec3 VertexColor;
layout (location = 1) in vec2 TextureCoord;

uniform mat4 model; //Model matrix
uniform mat4 view; //View matrix
uniform mat4 projection; //Projection matrix

layout (location = 0) out vec2 TexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);    
    TexCoord = TextureCoord;
}