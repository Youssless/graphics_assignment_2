#version 420 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time;

in VERT_OUT {
	vec2 texcoords;
} g_in[];

out vec2 ftexcoords;

vec3 calculate_normal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position); 

	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
	float magnitude = 8.0;
	vec3 direction = normal * ((sin(time) + 1.0)/2.0)*magnitude;

	return position + vec4(direction, 1.0);
}

void main() {
	// calculate normals
	vec3 normal = calculate_normal();
	gl_Position = explode(gl_in[0].gl_Position, normal);
	ftexcoords = g_in[0].texcoords;
	EmitVertex();
	gl_Position = explode(gl_in[1].gl_Position, normal);
	ftexcoords = g_in[1].texcoords;
	EmitVertex();
	gl_Position = explode(gl_in[2].gl_Position, normal);
	ftexcoords = g_in[2].texcoords;
	EmitVertex();
	

	EndPrimitive();
}

