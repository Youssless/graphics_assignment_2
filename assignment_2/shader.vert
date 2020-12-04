#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;		// Not used
layout(location = 2) in vec3 normal;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

out vec4 fcolour;

void main() {

	vec4 position_h = vec4(position, 1.0);

	fcolour = vec4(colour, 1.f);
	
	// Define the vertex position
	gl_Position = projection * view * model * position_h;
}