#version 420

in vec4 fcolour;
out vec4 output_col;

in vec3 fV, fnormal, f_lightdir;

vec3 global_ambient = vec3(0.05, 0.05, 0.05);
vec3 emissive = vec3(1.0, 1.0, 0.8);

uniform vec4 ambient_colour, specular_colour;
uniform float shininess;


void main() {
	/*AMBIENT LIGHTING*/
	// diffuse_colour is the intensity of the light ray multiplied by the combination of the normal and the light direction
	vec4 ambient = fcolour*ambient_colour;

	/*DIFFUSE LIGHTING*/
	float NL = max(dot(fnormal, f_lightdir),0);
	vec4 diffuse_lighting = fcolour * NL;

	/*SPECULAR LIGHTING*/
	vec3 R = reflect(-f_lightdir, fnormal);
	vec3 specular_lighting = pow(max(dot(R, fV), 0.0), shininess) * specular_colour.xyz;


	output_col = ambient + diffuse_lighting + vec4(specular_lighting, 1.0);
}