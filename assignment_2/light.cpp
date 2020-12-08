#include "light.h"

Light::Light() {
	lightdir = glm::vec3(1.f);
	lightpos = glm::vec4(1.f);

	lightpos_id = 0;
	ambient_colour_id = 0;
	shininess_id = 0;
	specular_colour_id = 0;
	normal_trans_id = 0;

	ambient_colour = glm::vec4(0.f);
	specular_colour = glm::vec4(1.f);
	shininess = 0.f;

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

	light_src.makeSphere(120, 120);
}


/*
*	update and display the light by passing lighting vars to the frag and vert shaders
*	params:
*		const glm::mat4 &view : pass the current camera view to calculate the light direction
*/
void Light::display(const glm::mat4 &view, glm::mat4 &model, const GLuint& normal_trans_id) {
	lightpos = view * glm::vec4(lightdir, 1.f);
	glUniform4fv(lightpos_id, 1, &lightpos[0]);

	ambient_colour = glm::vec4(0.2f);
	glUniform4fv(ambient_colour_id, 1, &ambient_colour[0]);

	specular_colour = glm::vec4(1.f);
	glUniform4fv(specular_colour_id, 1, &specular_colour[0]);

	shininess = 8.f;
	glUniform1fv(shininess_id, 1, &shininess);

	light_src.drawSphere(0);
}

void Light::translateX(std::function<float(float, float)> op) {}
void Light::translateY(std::function<float(float, float)> op) {}
void Light::translateZ(std::function<float(float, float)> op) {}