#pragma once
#include "common.h"
#include "texture.h"
#include "cube_tex.h"

class Skybox {
public:
	Skybox(const GLuint &program);
	~Skybox();

	void create();
	void display();
	
private:
	Texture texture;
	GLuint texid;
	
	GLuint vao, vbo;

};