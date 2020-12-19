#include "skybox.h"

/*
* skybox constructor
* params:
*	const GLuint &program : shader program for the skybox
*/
Skybox::Skybox() {
    // set the vao to 0 to reference the location in the shader
    vao = 0;
}

void Skybox::set_shader(Shader& shader) {
    this->shader = shader;
}

/*
* initialise the vertices and memory for the cube 
* params:
*/
void Skybox::create() {
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
        Texture::load_cube_map(file_paths, texid);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    int loc = glGetUniformLocation(shader.program, "skybox");
    if (loc > 0) glUniform1i(loc, 0);

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
void Skybox::display(float aspect_ratio) {
    // enable the vertex attrib and set attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(vao);
    glVertexAttribPointer(vao, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glm::mat4 projection = glm::perspective(glm::radians(30.0f), aspect_ratio, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, -1.5f),
        glm::vec3(0.0f, 0.0f, -1.f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    shader.send_projection(projection);
    shader.send_view(view);
    

    // bind the textures of the cubemap and draw the cube
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glActiveTexture(GL_TEXTURE0);
    Texture::bind_cube_map(texid);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    Texture::unbind_cube_map();
}