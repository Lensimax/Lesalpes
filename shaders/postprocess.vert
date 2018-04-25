#version 330

// input attributes 
layout(location = 0) in vec3 position;// position of the vertex in world space

out vec2 texcoord;
out vec3 pos;

uniform mat4 mdvDepth;

out vec4 shadowCoord;

void main() {
	gl_Position = vec4(position,1.0);
	texcoord = position.xy*0.5+0.5;
	pos = position;

  	shadowCoord = (mdvDepth * vec4(position,1.0)) * 0.5 + 0.5 ;
}
