#include <GL/glew.h>
#include "resources.h"
#include "shader.h"
#include "texture.h"
#include <FTGL/ftgl.h>
#include <iostream>

Resources::Resources()
{
	font_ = new FTGLPixmapFont("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf");
	if (font_->Error())
	{
		std::cerr << "Error opening font" << std::endl;
	}
	else
		font_->FaceSize(12);

	shaderProgram_ = makeShaderProgram();
	uniformBlockLoc_ = glGetUniformLocation(shaderProgram_, "blockOffset");

	textures_.push_back(new Texture("")); // air
	textures_.push_back(new Texture("textures/blocks/stone.png"));
}

Resources::~Resources()
{
	delete font_;
	for(std::vector<Texture*>::const_iterator i = textures_.begin(); i != textures_.end(); ++i)
		delete *i;
}

FTFont& Resources::font()
{
	return *font_;
}

GLuint Resources::mainShader()
{
	return shaderProgram_;
}

GLint Resources::uniformBlockLocation()
{
	return uniformBlockLoc_;
}

Texture& Resources::textureFromId(unsigned char id)
{
	return *textures_[id];
}

