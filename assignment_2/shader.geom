#version 420

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gtexcoords[];
out vec2 ftexcoords;

void main() {
	for (int i = 0; i < gl_in.length(); i++) {
		ftexcoords = gtexcoords[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}