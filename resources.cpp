#include "resources.h"
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <cassert>

/*
static
unsigned char* makeColorTexture(unsigned numBytes)
{
	assert(numBytes % 4 == 0);
	unsigned char *buf= new unsigned char[numBytes];
	for(unsigned i = 0; i < numBytes; ++i)
	{
		buf[i] = 255; ++i; // red
		buf[i] = 250; ++i; // green
		buf[i] =   0; ++i; // blue
		buf[i] = 127; // alpha
	}
	return buf;
}
*/

Resources::Resources()
{
	cubeVerts_ = new osg::Vec3Array;
	cubeVerts_->push_back(osg::Vec3(1, 1, 1)); // 0
	cubeVerts_->push_back(osg::Vec3(0, 1, 1)); // 1
	cubeVerts_->push_back(osg::Vec3(1, 1, 0)); // 2
	cubeVerts_->push_back(osg::Vec3(0, 1, 0)); // 3
	cubeVerts_->push_back(osg::Vec3(1, 0, 1)); // 4
	cubeVerts_->push_back(osg::Vec3(0, 0, 1)); // 5
	cubeVerts_->push_back(osg::Vec3(0, 0, 0)); // 6
	cubeVerts_->push_back(osg::Vec3(1, 0, 0)); // 7

	textures_.push_back(new osg::Texture2D()); // air

	osg::Texture2D *stoneTex = new osg::Texture2D;
	stoneTex->setImage(osgDB::readImageFile("textures/blocks/stone.png"));
	stoneTex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	stoneTex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	textures_.push_back(stoneTex);

	//std::auto_ptr<unsigned char> textureBuf(makeColorTexture(32*32*4));
	//textures_.push_back(new Texture(textureBuf.get(), 32, 32, true));
	//textures_.push_back(new Texture("textures/blocks/glass.png"));
}

Resources::~Resources()
{
	//for(std::vector<Texture*>::const_iterator i = textures_.begin(); i != textures_.end(); ++i)
		//delete *i;
}

osg::Texture2D& Resources::textureFromId(unsigned char id)
{
	if (id > textures_.size())
		return *textures_[0];
	return *textures_[id];
}

