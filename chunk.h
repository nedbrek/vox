#ifndef CHUNK_H
#define CHUNK_H

/// a 3d group of blocks (16 cubed)
class Chunk
{
public:
	Chunk(unsigned char *blockIds, int x, int y, int z);

	void render(int uniformBlock);

	///@return the index of a block within a chunk
	// (0, 0, 0) is the lower left front block
	// (15, 15, 15) is the upper right back block
	static
	unsigned index(unsigned x, unsigned y, unsigned z);

private:
	unsigned char *blockIds_;
	int x_, y_, z_;
};

#endif

