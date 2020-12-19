#include "shader.h"


Shader::Shader() {
	program = std::numeric_limits<GLuint>::max();
}

Shader::Shader(std::vector<std::string> file_paths) {
	std::cout << file_paths.size() << std::endl;
	try {
		if (file_paths.size() > 2)
			program = load_shader(file_paths[0].c_str(), file_paths[1].c_str(), file_paths[2].c_str());
		else if (file_paths.size() <= 2)
			program = load_shader(file_paths[0].c_str(), file_paths[1].c_str());
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}
}

Shader::~Shader() {

}

void Shader::use(int i) {
	if (i == 1)
		glUseProgram(program);
	else if (i == 0)
		glUseProgram(0);
}

void Shader::send_model(glm::mat4& model) {
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, &model[0][0]);

}

void Shader::send_view(glm::mat4& view) {
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);

}

void Shader::send_projection(glm::mat4& proj) {
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &proj[0][0]);
}

void Shader::send_normal_transformation(glm::mat3& normal_transformation) {
	glUniformMatrix3fv(glGetUniformLocation(program, "normal_transformation"), 1, GL_FALSE, &normal_transformation[0][0]);
}

void Shader::send_lightpos(glm::vec4& lightpos) {
	glUniform4fv(glGetUniformLocation(program, "lightpos"), 1, &lightpos[0]);
}

void Shader::send_ambient_colour(glm::vec4& ambient_colour) {
	glUniform4fv(glGetUniformLocation(program, "ambient_colour"), 1, &ambient_colour[0]);

}

void Shader::send_specular_colour(glm::vec4& specular_colour) {
	glUniform4fv(glGetUniformLocation(program, "specular_colour"), 1, &specular_colour[0]);
}

void Shader::send_shininess(GLfloat& shininess) {
	glUniform1f(glGetUniformLocation(program, "shininess"), shininess);
}

void Shader::send_attenuationmode(GLuint& attenuationmode) {
	glUniform1ui(glGetUniformLocation(program, "attenuationmode"), attenuationmode);
}

void Shader::send_emitmode(GLuint& emitmode) {
	glUniform1ui(glGetUniformLocation(program, "emitmode"), emitmode);
}

void Shader::send_time(float time) {
	glUniform1f(glGetUniformLocation(program, "time"), time);
}

//void Shader::send_texture(GLenum tex_type) {
//	if (tex_type == GL_TEXTURE_2D) {
//
//	}
//	else if (tex_type == GL_TEXTURE_CUBE_MAP) {
//		uids.skybox = glGetUniformLocation(program, "skybox");
//		std::cout << uids.skybox << std::endl;
//		if (uids.skybox > 0) glUniform1i(uids.skybox, 0);
//	}
//	uids.tex1 = glGetUniformLocation(program, "tex1");
//	if (uids.tex1 > 0) glUniform1i(uids.tex1, 0);
//}

/* Build shaders from strings containing shader source code */
GLuint Shader::build_shader(GLenum eShaderType, const std::string& shaderText)
{
	GLuint shader = glCreateShader(eShaderType);
	const char* strFileData = shaderText.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Output the compile errors

		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char* strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		std::cerr << "Compile error in " << strShaderType << "\n\t" << strInfoLog << std::endl;
		delete[] strInfoLog;

		throw std::exception("Shader compile exception");
	}

	return shader;
}

/* Read a text file into a string*/
std::string Shader::read_file(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

/* Load vertex and fragment shader and return the compiled program */
GLuint Shader::load_shader(const char* vertex_path, const char* fragment_path,
	const char* geometry_path)
{
	GLuint vertShader, fragShader, geomShader;

	// Read shaders
	std::string vertShaderStr = read_file(vertex_path);
	std::string fragShaderStr = read_file(fragment_path);

	std::string geomShaderStr;

	if (geometry_path != NULL)	std::cout << "YAYYY" << std::endl;

	if (geometry_path != NULL)	geomShaderStr = read_file(geometry_path);

	GLint result = GL_FALSE;
	int logLength;

	vertShader = build_shader(GL_VERTEX_SHADER, vertShaderStr);
	if (geometry_path != NULL) geomShader = build_shader(GL_GEOMETRY_SHADER, geomShaderStr);
	fragShader = build_shader(GL_FRAGMENT_SHADER, fragShaderStr);
	

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	if (geometry_path != NULL)	glAttachShader(program, geomShader);
	glAttachShader(program, fragShader);
	
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	if (geometry_path != NULL)	glDeleteShader(geomShader);
	glDeleteShader(fragShader);
	

	return program;
}

/* Load vertex and fragment shader and return the compiled program */
GLuint Shader::build_shader_program(std::string vertShaderStr, std::string fragShaderStr)
{
	GLuint vertShader, fragShader;
	GLint result = GL_FALSE;

	try
	{
		vertShader = build_shader(GL_VERTEX_SHADER, vertShaderStr);
		fragShader = build_shader(GL_FRAGMENT_SHADER, fragShaderStr);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		throw std::exception("BuildShaderProgram() Build shader failure. Abandoning");
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{

		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		std::cerr << "Linker error: " << strInfoLog << std::endl;

		delete[] strInfoLog;
		throw std::runtime_error("Shader could not be linked.");
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}