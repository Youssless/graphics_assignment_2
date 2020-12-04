#include "camera.h"
#include <iostream>

Camera::Camera() {
	eye = glm::vec3(6.0f, 3.0f, 3.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	speed = 0.1f;

	view = glm::mat4(1.0);
	projection = glm::mat4(1.0);
}

Camera::~Camera() {

}

/*
	aspect_ratio = current aspect ratio of the window
	updates the vertex shader with the new camera positions
*/
void Camera::update(const GLfloat &aspect_ratio) {
	projection = glm::perspective(glm::radians(30.0f), aspect_ratio, 0.1f, 100.0f);
	view = glm::lookAt(
		eye,
		center,
		up
	);
}

void Camera::viewmode(GLint vm) {
	switch (vm) {
	case 1:
		eye = glm::vec3(6.0f, 3.0f, 30.0f);
		center = glm::vec3(6.0f, 3.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 2:
		// pan behind
		eye = glm::vec3(6.0f, 3.0f, -30.0f);
		center = glm::vec3(6.0f, 3.0f, 0.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case 3:
		// pan top
		eye = glm::vec3(6.0f, 50.0f, 0.0f);
		center = glm::vec3(6.0f, 3.0f, 0.0f);
		up = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	}
}

/*
	k = key callback
	translates the camera depending on the key input
*/
void Camera::translate(int k) {
	if (k == 'W') translateY(std::minus<float>());
	if (k == 'D') translateX(std::minus<float>());
	if (k == 'A') translateX(std::plus<float>());
	if (k == 'S') translateY(std::plus<float>());
	if (k == GLFW_KEY_UP) translateZ(std::minus<float>());
	if (k == GLFW_KEY_DOWN) translateZ(std::plus<float>());
}

/*
 T: pass in std::plus or std::minus to allow for positive and negative directions
*/

void Camera::translateX(std::function<float (float, float)> op) {
	eye = eye + glm::vec3(op(0, speed), 0.0f, 0.0f);
	center = center + glm::vec3(op(0, speed), 0.0f, 0.0f);
}

void Camera::translateY(std::function<float(float, float)> op) {
	eye = eye + glm::vec3(0.0f, op(0, speed), 0.0f);
	center = center + glm::vec3(0.0f, op(0, speed), 0.0f);
}

void Camera::translateZ(std::function<float(float, float)> op) {
	eye = eye + glm::vec3(0.0f, 0.0f, op(0, speed));
}

