#version 330

// input attributes
layout(location = 0) in vec3 position;


uniform mat4 mdvMat;
uniform mat4 projMat;
uniform mat4 normalMatrix;

out vec2 texCoord;


void main() {
	texCoord = position.xy *0.5 + 0.5;



	gl_Position = projMat * mdvMat * vec4(position, 1);

}