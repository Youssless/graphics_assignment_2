#pragma once

#include "base_component.h"


class Camera : public BaseComponent {
public:
	Camera();
	~Camera();

	GLfloat speed;
	glm::mat4 view, projection;
	
	void update(const GLfloat &aspect_ratio);
	void translate(int k);

	void viewmode(GLint vm);

private:
	glm::vec3 eye, center, up;

	void translateX(std::function<float (float, float)> op);
	void translateY(std::function<float (float, float)> op);
	void translateZ(std::function<float (float, float)> op);
};