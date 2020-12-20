#include "camera.h"
#include <iostream>


/*
*	camera constructor 
*	params:
*/
Camera::Camera() {
	eye = glm::vec3(0.0f, 9.0f, 40.0f);
	center = glm::vec3(0.0f, 2.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	speed = 0.1f;

	view = glm::mat4(1.0);
	projection = glm::mat4(1.0);

	view_id = 0;
	projection_id = 0;
}

Camera::~Camera() {

}

/*
* sets the current shader
* params:
*	const Shader &shader : shader for the skybox
*/
void Camera::set_shader(Shader& shader) {
	this->shader = shader;
}

/*
*	updates the vertex shader with the current camera positions, shader computes the camera position
*		to display the camera
*	params:
*		aspect ratio to update the perspective when the widow size changes
*/
void Camera::send_data(const GLfloat &aspect_ratio) {
	projection = glm::perspective(glm::radians(30.0f), aspect_ratio, 0.1f, 100.0f);
	shader.send_projection(projection);

	view = glm::lookAt(
		eye,
		center,
		up
	);
	shader.send_view(view);
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
	if (k == 'W') translateY(std::plus<float>());
	if (k == 'D') translateX(std::plus<float>());
	if (k == 'A') translateX(std::minus<float>());
	if (k == 'S') translateY(std::minus<float>());
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

void Camera::lock() {
	prev_eye = eye;
	prev_center = center;
	prev_up = up;

	eye = glm::vec3(0.0f, 0.0f, -1.5f);
	center = glm::vec3(0.0f, 0.0f, -1.f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::unlock() {
	eye = prev_eye;
	center = prev_center;
	up = prev_up;
}

