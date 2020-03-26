#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

//varying vec3 color; // Pass to fragment shader
varying vec3 verPos; // in camera space
varying vec3 normal; // in camera space

void main()
{
	gl_Position = P * MV * aPos;
	normal = (MV * vec4(aNor, 0.0)).xyz;
	verPos = (MV * aPos).xyz;
}
