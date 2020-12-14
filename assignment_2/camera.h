#pragma once

#include "base_component.h"
#include "stack"

class Camera : public BaseComponent {
public:
	Camera();
	~Camera();

	GLfloat speed;
	glm::mat4 view, projection;
	

	void create_component(const GLuint &program);

	void display(const GLfloat &aspect_ratio);
	void translate(int k);

	void viewmode(GLint vm);

	void set_view();

private:
	std::stack<glm::mat4> model;
	glm::vec3 eye, center, up;
	GLuint view_id, projection_id, model_id;

	void translateX(std::function<float (float, float)> op);
	void translateY(std::function<float (float, float)> op);
	void translateZ(std::function<float (float, float)> op);
};