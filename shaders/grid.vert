#version 330

// input attributes
layout(location = 0) in vec3 position;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


void main() {
	
	gl_Position = projMat*viewMat*modelMat*vec4(position,1);
}