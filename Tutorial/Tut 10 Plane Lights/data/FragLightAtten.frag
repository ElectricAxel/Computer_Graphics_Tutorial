#version 330

in vec4 diffuseColor;
in vec3 vertexNormal;

out vec4 outputColor;

uniform vec3 modelSpaceLightPos;

uniform vec4 lightIntensity;
uniform vec4 ambientIntensity;

uniform vec3 cameraSpaceLightPos;

uniform float maxDistance;

uniform UnProjection
{
	mat4 clipToCameraMatrix;
	ivec2 windowSize;
};

vec3 CalcCameraSpacePosition()
{
	vec4 ndcPos;
	ndcPos.xy = ((gl_FragCoord.xy / windowSize.xy) * 2.0) - 1.0;
	ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
		(gl_DepthRange.far - gl_DepthRange.near);
	ndcPos.w = 1.0;
	
	vec4 clipPos = ndcPos / gl_FragCoord.w;
	
	return vec3(clipToCameraMatrix * clipPos);
}

vec4 ApplyLightIntensity(in vec3 cameraSpacePosition, out vec3 lightDirection)
{
	vec3 lightDifference =  cameraSpaceLightPos - cameraSpacePosition;
	float lightDistanceSqr = dot(lightDifference, lightDifference);
	lightDirection = lightDifference * inversesqrt(lightDistanceSqr);
	
	return lightIntensity * clamp((maxDistance - sqrt(lightDistanceSqr)) / maxDistance, 0, 1);
}

void main()
{
	vec3 cameraSpacePosition = CalcCameraSpacePosition();

	vec3 lightDir = vec3(0.0);
	vec4 attenIntensity = ApplyLightIntensity(cameraSpacePosition, lightDir);

	float cosAngIncidence = dot(normalize(vertexNormal), lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	outputColor = (diffuseColor * attenIntensity * cosAngIncidence) +
		(diffuseColor * ambientIntensity);
}
