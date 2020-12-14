#include "skybox.h"
#include <iostream>


Skybox::Skybox(const GLuint &program) {
	std::vector<std::string> file_paths = {
		"..\\textures\\cubemap\\right.jpg",
		"..\\textures\\cubemap\\left.jpg",
		"..\\textures\\cubemap\\top.jpg",
		"..\\textures\\cubemap\\bottom.jpg",
		"..\\textures\\cubemap\\front.jpg",
		"..\\textures\\cubemap\\back.jpg",
	};

    std::cout << "texid: " << texid << std::endl;
	try {
		texture.load_cube_map(file_paths, texid);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	int loc = glGetUniformLocation(program, "skybox");
	if (loc > 0) glUniform1i(loc, 0);

    std::cout << "texid: " << texid << std::endl;
    vao = 0;
}

void Skybox::create() {
	float vertices[] = {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox() {}

void Skybox::display() {
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE0);
    texture.bind_cube_map(texid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    texture.unbind_cube_map();
}