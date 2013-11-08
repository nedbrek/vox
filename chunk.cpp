#include "chunk.h"
#include "resources.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osg/TexEnv>
#include <osg/Texture2D>

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

osg::Group* Chunk::makeChunkMesh(Resources &r)
{
	osg::Group *chunk = new osg::Group;
	osg::PositionAttitudeTransform *transform = new osg::PositionAttitudeTransform;
	chunk->addChild(transform);

	transform->setPosition(osg::Vec3(x_ * 16, y_ * 16, z_ * 16));

	osg::Geometry *geometry = new osg::Geometry;
	osg::Vec3Array *cubeVerts = new osg::Vec3Array;
	cubeVerts->push_back(osg::Vec3(16, 16, 1)); // 0
	cubeVerts->push_back(osg::Vec3( 0, 16, 1)); // 1
	cubeVerts->push_back(osg::Vec3(16, 16, 0)); // 2
	cubeVerts->push_back(osg::Vec3( 0, 16, 0)); // 3
	cubeVerts->push_back(osg::Vec3(16,  0, 1)); // 4
	cubeVerts->push_back(osg::Vec3( 0,  0, 1)); // 5
	cubeVerts->push_back(osg::Vec3( 0,  0, 0)); // 6
	cubeVerts->push_back(osg::Vec3(16,  0, 0)); // 7

	geometry->setVertexArray(cubeVerts);

	osg::DrawElementsUByte *cubeIndexes = new osg::DrawElementsUByte(
	  osg::PrimitiveSet::QUADS, 0);
	// bottom face
	cubeIndexes->push_back(6); // 000
	cubeIndexes->push_back(3); // 010
	cubeIndexes->push_back(2); // 110
	cubeIndexes->push_back(7); // 100

	// top face
	cubeIndexes->push_back(5); // 001
	cubeIndexes->push_back(4); // 101
	cubeIndexes->push_back(0); // 111
	cubeIndexes->push_back(1); // 011

	/*
	cubeVerts_->push_back(osg::Vec3(1, 1, 1)); // 0
	cubeVerts_->push_back(osg::Vec3(0, 1, 1)); // 1
	cubeVerts_->push_back(osg::Vec3(1, 1, 0)); // 2
	cubeVerts_->push_back(osg::Vec3(0, 1, 0)); // 3
	cubeVerts_->push_back(osg::Vec3(1, 0, 1)); // 4
	cubeVerts_->push_back(osg::Vec3(0, 0, 1)); // 5
	cubeVerts_->push_back(osg::Vec3(0, 0, 0)); // 6
	cubeVerts_->push_back(osg::Vec3(1, 0, 0)); // 7
	 */

	geometry->addPrimitiveSet(cubeIndexes);

	osg::Geode *geode = new osg::Geode;
	geode->addDrawable(geometry);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, &r.textureFromId(1));
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, new osg::TexEnv(osg::TexEnv::BLEND));
	transform->addChild(geode);

	return chunk;
}

unsigned Chunk::index(unsigned x, unsigned y, unsigned z)
{
	return x + y * 16 + z * 256;
}

