#include "light.h"
#include <iostream>

Light::Light() {
	lightdir = glm::vec3(0.f, 10.f, 10.f);
	lightpos = glm::vec4(1.f);

	lightpos_id = 0;
	ambient_colour_id = 0;
	shininess_id = 0;
	specular_colour_id = 0;
	normal_trans_id = 0;
	emitmode_id = 0;

	ambient_colour = glm::vec4(0.f);
	specular_colour = glm::vec4(1.f);
	shininess = 0.f;
	step = 0.1f;

	emitmode = 0;
	attenuationmode = 0;

	light_src = Sphere(false);
	light_src.makeSphere(120, 120);
}

Light::~Light() {}

void Light::set_shader(Shader& shader) {
	this->shader = shader;
}

/*
*	update and display the light by passing lighting vars to the frag and vert shaders
*	params:
*		const glm::mat4 &view : pass the current camera view to calculate the light direction
*		glm::mat4 &model : model for the light_src
*		const SharedUniforms &uids : shared uniforms between all objects, used for model_id and normal_trans_id
*/
void Light::display(const glm::mat4 &view, glm::mat4 &model) {

	// transform and scale light_src
	model = glm::translate(model, lightdir);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	shader.send_model(model);

	// vars to send to the shader for lighting calculations
	glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(view * model)));
	shader.send_normal_transformation(normal_transformation);

	lightpos = view * glm::vec4(lightdir, 1.f);
	shader.send_lightpos(lightpos);

	ambient_colour = glm::vec4(0.2f);
	shader.send_ambient_colour(ambient_colour);

	specular_colour = glm::vec4(1.f);
	shader.send_specular_colour(specular_colour);

	shininess = 8.f;
	shader.send_shininess(shininess);

	attenuationmode = 0;
	shader.send_attenuationmode(attenuationmode);

	// draw the light src, switching between emitmodes only applies the emissve on light_src rather
	//	than the whole terrain
	emitmode = 1;
	shader.send_emitmode(emitmode);
	light_src.drawSphere(0);
	emitmode = 0;
	shader.send_emitmode(emitmode);
}

void Light::translate(int k) {
	if (k == 'W') translateY(std::plus<float>());
	if (k == 'D') translateX(std::plus<float>());
	if (k == 'A') translateX(std::minus<float>());
	if (k == 'S') translateY(std::minus<float>());
	if (k == GLFW_KEY_UP) translateZ(std::minus<float>());
	if (k == GLFW_KEY_DOWN) translateZ(std::plus<float>());
}

void Light::translateX(std::function<float(float, float)> op) {
	lightdir = lightdir + glm::vec3(op(0.f, step), 0.f, 0.f);
}

void Light::translateY(std::function<float(float, float)> op) {
	lightdir = lightdir + glm::vec3(0.f, op(0.f, step), 0.f);
}

void Light::translateZ(std::function<float(float, float)> op) {
	lightdir = lightdir + glm::vec3(0.f, 0.f, op(0.f, step));
}