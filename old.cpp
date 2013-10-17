#include <GL/glew.h>

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

