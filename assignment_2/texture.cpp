#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

/*
*	load a single 2D texture
*	params:
*		const char* file_path : file path of the texture image
*		GLuint &texid : texture id to generate and bind to
*		bool gen_mipmaps : enable or disable mipmaps
*/
void Texture::load(const char* file_path, GLuint &texid, bool gen_mipmaps) {
	int width, height, nr_channels;
	glGenTextures(1, &texid);

	unsigned char* data = stbi_load(file_path, &width, &height, &nr_channels, 0);

	if (data) {
		int pixel_format = 0;
		if (nr_channels == 3)
			pixel_format = GL_RGB;
		else
			pixel_format = GL_RGBA;

		// bind before glTexImage2D
		bind_texture(texid);

		// create texture with attributes
		// 2nd param defines the level
		glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, data);

		// free the data after the texture is created
		stbi_image_free(data);

		if (gen_mipmaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			// Texture params -> needed for each unique texture
			{
				// these are called texture params, controls how the texture is rendered. If not
				//	defined the texture will not render

				// MIN_FILTER = controls how the texture will look when the pixels are larger than the pixel fragments
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				// MAG_FILTER = controls how the texture will look when the pixels are smaller than the pixel fragments
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				// GL_TEXTURE_WRAP_S(_AND_T) controls how the texture s/t (s = local x, t = local y texture coordinates) is smaller than the
				//	polygon is repeated or stretched
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
		}
	}
	else {
		// free the data after the texture is created
		stbi_image_free(data);
		throw std::exception("File not found");
	}
}

/*
*	load multiple texture images for a cube map
*	params:
*		std::vector<std::string> file_paths : file paths for the cube map, need 6 images for the cube map
*		GLuint &texid : texture id to generate and bind to
*/
void Texture::load_cube_map(std::vector<std::string> file_paths, GLuint& texid) {
	int width, height, nr_channels;
	glGenTextures(1, &texid);
	bind_cube_map(texid);

	for (int i = 0; i < file_paths.size(); i++) {
		unsigned char* data = stbi_load(file_paths[i].c_str(), &width, &height, &nr_channels, 0);

		if (data) {
			// create texture with attributes
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			// free the data after the texture is created
			stbi_image_free(data);	
		}
		else {
			// free the data after the texture is created
			stbi_image_free(data);
			throw std::exception("File not found");
		}
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	/*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
}

/*
*	bind a 2D texture 
*	params:
*		GLuint &texid : texture id to bind to 
*/
void Texture::bind_texture(const GLuint &texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
}

/*
*	unbind a 2D texture
*	params:
*/
void Texture::unbind_texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
*	bind a cube map texture
*	params:
*		GLuint &texid : texture id to bind to
*/
void Texture::bind_cube_map(const GLuint& texid) {
	glBindTexture(GL_TEXTURE_CUBE_MAP, texid);
}

/*
*	unbind a cube map texture
*	params:
*/
void Texture::unbind_cube_map() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}