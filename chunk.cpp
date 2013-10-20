#include <GL/glew.h>
#include "chunk.h"
#include "resources.h"
#include "texture.h"

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

void Chunk::render(Resources &r)
{
	glUseProgram(r.mainShader());

	const int chunkX = x_ * 16;
	const int chunkY = y_ * 16;
	const int chunkZ = z_ * 16;

	// foreach block
	for(unsigned i = 0; i < 4096; ++i)
	{
		const unsigned char blockId = blockIds_[i];

		// 0 is empty space
		if (blockId == 0)
			continue;

		// make the texture active
		glActiveTexture(GL_TEXTURE0);
		r.textureFromId(blockId).makeActive();

		// update uniform intra-chunk block x/y/z
		const int x = chunkX + (i & 15);
		const int y = chunkY + ((i >> 4) & 15);
		const int z = chunkZ + ((i >> 8) & 15);
		glUniform3i(r.uniformBlockLocation(), x, y, z);

		// render block
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

