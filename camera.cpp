#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
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

glm::mat4 Camera::projection() const
{
	return glm::perspective(fov_, aspectRatio_,
	  nearClipPlane_, farClipPlane_);
}

glm::mat4 Camera::view() const
{
	return glm::lookAt(
	  position(),
	  glm::vec3(tX_, tY_, tZ_), // target xyz
	  headVec_
	);
}

glm::vec3 Camera::position() const
{
	return glm::vec3(x_, y_, z_);
}

void Camera::setPosition(const glm::vec3 &pos)
{
	x_ = pos[0];
	y_ = pos[1];
	z_ = pos[2];
}

void Camera::setTargetPosition(const glm::vec3 &v)
{
	tX_ = v[0];
	tY_ = v[1];
	tZ_ = v[2];
}

void Camera::setHeadVec(const glm::vec3 &v)
{
	headVec_ = v;
}

