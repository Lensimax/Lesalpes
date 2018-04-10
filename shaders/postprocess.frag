#version 330

out vec4 bufferColor;

uniform sampler2D renderedMap;

in vec2 texcoord;
in vec3 pos;

void main() {
	float fogFactor = exp(-(pos.z));
	bufferColor = texture(renderedMap, texcoord);
}