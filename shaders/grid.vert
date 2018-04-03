#version 330

// input attributes
layout(location = 0) in vec3 position;


uniform mat4 mdvMat;
uniform mat4 projMat;

uniform sampler2D heightmap;

out vec2 texCoord;


void main() {

	texCoord = position.xy*0.5+0.5;

	float scale = 1.5;

	float height = scale * texture(heightmap, vec2(position.xy)).z;

	// vec4 computedPosition = vec4(position.x, position.y, height, 1);
	vec4 computedPosition = vec4(position, 1);

	gl_Position = projMat*mdvMat*computedPosition;

}