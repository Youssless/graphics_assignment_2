#include <wrapper_glfw.h>
#include <iostream>

// glm includes
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>

// components & terrain includes
#include "terrain_object.h"
#include "camera.h"
#include "light.h"

#include "uniform_ids.h"

#include "sphere_tex.h"

GLuint program, vao;
GLfloat aspect_ratio;


Uniforms uids;

Camera* camera;
terrain_object* terrain;
Light* light;
Sphere sphere;

void init(GLWrapper* glw) {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	aspect_ratio = 1024.f / 768.f;

	glw->set_title("nanimo");

	try {
		program = glw->LoadShader("shader.vert", "shader.frag");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	// get the uids
	uids = Uniforms(program);
	camera = new Camera();
	camera->create_component(program);
	terrain = new terrain_object(4, 1.f, 2.f);
	terrain->createTerrain(200.f, 200.f, 2.f, 2.f);
	terrain->setColour(glm::vec3(0.0f, 1.0f, 1.0f));
	terrain->createObject();

	light = new Light();
	light->create_component(program);
	
}

void display() {
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glUseProgram(program);

	std::stack<glm::mat4> model;
	model.push(glm::mat4(1.f));

	camera->display(aspect_ratio);

	model.push(model.top());
	{
		/*model.top() = glm::translate(model.top(), glm::vec3(1.f));
		model.top() = glm::scale(model.top(), glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uids.modelID, 1, GL_FALSE, &model.top()[0][0]);*/
		light->display(camera->view, model.top(), uids.normal_trans_id);
	}
	model.pop();

	model.push(model.top());
	{
		glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(camera->view * model.top())));
		glUniformMatrix3fv(uids.normal_trans_id, 1, GL_FALSE, &normalmatrix[0][0]);
		terrain->drawObject(0);
		glUniformMatrix4fv(uids.modelID, 1, GL_FALSE, &model.top()[0][0]);
	}
	model.pop();
	

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspect_ratio = float(w) / float(h);
}

void print_key_bindings() {
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "		KEYS	" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "[W, A, S, D]  pan the camera up, left, down, right" << std::endl;
	std::cout << "[ARROW_UP, ARROW_DOWN]  zoom the camera in, out" << std::endl;
	std::cout << "[ARROW_LEFT, ARROW_RIGHT, SPACE]  animation speed decrease, increase, stop" << std::endl;
	std::cout << "[1, 2, 3]  view mode 1 (default view), view mode 2 (back view), view mode 3 (top view)" << std::endl;
	std::cout << std::endl;
	std::cout << "[M] print menu" << std::endl;
	std::cout << std::endl;
	std::cout << "-----------------------------------------" << std::endl;

}

static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

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



/* Entry point of program */
int main(int argc, char* argv[]) {
	GLWrapper* glw = new GLWrapper(1024, 768, "Lab3 start example");;

	if (!ogl_LoadFunctions()) {
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	// Register the callback functions
	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();

	init(glw);

	print_key_bindings();

	glw->eventLoop();

	delete(glw);
	return 0;
}
