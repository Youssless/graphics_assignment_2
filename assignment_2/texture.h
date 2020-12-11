#pragma once

#include "stb_image.h"
#include <stdexcept>
#include "common.h"


class Texture {
public:
	static void load(const char* file_path, GLuint &texid, bool gen_mipmaps);
	static void bind_texture(const GLuint& texid);
	static void unbind_texture();
};