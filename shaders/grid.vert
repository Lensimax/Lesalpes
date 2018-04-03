#version 330

// input attributes
layout(location = 0) in vec3 position;


uniform mat4 mdvMat;
uniform mat4 projMat;

uniform sampler2D heightmap;

out vec2 texCoord;


void main() {

	texCoord = position.xy;

	float height = texture(heightmap, vec2(position.xy)).y;

	vec4 computedPosition = vec4(position.x, position.y, height, 1);
	// vec4 computedPosition = vec4(position, 1);

	gl_Position = projMat*mdvMat*computedPosition;

}