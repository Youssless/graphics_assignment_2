#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;		// Not used
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoords;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

uniform mat3 normal_transformation;
uniform vec4 lightpos;
uniform vec4 ambient_colour;


uniform uint attenuationmode;

// out values to pass through to the geometry shader 
out VERT_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;

} g_out;

void main() {

	vec4 position_h = vec4(position, 1.0);

	//fcolour = vec4(colour, 1.0)*ambient_colour;
	g_out.colour = ambient_colour;

	/*TRANSFORMING AND NORMALISING THE NORMAL AND LIGHT DIRECTION*/
	// define the modelview transformation
	mat4 modelview = view * model; // model view transformation

	vec4 P = modelview * position_h; // transform the vertex positions into modelview
	vec3 L = lightpos.xyz - P.xyz; // calculate the light point on the vertex from the light position

	// transform the normal to the modelview coordinates
	//mat3 normal_transformation = transpose(inverse(mat3(modelview)));
	vec3 N = normal_transformation * normal;

	// normalize the normal and the light direction
	// turn the vectors to unit vectors, so that we dont scale our lighting to the actual size of the normal vector
	g_out.normal = normalize(N);
	g_out.lightdir = normalize(L);
	g_out.V = normalize(-P.xyz);
	
	/*ATTENUATION*/
	float attenuation;
	float distance_to_light = length(L);
	if (attenuationmode == 1) {
		attenuation = 1.0;
		g_out.attenuation = attenuation;
	}
	else if (attenuationmode == 0){ 
		float k1 = 0.5;
		float k2 = 0.5;
		float k3 = 0.5;

		attenuation = 1.0/(k1 + k2*distance_to_light + k3*pow(distance_to_light, 2));
		g_out.attenuation = 40*attenuation;
	}

	g_out.texcoords = vec2(texcoords.x, texcoords.y);

	// Define the vertex position
	gl_Position = projection * view * model * position_h;
}