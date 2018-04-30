#version 330

// input attributes 
layout(location = 0) in vec3 position;// position of the vertex in world space

out vec2 texcoord;
out vec3 pos;

uniform mat4 mvpDepth;
uniform mat4 mdvMat;

out vec4 shadowCoord;
out vec4 viewSpace;


void main() {
	gl_Position = vec4(position,1.0);
	texcoord = position.xy*0.5+0.5;
	pos = position;

  	shadowCoord = (mvpDepth * vec4(position,1.0)) * 0.5 + 0.5 ;
  	viewSpace = mdvMat * vec4(position,1.0);
}
