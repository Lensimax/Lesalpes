#version 330

out vec4 outBuffer;

uniform sampler2D normalMap;

void main() {
  outBuffer = texelFetch(normalMap,ivec2(gl_FragCoord.xy),0);
}
