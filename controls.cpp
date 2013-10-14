#include "controls.h"
#include "camera.h"
#include <GL/glfw.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

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
, horizontalAngle_(3.14f)
, verticalAngle_(0)
, framesSinceLastInterval_(0)
, fps_(0)
, lastDX_(0)
, lastDY_(0)
{
	lastInterval_ = lastTime_;
	glfwDisable(GLFW_MOUSE_CURSOR);

	// track key down events until we get around to them
	glfwEnable(GLFW_STICKY_KEYS);
}

std::string Controls::lastMouseDXY() const
{
	std::ostringstream os;
	os << '(' << lastDX_ << ',' << lastDY_ << ')';
	return os.str();
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
	glfwSetMousePos(windowWidth_/2, windowHeight_/2);

	// compute new orientation
	const float deltaX = float(windowWidth_/2 - mouseX);
	float deltaY = float(windowHeight_/2 - mouseY);
	if (abs(deltaY) <= 1)
		deltaY = 0;
	lastDX_ = deltaX;
	lastDY_ = deltaY;

	horizontalAngle_ += mouseSpeed * deltaX;
	verticalAngle_   += mouseSpeed * deltaY;

	// direction : spherical to cartesian coordinate conversion
	const glm::vec3 direction(
	  cos(verticalAngle_) * sin(horizontalAngle_),
	  sin(verticalAngle_),
	  cos(verticalAngle_) * cos(horizontalAngle_)
	);

	// Right vector
	const glm::vec3 right = glm::vec3(
	  sin(horizontalAngle_ - 3.14f/2.0f),
	  0,
	  cos(horizontalAngle_ - 3.14f/2.0f)
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

	// clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// build MVP matrix
	glLoadMatrixf(glm::value_ptr(cp->projection()));
	glMultMatrixf(glm::value_ptr(cp->view()));
}

