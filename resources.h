#ifndef RESOURCES_H
#define RESOURCES_H

#include <GL/gl.h>
#include <vector>
class FTFont;
class Texture;

//----------------------------------------------------------------------------
class Resources
{
public:
	Resources();
	~Resources();

	FTFont& font();
	GLuint mainShader();
	GLint uniformBlockLocation();

	Texture& textureFromId(unsigned char id);

protected:
	FTFont *font_;
	GLuint shaderProgram_;
	GLint uniformBlockLoc_;
	std::vector<Texture*> textures_;
};
#endif

