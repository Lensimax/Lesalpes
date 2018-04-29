#version 330

// input attributes 
layout(location = 0) in vec3 position; 

uniform mat4 mvpMat;

uniform sampler2D heightmap;


void main() {
	vec2 texcoord = position.xy * 0.5 + 0.5;
	float height = texture(heightmap, texcoord).x;
	height = (height - 0.5) * (-1);

  	gl_Position = mvpMat*vec4(position.x, position.y,height,1.0);
}
