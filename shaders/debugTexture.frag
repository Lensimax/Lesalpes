#version 330

out vec4 outBuffer;

uniform sampler2D myTexture;

void main() {
 	outBuffer = texelFetch(myTexture,ivec2(gl_FragCoord.xy),0);
}
