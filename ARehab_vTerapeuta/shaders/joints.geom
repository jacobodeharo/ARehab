/*
	This file is part of ARehab.

	ARehab is free software : you can redistribute it and / or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, version 3 of the License.

	ARehab is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Foobar.If not, see <http://www.gnu.org/licenses/>.

	Copyright 2015 Jacobo de Haro
	jacobodeharo@gmail.com
	@jacobodeharo

*/

#version 430 core

layout (points) in;
//layout (line_strip, max_vertices = 11) out;
layout (triangle_strip, max_vertices = 120) out;

uniform mat4 correction;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 Color;

//const float PI = 3.1415926f;
//float deg2rad(float deg)
//{
//	return ((PI * deg)/180.0f);
//}

//const float aspectRatio = 1920.0f / 1080.0f;
const float numPoints = 12.0f;
const float anglestep = 180.0f / numPoints;
const float r = 0.02f;


void build_circle(vec4 center)
{
	Color = gs_in[0].color; // gs_in[0] since there's only one input vertex	

	float angle = 0.0f;
	float rad = 0.0f;
	for (int i = 0; i <= numPoints; i++)
	{
		// circle center + Offset from center
		rad = radians(angle);
		gl_Position = center + (correction * vec4(cos(rad)*r, sin(rad)*r, 0.0f, 0.0f));
		EmitVertex();
		angle += anglestep;

		gl_Position = center;
		EmitVertex();
		angle += anglestep;
    }
    EndPrimitive();
}

// void build_house(vec4 position)
// {  
//     Color = gs_in[0].color; // gs_in[0] since there's only one input vertex
//     gl_Position = position + vec4(-0.01f, -0.01f, 0.0f, 0.0f);    // 1:bottom-left   
//     EmitVertex();    
//     gl_Position = position + vec4( 0.01f, -0.01f, 0.0f, 0.0f);    // 2:bottom-right
//     EmitVertex();
//     gl_Position = position + vec4(-0.01f,  0.01f, 0.0f, 0.0f);    // 3:top-left
//     EmitVertex();
//     gl_Position = position + vec4( 0.01f,  0.01f, 0.0f, 0.0f);    // 4:top-right
//     EmitVertex();
//     gl_Position = position + vec4( 0.0f,  0.02f, 0.0f, 0.0f);    // 5:top
    
//     Color = vec3(1.0f, 1.0f, 1.0f);
//     EmitVertex();
//     EndPrimitive();
// }

void main() {    
    //build_house(gl_in[0].gl_Position);
    build_circle(gl_in[0].gl_Position);
}