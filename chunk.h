#ifndef CHUNK_H
#define CHUNK_H

class Resources;
namespace osg
{
	class Group;
	class Node;
}

/// a 3d group of blocks (16 cubed)
class Chunk
{
public:
	Chunk(unsigned char *blockIds, int x, int y, int z);

	///@return the index of a block within a chunk
	// (0, 0, 0) is the lower left front block
	// (15, 15, 15) is the upper right back block
	static
	unsigned index(unsigned x, unsigned y, unsigned z);

	osg::Group* makeDumbChunk(osg::Node *stoneGeode);
	osg::Group* makeChunkMesh(Resources &r);

private:
	unsigned char *blockIds_;
	int x_, y_, z_;
};

#endif

