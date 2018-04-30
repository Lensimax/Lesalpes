#version 330

out vec4 outBuffer;



// layout(location = 0) out vec4 outRendered;

/*vec4 phong(vec3 myColor, float specDeg){
	const vec3 ambientColor  = vec3(0.1,0.1,0.1);
	const vec3 specular = vec3(0.9,0.9,0.9);

	// normal / view and light directions (in camera space)
	vec3 n = normalize(normalView);
	vec3 e = normalize(eyeView);
	vec3 l = normalize(lightVector);

	// diffuse and specular components of the phong shading model
	float diff = max(dot(l,n),0.0);
	float spec = pow(max(dot(reflect(l,n),e),0.0),specDeg);

	// final color 
	vec3 color = ambientColor + diff*myColor + spec*specular;
	return vec4(color,1.0);
}*/

void main(){

	outBuffer = vec4(1,0,0,1);
}