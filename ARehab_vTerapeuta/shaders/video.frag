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

layout (location = 0) in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float dimH;

layout (location = 0) out vec4 FragColor;

const float y_const = 0.0625f;
const float vu_const = 0.5f;

// YUV -> RGB
void YUVToRGBA(in vec3 yuv, out vec4 rgbcolor)
{
  rgbcolor.r = (1.164f * (yuv.r - y_const)) + (2.018f * (yuv.b - vu_const));
  rgbcolor.g = (1.164f * (yuv.r - y_const)) - (0.813f * (yuv.g - vu_const)) - (0.391f * (yuv.b - vu_const));
  rgbcolor.b = (1.164f * (yuv.r - y_const)) + (1.596f * (yuv.g - vu_const));
  rgbcolor.a = 1.0f;
}

void main()
{	
	vec2 T = gl_FragCoord.xy;
	vec4 yuy2 = texture(ourTexture, TexCoord); //color in YUY2 space
	vec4 color;
	YUVToRGBA(yuy2.rga, color);
	FragColor = color;
} 