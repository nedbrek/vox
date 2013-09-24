#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

namespace
{
	const float mouseSpeed = 0.005;
	const float speed = 3; // units per second
}

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
	, headVec_(0, 1, 0)
	, fov_(90)
	, aspectRatio_(16./9)
	, nearClipPlane_(.1)
	, farClipPlane_(100)
	{
	}

	glm::mat4 projection() const
	{
		return glm::perspective(fov_, aspectRatio_,
		  nearClipPlane_, farClipPlane_);
	}

	glm::mat4 view() const
	{
		return glm::lookAt(
		  position(),
		  glm::vec3(tX_, tY_, tZ_), // target xyz
		  headVec_
		);
	}

	glm::vec3 position() const
	{
		return glm::vec3(x_, y_, z_);
	}

	void setPosition(const glm::vec3 &pos)
	{
		x_ = pos[0];
		y_ = pos[1];
		z_ = pos[2];
	}

	void setTargetPosition(const glm::vec3 &v)
	{
		tX_ = v[0];
		tY_ = v[1];
		tZ_ = v[2];
	}

	void setHeadVec(const glm::vec3 &v)
	{
		headVec_ = v;
	}

protected:
	// pointing angle
	float x_, y_, z_; // position
	float tX_, tY_, tZ_; // target position

	glm::vec3 headVec_; // head tilt

	// projection state
	float fov_; // field of view

	float aspectRatio_;
	float nearClipPlane_, farClipPlane_;
};

class Controls
{
public:
	Controls()
	: lastTime_(glfwGetTime())
	, windowWidth_(1092)
	, windowHeight_(614)
	{
		glfwDisable(GLFW_MOUSE_CURSOR);
	}

	void beginFrame(Camera *cp)
	{
		const double curTime = glfwGetTime();
		const double deltaT = curTime - lastTime_;

		// find amount of mouse motion since last frame
		int mouseX, mouseY;
		glfwGetMousePos(&mouseX, &mouseY);

		// compute new orientation
		const float deltaX = float(windowWidth_/2 - mouseX);
		const float deltaY = float(windowHeight_/2 - mouseY);

		const float horizontalAngle = 3.14f + mouseSpeed * deltaX;
		const float verticalAngle   = mouseSpeed * deltaY;

		// direction : spherical to cartesian coordinate conversion
		const glm::vec3 direction(
		  cos(verticalAngle) * sin(horizontalAngle),
		  sin(verticalAngle),
		  cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		const glm::vec3 right = glm::vec3(
		  sin(horizontalAngle - 3.14f/2.0f),
		  0,
		  cos(horizontalAngle - 3.14f/2.0f)
		);

		// Up vector
		const glm::vec3 up = glm::cross(right, direction);

		glm::vec3 position = cp->position();
		// update position
		if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
		{
			position[1] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			position[1] -= deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			position[0] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			position[0] -= deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_PAGEUP) == GLFW_PRESS)
		{
			position[2] += deltaT * speed;
		}
		if (glfwGetKey(GLFW_KEY_PAGEDOWN) == GLFW_PRESS)
		{
			position[2] -= deltaT * speed;
		}

		cp->setPosition(position);
		cp->setTargetPosition(position + direction);
		cp->setHeadVec(up);

		lastTime_ = curTime;
	}

protected:
	double lastTime_;
	int windowWidth_, windowHeight_;
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

	Camera camera;
	// track key down events until we get around to them
	glfwEnable(GLFW_STICKY_KEYS);

	Controls ctl;
	bool running = true;
	while (running)
	{
		ctl.beginFrame(&camera);

		// build MVP matrix
		glLoadMatrixf(glm::value_ptr(camera.projection()));
		glMultMatrixf(glm::value_ptr(camera.view()));

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

