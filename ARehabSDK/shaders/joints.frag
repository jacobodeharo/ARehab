#version 430 core

in vec3 Color;


layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(Color, 0.8);
} 