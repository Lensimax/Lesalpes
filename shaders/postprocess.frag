in float fogFactor;
in vec4 fragmentColor;

out vec4 bufferColor;

void main() {
	//multiplication de la couleur par le facteur de brouillard
	bufferColor = vec4(fragmentColor.xyz*fogFactor,fragmentColor.w);
}