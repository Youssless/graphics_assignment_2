#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"


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
		glBindTexture(GL_TEXTURE_2D, texid);

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

void Texture::bind_texture(const GLuint &texid) {
	glBindTexture(GL_TEXTURE_2D, texid);
}

void Texture::unbind_texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}