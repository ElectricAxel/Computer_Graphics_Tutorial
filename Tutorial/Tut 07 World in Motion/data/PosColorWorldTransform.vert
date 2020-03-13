#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 interpColor;

uniform mat4 cameraToClipMatrix;
uniform mat4 modelToCameraMatrix;

void main()
{
	vec4 temp = modelToCameraMatrix * position;
	gl_Position = cameraToClipMatrix * temp;
	interpColor = color;
}
