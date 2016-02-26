#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

// out vec3 
out vec3 ourPosition;

uniform float xOffset;

void main()
{
	// Add the xOffset to the x position of the vertex position
	gl_Position = vec4(position.x + xOffset, position.y, position.z, 1.0f);
	ourColor = color;

	ourPosition = position;
}