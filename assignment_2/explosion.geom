#version 420 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time;

uniform float magnitude;

in VERT_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;
} g_in[];

out GEOM_OUT {
	vec2 texcoords;
	vec4 colour;
	float attenuation;
	vec3 V, normal, lightdir;
} g_out;

vec3 calculate_normal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position); 

	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
	vec3 direction = normal * ((pow(log2(time), 2.5)))*magnitude;
	return position + vec4(direction.xy, 0.0, 0.0);
}



void main() {
	//calculate normals
	vec3 normal = calculate_normal();
	for (int i = 0; i < gl_in.length(); i++) {
		
		if (time <= 10.25)
			gl_Position = gl_in[i].gl_Position;
		if (time > 10.25)
			gl_Position = explode(gl_in[i].gl_Position, normal);

		g_out.texcoords = g_in[i].texcoords;
		g_out.colour = g_in[i].colour;
		g_out.attenuation = g_in[i].attenuation;
		g_out.V = g_in[i].V;
		g_out.normal = g_in[i].normal;
		g_out.lightdir = g_in[i].lightdir;
			
		EmitVertex();
	}
	EndPrimitive();
}

