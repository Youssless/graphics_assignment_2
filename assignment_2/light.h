#pragma once
#include "base_component.h"
#include "sphere_tex.h"

class Light : public BaseComponent {
public:
	Light();
	~Light();

	GLuint lightpos_id, ambient_colour_id, shininess_id, 
		specular_colour_id, normal_trans_id, emitmode_id, attenuationmode_id;

	void set_shader(Shader& shader);
	void display();
	void send_data(const glm::mat4& view, glm::mat4& model);

	void translate(int k);

private:
	glm::vec4 lightpos, ambient_colour, specular_colour;
	glm::vec3 lightdir;
	GLfloat shininess, step;
	Sphere light_src;
	GLuint emitmode, attenuationmode;
	Shader shader;

	

	void translateX(std::function<float(float, float)> op);
	void translateY(std::function<float(float, float)> op);
	void translateZ(std::function<float(float, float)> op);
};