#include <wrapper_glfw.h>
#include <iostream>

// glm includes
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>

#include "scene.h"

GLuint program, vao;
GLfloat aspect_ratio;

Scene *scene;
GLuint move_mode;

void init(GLWrapper* glw) {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	aspect_ratio = 1024.f / 768.f;
	move_mode = 0;

	glw->set_title("nanimo");

	try {
		program = glw->LoadShader("shader.vert", "shader.frag");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	scene = new Scene(program);
	
}

void display() {
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glUseProgram(program);

	scene->display(aspect_ratio);

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

	if (key == 'L' && action == GLFW_PRESS) move_mode = 8;
	if (key == 'C' && action == GLFW_PRESS) move_mode = 9;

	if (move_mode == 8) { 
		scene->light_keys(key, action); 
		std::cout << "[move_mode = 8] moving light" << std::endl;
	}

	if (move_mode == 9) { 
		scene->camera_keys(key, action); 
		std::cout << "[move_mode = 9] moving camera" << std::endl;
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
	delete(scene);
	return 0;
}
