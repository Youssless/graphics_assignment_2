#include "scene.h"

Scene::Scene(const GLuint &program) {
	uids = SharedUniforms(program);

	camera = new Camera();
	camera->create_component(program);

	light = new Light();
	light->create_component(program);

	terrain = new terrain_object(3.f, 1.f, 6.f);
	terrain->createTerrain(600.f, 600.f, 12.f, 12.f);
	terrain->setColour(glm::vec3(0.0f, 1.0f, 1.0f));
	terrain->createObject();

	pyramids = new TinyObjLoader();
	pyramids->load_obj("..\\obj\\pyramids_of_giza.obj");

	sphyinx = new TinyObjLoader();
	sphyinx->load_obj("..\\obj\\sphyinx.obj");


	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	try {
		texture.load("..\\textures\\sand.png", texid, true);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	int loc = glGetUniformLocation(program, "tex1");
	if (loc > 0) glUniform1i(loc, 0);
}

Scene::~Scene() {
	if (camera) delete(camera);
	if (light) delete(light);
	if (terrain) delete(terrain);
	if (pyramids) delete(pyramids);
	if (sphyinx) delete(sphyinx);
}

void Scene::display(float aspect_ratio) {
	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.f));

	// display camera
	camera->display(aspect_ratio);

	// display light source
	model.push(model.top());
	{
		light->display(camera->view, model.top(), uids);
	}
	model.pop();

	texture.bind_texture(texid);
	// display terrain
	model.push(model.top());
	{
		model.top() = glm::scale(model.top(), glm::vec3(1.5f, 1.0f, 1.5f));
		glUniformMatrix4fv(uids.model_id, 1, GL_FALSE, &model.top()[0][0]);

		glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		glUniformMatrix3fv(uids.normal_trans_id, 1, GL_FALSE, &normalmatrix[0][0]);
		terrain->drawObject(0);
	}
	model.pop();

	
	// display pyramids
	model.push(model.top());
	{
		model.top() = glm::translate(model.top(), glm::vec3(-0.5f, -0.25f, 1.5f));
		model.top() = glm::rotate(model.top(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		model.top() = glm::scale(model.top(), glm::vec3(2.f));

		glUniformMatrix4fv(uids.model_id, 1, GL_FALSE, &model.top()[0][0]);

		glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		glUniformMatrix3fv(uids.normal_trans_id, 1, GL_FALSE, &normalmatrix[0][0]);
		pyramids->drawObject(0);
	}
	model.pop();
	
	

	
	// display sphyinx
	model.push(model.top());
	{
		model.top() = glm::translate(model.top(), glm::vec3(-0.75f, 0.5f, 7.f));
		//model.top() = glm::rotate(model.top(), glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
		model.top() = glm::scale(model.top(), glm::vec3(0.75f));

		glUniformMatrix4fv(uids.model_id, 1, GL_FALSE, &model.top()[0][0]);

		glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		glUniformMatrix3fv(uids.normal_trans_id, 1, GL_FALSE, &normalmatrix[0][0]);
		sphyinx->drawObject(0);
	}
	model.pop();
	texture.unbind_texture();

}

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

void Scene::light_keys(int key, int action) {
	light->translate(key);
}