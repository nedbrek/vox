#include <GL/glew.h>
#include <cassert>

unsigned char* makeColorTexture(unsigned numPixels)
{
	assert(numPixels % 4 == 0);
	unsigned char *buf= new unsigned char[numPixels];
	for(unsigned i = 0; i < numPixels; ++i)
	{
		buf[i] = 255; ++i; // red
		buf[i] = 250; ++i; // green
		buf[i] =   0; ++i; // blue
		buf[i] = 127; // alpha
	}
	return buf;
}

void drawSceneQuad()
{
	glBegin(GL_QUADS);

	// bottom
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glVertex3d(1, 1, 0);
	glVertex3d(0, 1, 0);

	// top
	glVertex3d(0, 0, 1);
	glVertex3d(1, 0, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(0, 1, 1);

	glEnd();
}

