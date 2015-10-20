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