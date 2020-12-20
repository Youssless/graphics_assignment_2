#version 420

out vec4 output_col;

vec3 global_ambient = vec3(0.05, 0.05, 0.05);

uniform vec4 specular_colour;
uniform float shininess;
uniform uint emitmode;

uniform sampler2D tex1;

in GEOM_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;
} f_in;

void main() {
	vec3 emissive = vec3(0);

	/*AMBIENT LIGHTING*/
	// diffuse_colour is the intensity of the light ray multiplied by the combination of the normal and the light direction
	//vec4 ambient = ambient_colour;
	vec4 diffuse_colour = vec4(0.5,0.5,0.5,1.0);

	/*DIFFUSE LIGHTING*/
	float NL = max(dot(f_in.normal, f_in.lightdir),0);
	vec4 diffuse_lighting = NL * diffuse_colour;

	/*SPECULAR LIGHTING*/
	vec3 R = reflect(-f_in.lightdir, f_in.normal);
	vec3 specular_lighting = pow(max(dot(R, f_in.V), 0.0), shininess) * specular_colour.xyz;

	if (emitmode == 1) emissive = vec3(1.0);
	
	vec4 texcolour = texture(tex1, f_in.texcoords);
	vec4 shadedcolour = texcolour*(f_in.colour + diffuse_lighting) + vec4(specular_lighting, 1.0);

	output_col = (f_in.attenuation)*(shadedcolour) + vec4(emissive, 1.0) + vec4(global_ambient, 1.0);
	//output_col = shadedcolour;
}