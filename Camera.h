#pragma once

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::vec3 camera_pos;
	glm::vec3 camera_front;
	glm::vec3 camera_up;
	glm::vec3 camera_right;

	float yaw = -90.0f;
	float pitch = 0.0f;

	Camera();

	glm::mat4 get_camera_transformation();

	void apply_mouse_movements(float xDifference, float yDifference);

private:

};

