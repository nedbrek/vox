#include <GL/glfw.h>
#include <cstdio>

void drawScene()
{
	glBegin(GL_QUADS);
	glColor3d(1, 0, 0);

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

int main(int argc, char **argv)
{
	// initialize GLFW
	int rc = glfwInit();
	if (rc != GL_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return 1;
	}

	// open main window
	rc = glfwOpenWindow(1092, 614, 8, 8, 8, 8, 32, 0, GLFW_WINDOW);
	if (rc != GL_TRUE)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return 1;
	}

	bool running = true;
	while (running)
	{
		// clear screen
		glClear(GL_COLOR_BUFFER_BIT);

		drawScene();

		// swap double buffers
		glfwSwapBuffers();

		// keep running until the user hits ESC or closes the main window
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	// cleanup
	glfwTerminate();
	return 0;
}

