#version 330

out vec4 bufferColor;

uniform sampler2D renderedMap;
uniform sampler2D shadowMap;

in vec2 texcoord;
in vec3 pos;
in vec4 shadowCoord;

void main() {

	/* Calcul des ombres */
	/* profondeur par rapport a la lumière */
	/*float lz = texture(shadowMap, shadowCoord.xy).x;

	float bias = 0.005;
	float v = 1.0;

	if(lz + bias < shadowCoord.z){
		v = 0.2;
	}*/



	float fogFactor = exp(-(pos.z));
	bufferColor = v*texture(renderedMap, texcoord);
}