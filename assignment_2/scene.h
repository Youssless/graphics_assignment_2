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

	void display(float aspect_ratio);

	void camera_keys(int key, int action);
	void light_keys(int key, int action);

private:
	Shader main_shader, skybox_shader, explosion_shader;

	Camera* camera, *ufo_camera;
	Light *light, *debree;
	Skybox* skybox;

	Sphere sphere;
	
	terrain_object *terrain;
	TinyObjLoader *pyramids, *sphyinx, *spaceship;

	GLuint texid;

	GLfloat s_x, s_y;
	GLfloat anim_speed;

	void display_model(float aspect_ratio);
	void display_skybox(float aspect_ratio);
	void display_spaceship(float aspect_ratio);
	void display_fireworks(float aspect_ratio);
};