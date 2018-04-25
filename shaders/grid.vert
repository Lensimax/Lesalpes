#version 330

// input attributes
layout(location = 0) in vec3 position;


uniform mat4 mdvMat;
uniform mat4 projMat;
uniform mat3 normalMatrix;
uniform vec3 lighVector;

uniform sampler2D heightmap;
uniform sampler2D normalMap;

out vec2 texCoord;
out vec3 normalView;
out vec3 eyeView;

void main() {
	texCoord = position.xy *0.5 + 0.5;

	/* creation de la hauteur */
	float height =  texture(heightmap,texCoord).z;
	vec4 computedPosition = vec4(position.x, position.y, height, 1);

	/* pour phong */
	vec3 normal = texture(normalMap, texCoord).xyz;
	normalView = normalize(normalMatrix*normal);
	eyeView = (mdvMat*computedPosition).xyz;





	gl_Position = projMat*mdvMat*computedPosition;

}