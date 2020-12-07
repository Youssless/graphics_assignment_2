#pragma once
#include "base_component.h"
#include "sphere_tex.h"
#include <stack>

class Light : public BaseComponent {
public:
	Light();
	~Light();

	GLuint lightpos_id, ambient_colour_id, shininess_id, specular_colour_id, normal_trans_id;

	void create_component(const GLuint &program);
	void display(const glm::mat4& view, glm::mat4 &model, const GLuint &normal_trans_id);

private:
	glm::vec4 lightpos, ambient_colour, specular_colour;
	glm::vec3 lightdir;
	GLfloat shininess;
	Sphere light_src;

	void translateX(std::function<float(float, float)> op);
	void translateY(std::function<float(float, float)> op);
	void translateZ(std::function<float(float, float)> op);
};