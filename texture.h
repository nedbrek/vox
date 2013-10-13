#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

class Texture
{
public:
	Texture(const char *filename);

	void makeActive();

protected:
	GLuint textureId_;
};
#endif

