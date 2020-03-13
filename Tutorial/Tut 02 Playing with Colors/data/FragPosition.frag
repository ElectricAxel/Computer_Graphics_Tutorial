#version 330

out vec4 outputColor;

uniform float windowHeight;

void main()
{
	float lerpValue = gl_FragCoord.y / windowHeight;
		
	outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.2f, 0.2f, 0.2f, 1.0f), lerpValue);
}
