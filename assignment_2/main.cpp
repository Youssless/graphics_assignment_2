#include <wrapper_glfw.h>
#include <iostream>

// glm includes
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>

#include "scene.h"

#include "shader.h"

GLuint program, skybox_program, vao;
GLfloat aspect_ratio;

Scene *scene;
GLuint move_mode;
std::vector<Shader*> shaders;

std::vector<std::string> shader_paths = {
	"shader.vert",
	"shader.frag",
	"skybox.vert",
	"skybox.frag",
};

void init(GLWrapper* glw) {

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	aspect_ratio = 1024.f / 768.f;
	move_mode = 0;

	shaders.insert(shaders.begin(), new Shader({"shader.vert", "shader.frag"}));
	shaders.insert(shaders.begin() + 1, new Shader({"skybox.vert", "skybox.frag"}));
	glw->set_title("Pyramids in Space");
	scene = new Scene(shaders);
	
}

void display() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// display models
	shaders[0]->use(1);
	scene->display_model(aspect_ratio);
	shaders[0]->use(0);

	// display skybox
	glDepthFunc(GL_LEQUAL);
	shaders[1]->use(1);
	scene->display_skybox(aspect_ratio);
	
	glDisableVertexAttribArray(0);
	shaders[1]->use(0);
	glDepthFunc(GL_LESS);
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
	std::cout << "[C, L] C = Camera mode, L = Light mode" << std::endl;
	std::cout << "|NOTE| Depending on the mode you can use the [W, A, S, D] keys to move ethier the camera or the light" << std::endl;
	std::cout << std::endl;
	std::cout << "[W, A, S, D]  pan the camera/light up, left, down, right" << std::endl;
	std::cout << "[ARROW_UP, ARROW_DOWN]  zoom the camera/light in, out" << std::endl;
	//std::cout << "[ARROW_LEFT, ARROW_RIGHT, SPACE]  animation speed decrease, increase, stop" << std::endl;
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

	if (key == 'C' && action == GLFW_PRESS) {
		move_mode = 0;
		std::cout << "|move_mode = Camera| moving camera" << std::endl;
	}
	if (key == 'L' && action == GLFW_PRESS) {
		move_mode = 1;
		std::cout << "|move_mode = Light| moving light" << std::endl;
	}

	if (move_mode == 0) { 
		scene->camera_keys(key, action); 
	}

	if (move_mode == 1) {
		scene->light_keys(key, action);
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
	std::cout << "|move_mode = Camera| moving camera" << std::endl;

	glw->eventLoop();

	delete(glw);
	delete(scene);
	return 0;
}
