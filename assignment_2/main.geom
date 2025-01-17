#version 420

// PASS THROUGH GEOMETRY SHADER

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

// in variables passed from the vert
in VERT_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;
} g_in[];

// out variables passed into the frag
out GEOM_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;
} g_out;


void main() {
	for (int i = 0; i < gl_in.length(); i++) {
		// pass through the values to the frag for the lighting calcuations
		g_out.texcoords = g_in[i].texcoords;
		g_out.colour = g_in[i].colour;
		g_out.attenuation = g_in[i].attenuation;
		g_out.V = g_in[i].V;
		g_out.normal = g_in[i].normal;
		g_out.lightdir = g_in[i].lightdir;
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}