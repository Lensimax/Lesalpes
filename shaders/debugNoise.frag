#version 330

out vec4 outBuffer;

uniform sampler2D noiseMap;

void main() {
  outBuffer = texelFetch(noiseMap,ivec2(gl_FragCoord.xy),0);
}
