#version 330

// input attributes 
layout(location = 0) in vec3 position; 

uniform mat4 mdvMat;
uniform mat4 projMat;

uniform sampler2D heightmap;

void main() {
	vec2 texcoord = position.xy * 0.5 + 0.5;
	
	float height = texture(heightmap,texCoord).z;

	if(texture(heightmap,texCoord).z <= 0.2){
		height = 0.2;
	}
	height = height-0.5;
	
	vec4 computedPosition = vec4(position.x, position.y, height, 1);


  	gl_Position = projMat*mdvMat*vec4(position.xy,height,1.0);
}
