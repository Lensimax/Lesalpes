#version 330

out vec4 bufferColor;

uniform sampler2D renderedMap;

in vec2 texcoord;

void main() {
	//multiplication de la couleur par le facteur de brouillard
	bufferColor = texture(renderedMap, texcoord);
	// bufferColor = vec4(fragmentColor.xyz*fogFactor,fragmentColor.w);
}


/*layout(location = 0) in vec3 position;

in vec2 textcoord;

uniform sampler2D postMap;

out float fogFactor;
out vec4 fragmentColor;

void main() {
	//calcul du facteur de brouillard
	fogFactor = exp(-(position.z));
	fragmentColor = texture(postMap,textcoord);
}*/