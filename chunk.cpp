#include "chunk.h"
#include <GL/gl.h>

Chunk::Chunk(unsigned char *blockIds, int x, int y, int z)
: blockIds_(blockIds)
, x_(x)
, y_(y)
, z_(z)
{
}

unsigned Chunk::index(unsigned x, unsigned y, unsigned z)
{
	return x + y * 16 + z * 256;
}

void Chunk::render()
{
	// TODO: update uniform chunk x/y/z

	// foreach block
	for(unsigned i = 0; i < 4096; ++i)
	{
		// 0 is empty space
		if (blockIds_[i] == 0)
			continue;

		// TODO: update uniform intra-chunk x/y/z
		// render block (TODO: select texture)
		// TODO: vertex buffer
		glBegin(GL_TRIANGLE_STRIP);

		glVertex3f(0, 1, 0); // 4
		glVertex3f(1, 1, 0); // 3
		glVertex3f(0, 0, 0); // 7
		glVertex3f(1, 0, 0); // 8
		glVertex3f(1, 0, 1); // 5
		glVertex3f(1, 1, 0); // 3
		glVertex3f(1, 1, 1); // 1
		glVertex3f(0, 1, 0); // 4
		glVertex3f(0, 1, 1); // 2
		glVertex3f(0, 0, 0); // 7
		glVertex3f(0, 0, 1); // 6
		glVertex3f(1, 0, 1); // 5
		glVertex3f(0, 1, 1); // 2
		glVertex3f(1, 1, 1); // 1

		glEnd();
	}
}

