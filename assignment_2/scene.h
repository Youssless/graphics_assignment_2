#pragma once
#include <iostream>
#include "camera.h"
#include "light.h"
#include "terrain_object.h"
#include "tiny_loader_texture.h"
#include "texture.h"
#include "skybox.h"

class Scene {
public:
	Scene(const GLuint &program, const GLuint &skybox_program);
	~Scene();

	void display(float aspect_ratio);
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

	Texture texture;

	Skybox* skybox;

	GLuint texid;

};