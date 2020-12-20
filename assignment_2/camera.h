#pragma once

#include "base_component.h"
#include "stack"

class Camera : public BaseComponent {
public:
	Camera();
	~Camera();

	GLfloat speed;
	glm::mat4 view, projection;

	void set_shader(Shader& shader);

	void send_data(const GLfloat &aspect_ratio);
	void translate(int k);

	void viewmode(GLint vm);

	void lock();
	void unlock();

private:
	std::stack<glm::mat4> model;
	glm::vec3 eye, center, up, prev_eye, prev_center, prev_up;;
	GLuint view_id, projection_id, model_id;
	Shader shader;

	void translateX(std::function<float (float, float)> op);
	void translateY(std::function<float (float, float)> op);
	void translateZ(std::function<float (float, float)> op);
};