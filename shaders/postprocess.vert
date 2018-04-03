layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textcoord;

uniform sampler2D postMap;

out float fogFactor;
out vec4 fragmentColor;

void main() {
	//calcul du facteur de brouillard
	fogFactor = exp(-(position.z));
	fragmentColor = texture(postMap,textcoord);
}