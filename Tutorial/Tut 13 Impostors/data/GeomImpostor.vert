#version 330

layout(location = 0) in vec3 cameraSpherePos;
layout(location = 1) in float sphereRadius;
layout(location = 2) in float materialIndex;

out VertexData
{
	vec3 cameraSpherePos;
	float sphereRadius;
	int materialIndex;
} outData;

void main()
{
	outData.cameraSpherePos = cameraSpherePos;
	outData.sphereRadius = sphereRadius;
	outData.materialIndex = int(materialIndex);
}
