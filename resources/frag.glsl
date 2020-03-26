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
	
	vec3 templight = normalize(lightPos-verPos);
	float intensity = max( 0.0, dot( templight,normal ) );	
	vec3 tempKD = kd* intensity;
	
	vec3 v = -normalize(verPos);
	vec3 h = (templight + v)/ (length(templight + v));
	h = normalize(h);
	float reflectance = pow(max(0,dot(h,normal)),s);
	vec3 tempKS = ks * reflectance;

	vec3 atemplight = normalize(alightPos-verPos);
	vec3 ah = (atemplight + v)/ (length(atemplight + v));
	ah = normalize(ah);
	float areflectance = pow(max(0,dot(ah,normal)),s);
	vec3 atempKS = ks * areflectance;
	
	vec4 color1 = lightInt*vec4(ka.r+tempKD.r+tempKS.r,ka.g+tempKD.g+tempKS.g,ka.b+tempKD.b+tempKS.b, 1.0);
	vec4 color2 = alightInt* vec4(ka.r+tempKD.r+atempKS.r,ka.g+tempKD.g+atempKS.g,ka.b+tempKD.b+atempKS.b, 1.0);
     	gl_FragColor = vec4(color1.r+color2.r,color1.g+color2.g,color1.b+color2.b, 1.0);
	

	//gl_FragColor = vec4(ka.r, ka.g, ka.b, 1.0);
	//vec3 n = normalize(normal);
	//vec3 color = 0.5 * (n + 1.0);
	//gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
