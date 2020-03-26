#version 120

uniform vec3 lightPos;
uniform vec3 alightPos;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

uniform float lightInt;
uniform float alightInt;

//varying vec3 color; // passed from the vertex shader
varying vec3 verPos; // in camera space
varying vec3 normal; // in camera space

void main()
{	

	vec3 v = -normalize(verPos);
	vec3 n = normalize(normal);
	vec3 color = vec3(1.0, 1.0, 1.0);
	if (dot(v,n)<= 0.4){//threshold
	    color = vec3(0.0, 0.0, 0.0);
	}
	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
