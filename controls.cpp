#include "controls.h"
#include "camera.h"
#include <GL/glfw.h>

namespace
{
	const float mouseSpeed = 0.005;
	const float speed = 3; // units per second
}

//----------------------------------------------------------------------------
Controls::Controls()
: lastTime_(glfwGetTime())
, windowWidth_(1092)
, windowHeight_(614)
, framesSinceLastInterval_(0)
, fps_(0)
{
	lastInterval_ = lastTime_;
	glfwDisable(GLFW_MOUSE_CURSOR);
}

void Controls::beginFrame(Camera *cp)
{
	const double curTime = glfwGetTime();
	const double deltaT = curTime - lastTime_;

	const double deltaInterval = curTime - lastInterval_;
	if (deltaInterval > 1)
	{
		fps_ = framesSinceLastInterval_ / deltaInterval;
		framesSinceLastInterval_ = 0;
		lastInterval_ = curTime;
	}

	++framesSinceLastInterval_;

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

