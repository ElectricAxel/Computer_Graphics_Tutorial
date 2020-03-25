#version 330
#extension GL_EXT_gpu_shader4 : enable

layout(std140) uniform;
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

uniform Projection
{
	mat4 cameraToClipMatrix;
};

in VertexData
{
	vec3 cameraSpherePos;
	float sphereRadius;
	int materialIndex;
} vert[];

out FragData
{
	flat vec3 cameraSpherePos;
	flat float sphereRadius;
	smooth vec2 mapping;
	flat int materialIndex;
};

const float g_boxCorrection = 1.5;

void main()
{
	vec4 cameraCornerPos;
	//Bottom-left
	mapping = vec2(-1.0, -1.0) * g_boxCorrection;
	cameraSpherePos = vec3(vert[0].cameraSpherePos);
	sphereRadius = vert[0].sphereRadius;
	cameraCornerPos = vec4(vert[0].cameraSpherePos, 1.0);
	cameraCornerPos.xy += vec2(-vert[0].sphereRadius, -vert[0].sphereRadius) * g_boxCorrection;
	gl_Position = cameraToClipMatrix * cameraCornerPos;
	materialIndex = vert[0].materialIndex;
	EmitVertex();
	
	//Top-left
	mapping = vec2(-1.0, 1.0) * g_boxCorrection;
	cameraSpherePos = vec3(vert[0].cameraSpherePos);
	sphereRadius = vert[0].sphereRadius;
	cameraCornerPos = vec4(vert[0].cameraSpherePos, 1.0);
	cameraCornerPos.xy += vec2(-vert[0].sphereRadius, vert[0].sphereRadius) * g_boxCorrection;
	gl_Position = cameraToClipMatrix * cameraCornerPos;
	materialIndex = vert[0].materialIndex;
	EmitVertex();
	
	//Bottom-right
	mapping = vec2(1.0, -1.0) * g_boxCorrection;
	cameraSpherePos = vec3(vert[0].cameraSpherePos);
	sphereRadius = vert[0].sphereRadius;
	cameraCornerPos = vec4(vert[0].cameraSpherePos, 1.0);
	cameraCornerPos.xy += vec2(vert[0].sphereRadius, -vert[0].sphereRadius) * g_boxCorrection;
	gl_Position = cameraToClipMatrix * cameraCornerPos;
	materialIndex = vert[0].materialIndex;
	EmitVertex();
	
	//Top-right
	mapping = vec2(1.0, 1.0) * g_boxCorrection;
	cameraSpherePos = vec3(vert[0].cameraSpherePos);
	sphereRadius = vert[0].sphereRadius;
	cameraCornerPos = vec4(vert[0].cameraSpherePos, 1.0);
	cameraCornerPos.xy += vec2(vert[0].sphereRadius, vert[0].sphereRadius) * g_boxCorrection;
	gl_Position = cameraToClipMatrix * cameraCornerPos;
	materialIndex = vert[0].materialIndex;
	EmitVertex();
}
