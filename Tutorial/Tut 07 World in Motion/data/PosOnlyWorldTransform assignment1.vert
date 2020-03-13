#version 330

layout(location = 0) in vec4 position;

uniform mat4 worldToClipMatrix;
uniform mat4 modelToWorldMatrix;

void main()
{
	vec4 temp = modelToWorldMatrix * position;
	gl_Position = worldToClipMatrix * temp;
}
