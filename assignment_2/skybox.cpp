#include "skybox.h"

/*
* skybox constructor
* params:
*	const GLuint &program : shader program for the skybox
*/
Skybox::Skybox(const GLuint &program) {
    // initialise file paths for the cubemap
	std::vector<std::string> file_paths = {
		"..\\textures\\cubemap\\right.jpg",
		"..\\textures\\cubemap\\left.jpg",
		"..\\textures\\cubemap\\top.jpg",
		"..\\textures\\cubemap\\bottom.jpg",
		"..\\textures\\cubemap\\front.jpg",
		"..\\textures\\cubemap\\back.jpg",
	};
    
    // load cubemap texures
	try {
		texture.load_cube_map(file_paths, texid);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	int loc = glGetUniformLocation(program, "skybox");
	if (loc > 0) glUniform1i(loc, 0);
    
    // set the vao to 0 to reference the location in the shader
    vao = 0;
}

/*
* initialise the vertices and memory for the cube 
* params:
*/
void Skybox::create() {
    // skybox positions
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

    // generate the memory for the vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox() {}

/*
* draws the skybox with the textures
* params:
*/
void Skybox::display() {
    // enable the vertex attrib and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // bind the textures of the cubemap and draw the cube
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE0);
    texture.bind_cube_map(texid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    texture.unbind_cube_map();
}