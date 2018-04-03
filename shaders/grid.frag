#version 330

out vec4 outBuffer;

uniform sampler2D heightmap;
uniform sampler2D normalMap;

in vec2 texCoord;

void main(){

	// outBuffer = texture(heightmap, texCoord);
	outBuffer = texture(normalMap, texCoord);
	// outBuffer = vec4(1.0,0.0,0.0,1.0);
}