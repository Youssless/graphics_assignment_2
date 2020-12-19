#version 420
// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;		// Not used
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 texcoords;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;

out VERT_OUT {
	vec2 texcoords;
} g_out;

void main() {
	vec4 position_h = vec4(position, 1.0);


	g_out.texcoords = vec2(texcoords.x, texcoords.y);

	// Define the vertex position
	gl_Position = projection * view * model * position_h;
}