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
}

Light::~Light() {}

/*
*	send ids to the shader
*	params:
*		const GLuint &program : current shader program reference
*/
void Light::create_component(const GLuint &program) {
	lightpos_id = glGetUniformLocation(program, "lightpos");
	ambient_colour_id = glGetUniformLocation(program, "ambient_colour");
	shininess_id = glGetUniformLocation(program, "shininess");
	specular_colour_id = glGetUniformLocation(program, "specular_colour");
	normal_trans_id = glGetUniformLocation(program, "normal_transformation");
	emitmode_id = glGetUniformLocation(program, "emitmode");
	attenuationmode_id = glGetUniformLocation(program, "attenuationmode");

	light_src.makeSphere(120, 120);
	std::cout << emitmode_id << std::endl;
}


/*
*	update and display the light by passing lighting vars to the frag and vert shaders
*	params:
*		const glm::mat4 &view : pass the current camera view to calculate the light direction
*		glm::mat4 &model : model for the light_src
*		const SharedUniforms &uids : shared uniforms between all objects, used for model_id and normal_trans_id
*/
void Light::display(const glm::mat4 &view, glm::mat4 &model, const SharedUniforms &uids) {

	// transform and scale light_src
	model = glm::translate(model, lightdir);
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uids.model_id, 1, GL_FALSE, &model[0][0]);

	// vars to send to the shader for lighting calculations
	glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(view * model)));
	glUniformMatrix3fv(uids.normal_trans_id, 1, GL_FALSE, &normal_transformation[0][0]);

	lightpos = view * glm::vec4(lightdir, 1.f);
	glUniform4fv(lightpos_id, 1, &lightpos[0]);

	ambient_colour = glm::vec4(0.2f);
	glUniform4fv(ambient_colour_id, 1, &ambient_colour[0]);

	specular_colour = glm::vec4(1.f);
	glUniform4fv(specular_colour_id, 1, &specular_colour[0]);

	shininess = 8.f;
	glUniform1f(shininess_id, shininess);

	attenuationmode = 0;
	glUniform1ui(attenuationmode_id, attenuationmode);

	// draw the light src, switching between emitmodes only applies the emissve on light_src rather
	//	than the whole terrain
	emitmode = 1;
	glUniform1ui(emitmode_id, emitmode);
	light_src.drawSphere(0);
	emitmode = 0;
	glUniform1ui(emitmode_id, emitmode);
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