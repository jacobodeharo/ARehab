#version 430 core

layout (location = 0) in vec3 JointPosition;
layout (location = 1) in vec3 JointColor;

//out VS_OUT {
//	vec3 color;
//} vs_out;

out vec3 Color;

uniform mat4 model; //Model matrix
uniform mat4 view; //View matrix
uniform mat4 projection; //Projection matrix

void main()
{
	gl_Position = projection * view * model * vec4(JointPosition, 1.0f);
	//vs_out.color = JointColor;
	Color = JointColor;
}