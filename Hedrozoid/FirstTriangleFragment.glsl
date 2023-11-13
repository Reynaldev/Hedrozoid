#version 330 core

in vec3 vertColor;

out vec4 fragColor;

uniform vec3 rotColor;

void main()
{
	fragColor = vec4(vertColor + rotColor, 1.0);
}