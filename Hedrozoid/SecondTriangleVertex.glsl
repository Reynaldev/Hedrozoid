#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vColor;

uniform vec3 offset;
uniform vec4 color;

void main()
{
	gl_Position = vec4(aPos + offset, 1.0);
	vColor = color;
}