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