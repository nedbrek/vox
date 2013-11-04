#include "chunk.h"
#include <osg/PositionAttitudeTransform>

Chunk::Chunk(unsigned char *blockIds, int x, int y, int z)
: blockIds_(blockIds)
, x_(x)
, y_(y)
, z_(z)
{
}

osg::Group* Chunk::makeDumbChunk(osg::Node *stoneGeode)
{
	osg::Group *chunk = new osg::Group;
	osg::PositionAttitudeTransform *transform = NULL;
	for (unsigned x = 0; x < 16; ++x)
	for (unsigned y = 0; y < 16; ++y)
	for (unsigned z = 0; z < 16; ++z)
	if (blockIds_[index(x, y, z)] == 1)
	{
		transform = new osg::PositionAttitudeTransform;
		transform->setPosition(osg::Vec3(x_ * 16 + x, y_ * 16 + y, z_ * 16 + z));
		transform->addChild(stoneGeode);
		chunk->addChild(transform);
	}

	return chunk;
}

unsigned Chunk::index(unsigned x, unsigned y, unsigned z)
{
	return x + y * 16 + z * 256;
}

