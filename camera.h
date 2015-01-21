#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

/// wrapper for matrix operations related to the view
class Camera
{
public:
	Camera();

	///@return matrix for 2d projection to window space
	glm::mat4 projection() const;

	///@return matrix for projection to view space
	glm::mat4 view() const;

	///@return camera position (world space)
	glm::vec3 position() const;

	///@return camera direction
	glm::vec3 direction() const;

	/// set camera position (world space)
	void setPosition(const glm::vec3 &pos);

	glm::vec3 targetPosition() const;

	/// set camera target position (world space)
	void setTargetPosition(const glm::vec3 &v);

	/// set the head angle
	void setHeadVec(const glm::vec3 &v);

protected:
	// pointing angle
	float x_, y_, z_; /// position
	float tX_, tY_, tZ_; /// target position

	glm::vec3 headVec_; /// head tilt

	// projection state
	float fov_; /// field of view

	float aspectRatio_; /// screen ratio of width / height
	float nearClipPlane_, farClipPlane_; /// shouldn't need to change these...
};

#endif

