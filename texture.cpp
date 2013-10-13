#include <GL/glew.h>
#include "texture.h"
#include "utils.h"
#include <iostream>
#include <memory>

Texture::Texture(const char *filename)
: textureId_(0)
{
	// create texture
	// 1k pix, 4 B per pix
	//std::auto_ptr<unsigned char> textureBuf(makeColorTexture(32*32*4));
	int width = 0, height = 0;
	bool hasAlpha = false;
	std::auto_ptr<unsigned char> textureBuf(
	  loadPngTexture(filename, &width, &height, &hasAlpha)
	);
	const GLenum format = hasAlpha ? GL_RGBA : GL_RGB;

	// send it to OpenGL
	glGenTextures(1, &textureId_);
	glBindTexture(GL_TEXTURE_2D, textureId_);

	// our texture is 32 by 32 unnormalized integer RGBA data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
	  GL_UNSIGNED_BYTE, textureBuf.get());
	glGenerateMipmap(GL_TEXTURE_2D);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cerr << "Error creating texture: " << filename << std::endl;
	}
}

void Texture::makeActive()
{
	glBindTexture(GL_TEXTURE_2D, textureId_);
}

