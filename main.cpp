#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

	glm::mat4 projection = glm::perspective(90.f, 16.f/9.f, .1f, 100.f);
	glm::mat4 view = glm::lookAt(
	  glm::vec3(-1, -1, 2), // eye position
	  glm::vec3(0, 0, 0), // target xyz
	  glm::vec3(0, 1, 0) // head up (-1 for inverted)
	);

	glLoadMatrixf(glm::value_ptr(projection));
	glMultMatrixf(glm::value_ptr(view));

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

