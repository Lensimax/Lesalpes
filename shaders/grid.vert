#version 330

// input attributes
layout(location = 0) in vec3 position;


uniform mat4 mdvMat;
uniform mat4 projMat;
uniform mat4 normalMatrix;


void main() {

	gl_Position = projMat*mdvMat*vec4(position,1);

}