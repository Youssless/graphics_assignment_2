#pragma once

#include "stb_image.h"
#include <stdexcept>
#include "common.h"
#include <iostream>
#include <vector>


class Texture {
public:
	static void load(const char* file_path, GLuint &texid, bool gen_mipmaps);
	static void load_cube_map(std::vector<std::string> file_paths, GLuint &texid);
	static void bind_texture(const GLuint& texid);
	static void unbind_texture();

	static void bind_cube_map(const GLuint& texid);
	static void unbind_cube_map();
};