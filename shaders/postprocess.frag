#version 330

out vec4 bufferColor;

uniform sampler2D renderedMap;
uniform sampler2D shadowMap;

in vec2 texcoord;
in vec3 pos;
in vec4 shadowCoord;
in vec4 viewSpace;

void main() { 

	/* Calcul des ombres */
	/* profondeur par rapport a la lumière */
	float lz = texture(shadowMap, shadowCoord.xy).x;

	/*float bias = 0.005;
	float v = 1.0;

	if(lz + bias < shadowCoord.z){
		v = 0.2;
	}*/

	vec4 fogColor = vec4(0.5,0.5,0.5,1);
	float fogDensity = 0.2;
    float fogFactor = clamp(exp(-pow((length(viewSpace.xyz) * fogDensity), 2)), 0.0, 1.0 );
	bufferColor = mix(fogColor, texture(renderedMap, texcoord), fogFactor);
    //bufferColor = texture(renderedMap, texcoord)*length(viewSpace);
}