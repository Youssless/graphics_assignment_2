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
	Scene(std::vector<Shader*> shaders);
	~Scene();

	void display_model(float aspect_ratio);
	void display_skybox(float aspect_ratio);

	void camera_keys(int key, int action);
	void light_keys(int key, int action);

private:
	Camera *camera, *skybox_camera;
	Light *light;
	terrain_object *terrain;
	SharedUniforms uids;
	TinyObjLoader *pyramids;
	TinyObjLoader *sphyinx;

	Shader* main_shader, * skybox_shader;

	Texture texture;

	Skybox* skybox;

	GLuint texid;

};