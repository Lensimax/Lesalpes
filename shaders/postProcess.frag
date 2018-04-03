#version 330

out vec4 bufferColor;

uniform sampler2D renderedMap;

in vec2 texcoord;


void main() {
  

  bufferColor = texture(renderedMap, texcoord);
}
