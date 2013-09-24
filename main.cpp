#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

// wrapper for matrix operations related to the view
class Camera
{
public:
	Camera()
	: x_(-1)
	, y_(-1)
	, z_(2)
	, tX_(0)
	, tY_(0)
	, tZ_(0)
	, headRay_(1)
	, fov_(90)
	, aspectRatio_(16./9)
	, nearClipPlane_(.1)
	, farClipPlane_(100)
	{
	}

	glm::mat4 projection()
	{
		return glm::perspective(fov_, aspectRatio_,
		  nearClipPlane_, farClipPlane_);
	}

	glm::mat4 view()
	{
		return glm::lookAt(
		  glm::vec3(x_, y_, z_), // eye position
		  glm::vec3(tX_, tY_, tZ_), // target xyz
		  glm::vec3(0, headRay_, 0) // head tilt
		);
	}

protected:
	// pointing angle
	float x_, y_, z_; // position
	float tX_, tY_, tZ_; // target position

	float headRay_; // 1 is up, -1 is down

	// projection state
	float fov_; // field of view

	float aspectRatio_;
	float nearClipPlane_, farClipPlane_;
};

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

	Camera c;
	glLoadMatrixf(glm::value_ptr(c.projection()));
	glMultMatrixf(glm::value_ptr(c.view()));

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

