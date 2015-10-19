#version 430 core

layout (location = 0) in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float dimH;

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec2 T = gl_FragCoord.xy;
	FragColor = texture(ourTexture, TexCoord); //color in RGBA space
} 