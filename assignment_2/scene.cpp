#include "scene.h"

/*
* scene constructor
* params:
*	const GLuint &program : shader program for objects and lighting
*	const GLuint &skybox_program : shader program for skybox
*/
Scene::Scene(std::vector<Shader> &shaders) {
	main_shader = shaders[0];
	skybox_shader = shaders[1];
	explosion_shader = shaders[2];
}

Scene::~Scene() {
	if (camera) delete(camera);
	if (light) delete(light);
	if (terrain) delete(terrain);
	if (pyramids) delete(pyramids);
	if (sphyinx) delete(sphyinx);
	if (skybox) delete(skybox);
	if (debree) delete(debree);
}


/*
* initialise scene
* params:
*/
void Scene::create() {
	// initialise camera
	camera = new Camera();
	camera->set_shader(main_shader);

	ufo_camera = new Camera();
	ufo_camera->set_shader(explosion_shader);

	// initialise light
	light = new Light();
	light->set_shader(main_shader);

	debree = new Light();
	debree->set_shader(explosion_shader);

	// initialise terrain
	terrain = new terrain_object(8.f, 1.f, 4.f);
	terrain->createTerrain(300.f, 300.f, 50.f, 50.f);
	terrain->setColour(glm::vec3(0.0f, 1.0f, 1.0f));
	terrain->createObject();

	// initialise objects
	pyramids = new TinyObjLoader();
	pyramids->load_obj("..\\obj\\pyramids_of_giza.obj");

	sphyinx = new TinyObjLoader();
	sphyinx->load_obj("..\\obj\\sphyinx.obj");

	spaceship = new TinyObjLoader();
	spaceship->load_obj("..\\obj\\spaceship.obj");
	s_x = 20.f;
	s_y = 20.f;
	anim_speed = 0.1f;

	

	//initialise skybox 
	skybox = new Skybox();
	skybox->set_shader(skybox_shader);
	skybox->create();

	sphere = Sphere(true);
	sphere.makeSphere(120, 120);

	// load textures
	try {
		Texture::load("..\\textures\\sand.png", texid, true);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	int loc = glGetUniformLocation(main_shader.program, "tex1");
	if (loc > 0) glUniform1i(loc, 0);
}


/*
* displays the whole scene
* params:
*	float aspec_ratio : current aspect_ratio of the widow
*/
void Scene::display(float aspect_ratio) {
	main_shader.use(1);
	{
		display_model(aspect_ratio);
	}
	main_shader.use(0);

	camera->set_shader(explosion_shader);
	light->set_shader(explosion_shader);
	explosion_shader.use(1);
	{
		display_spaceship(aspect_ratio);
	}
	explosion_shader.use(0);
	
	explosion_shader.use(1);
	{
		display_debree(aspect_ratio);
	}
	explosion_shader.use(0);

	camera->set_shader(main_shader);
	light->set_shader(main_shader);

	glDepthFunc(GL_LEQUAL);
	skybox_shader.use(1);
	{
		display_skybox(aspect_ratio);
		glDisableVertexAttribArray(0);
	}
	skybox_shader.use(0);
	glDepthFunc(GL_LESS);
}

/*
* displays the whole scene
* params:
*	float aspec_ratio : current aspect_ratio of the widow
*/
void Scene::display_model(float aspect_ratio) {
	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.f));

	// display camera
	camera->send_data(aspect_ratio);

	//// display light source

	model.push(model.top());
	{
		light->send_data(camera->view, model.top());
		light->display();
	}
	model.pop();

	Texture::bind_texture(texid);
	// display terrain
	model.push(model.top());
	{
		model.top() = glm::scale(model.top(), glm::vec3(1.5f, 1.0f, 1.5f));
		main_shader.send_model(model.top());

		glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		main_shader.send_normal_transformation(normal_transformation);
		terrain->drawObject(0);
	}
	model.pop();

	// display pyramids
	model.push(model.top());
	{
		model.top() = glm::translate(model.top(), glm::vec3(-0.5f, -0.5f, 1.5f));
		model.top() = glm::rotate(model.top(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		model.top() = glm::scale(model.top(), glm::vec3(2.f));

		main_shader.send_model(model.top());

		glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		main_shader.send_normal_transformation(normal_transformation);
		pyramids->drawObject(0);
	}
	model.pop();

	// display sphyinx
	model.push(model.top());
	{
		model.top() = glm::translate(model.top(), glm::vec3(-0.75f, 0.4f, 7.f));
		//model.top() = glm::rotate(model.top(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		model.top() = glm::scale(model.top(), glm::vec3(0.75f));

		main_shader.send_model(model.top());

		glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		main_shader.send_normal_transformation(normal_transformation);
		sphyinx->drawObject(0);
	}
	model.pop();
	Texture::unbind_texture();
}

/*
* displays the skybox in the scene
* params:
*	float aspec_ratio : current aspect_ratio of the widow
*/
void Scene::display_skybox(float aspect_ratio) {
	skybox->display(aspect_ratio);
}

/*
* displays the spaceship 
* params:
*	float aspec_ratio : current aspect_ratio of the widow
*/
void Scene::display_spaceship(float aspect_ratio) {
	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.f));

	// display camera
	camera->send_data(aspect_ratio);

	//// display light source
	model.push(model.top());
	{
		light->send_data(camera->view, model.top());
	}
	model.pop();

	GLfloat magnitude = 1.f;
	explosion_shader.send_magnitude(magnitude);

	Texture::bind_texture(texid);
	model.push(model.top());
	{
		//glm::mat4 mvp = camera->projection * camera->view * model.top();

		model.top() = glm::translate(model.top(), glm::vec3(s_x, s_y, 0.0f));
		model.top() = glm::rotate(model.top(), glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
		model.top() = glm::rotate(model.top(), glm::radians(20.f), glm::vec3(1.f, 0.f, 0.f));
		model.top() = glm::scale(model.top(), glm::vec3(1.5f, 1.5f, 1.5f));

		

		float time = glfwGetTime();
		explosion_shader.send_time(time);
		if (time <= 10.25f) {
			s_x -= glfwGetTime() / 60.f;
			s_y -= glfwGetTime() / 60.f;
		}
		else if (time > 10.25f){
			s_x = s_x;
			s_y = s_y;
		}
		

		explosion_shader.send_model(model.top());
		//std::cout << glfwGetTime() << std::endl;
		

		glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		main_shader.send_normal_transformation(normal_transformation);

		spaceship->drawObject(0);
	}
	model.pop();
	Texture::unbind_texture();
}

/*
* displays the debree
* params:
*	float aspec_ratio : current aspect_ratio of the widow
*/
void Scene::display_debree(float aspect_ratio) {
	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.f));
	// display camera
	camera->send_data(aspect_ratio);
	GLfloat magnitude = 8.f;
	explosion_shader.send_magnitude(magnitude);

	//// display light source
	model.push(model.top());
	{
		model.top() = glm::translate(model.top(), glm::vec3(0.0f, -11.0f, -5.0f));
		debree->send_data(camera->view, model.top());
		debree->display();
	}
	model.pop();
}

/*
* translates the camera or changes the camera viewmode depending on the key pressed
* params:
*	int key : current key pressed
*	int action : current action macro
*/
void Scene::camera_keys(int key, int action) {
	camera->translate(key);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		camera->viewmode(1);
		std::cout << "view mode " << std::to_string(1) << " (default view)" << std::endl;
	}
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		camera->viewmode(2);
		std::cout << "view mode " << std::to_string(2) << " (back view)" << std::endl;
	}
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		camera->viewmode(3);
		std::cout << "view mode " << std::to_string(3) << " (top view)" << std::endl;
	}
}

/*
* translates the light
* params:
*	int key : current key pressed
*	int action : current action macro
*/
void Scene::light_keys(int key, int action) {
	light->translate(key);
}