#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "common.h"

class Shader {
public:
	Shader();
	Shader(std::vector<std::string> file_paths);
	~Shader();

	void use(int i);

	GLuint build_shader(GLenum eShaderType, const std::string& shaderText);
	std::string read_file(const char* filePath);
	GLuint load_shader(const char* vertex_path, const char* fragment_path);
	GLuint build_shader_program(std::string vertShaderStr, std::string fragShaderStr);

	void send_model(glm::mat4& model);
	void send_view(glm::mat4& view);
	void send_projection(glm::mat4& proj);
	void send_normal_transformation(glm::mat3& normal_transformation);
	void send_lightpos(glm::vec4& lightpos);
	void send_ambient_colour(glm::vec4& ambient_colour);
	void send_specular_colour(glm::vec4& specular_colour);
	void send_shininess(GLfloat& shininess);
	void send_attenuationmode(GLuint& attenuationmode);
	void send_emitmode(GLuint& emitmode);
	void send_texture(GLenum tex_type);

	GLuint program;
private:
	
};