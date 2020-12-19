#pragma once
#include "common.h"
#include "texture.h"
#include "shader.h"

class Skybox {
public:
	Skybox();
	Skybox(Shader* shader);
	~Skybox();

	void create();
	void display(float aspect_ratio);
	void set_shader(Shader* shader);
private:
	Texture texture;
	GLuint texid;
	Shader* shader;
	
	
	GLuint vao, vbo;

};