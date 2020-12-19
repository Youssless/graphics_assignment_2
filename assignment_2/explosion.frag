#version 420

in vec2 ftexcoords;

out vec4 out_colour;

uniform sampler2D tex1;

void main() {
	out_colour = texture(tex1, ftexcoords);
}