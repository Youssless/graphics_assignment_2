#version 420

//SKYBOX FRAGMENT SHADER

in vec3 ftexcoords;
out vec4 out_colour;

// sampler cube to texture skybox as a cube map
uniform samplerCube skybox;

void main() {
	out_colour = texture(skybox, ftexcoords);
}