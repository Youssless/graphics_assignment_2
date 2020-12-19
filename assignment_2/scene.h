#pragma once
#include <iostream>
#include "camera.h"
#include "light.h"
#include "terrain_object.h"
#include "tiny_loader_texture.h"
#include "texture.h"
#include "skybox.h"
#include "shader.h"

class Scene {
public:
	Scene(std::vector<Shader> &shaders);
	~Scene();
	
	void create();

	void display_model(float aspect_ratio);
	void display_skybox(float aspect_ratio);

	void camera_keys(int key, int action);
	void light_keys(int key, int action);

private:
	Shader main_shader, skybox_shader;

	Camera* camera;
	Light *light;
	Skybox* skybox;
	
	terrain_object *terrain;
	TinyObjLoader *pyramids, *sphyinx;

	GLuint texid;
};