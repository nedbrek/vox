#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

class Texture
{
public:
	Texture(const char *filename);
	Texture(const unsigned char *bytes, int w, int h, bool alpha);

	void makeActive();

protected: // methods
	bool init(const unsigned char *bytes, int w, int h, bool alpha);

protected: // data
	GLuint textureId_;
};
#endif

