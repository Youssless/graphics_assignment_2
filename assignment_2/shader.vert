#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;		// Not used
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

uniform mat3 normal_transformation;
uniform vec4 lightpos;

out vec4 fcolour;
out vec3 fV, fnormal, f_lightdir;

void main() {

	vec4 position_h = vec4(position, 1.0);

	fcolour = vec4(colour, 1.f);

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
	fnormal = normalize(N);
	f_lightdir = normalize(L);
	fV = normalize(-P.xyz);
	
	// Define the vertex position
	gl_Position = projection * view * model * position_h;
}