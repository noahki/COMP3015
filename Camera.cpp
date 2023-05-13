#include "Camera.h"

Camera::Camera()
{
	camera_pos = glm::vec3(0.0f, 1.0f, 3.0f);
	camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera_right = glm::normalize(camera_front);
}

glm::mat4 Camera::get_camera_transformation() {
	return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

void Camera::apply_mouse_movements(float xDifference, float yDifference) {
	float sensitivity = 0.5f;
	xDifference *= sensitivity;
	yDifference *= sensitivity;

	yaw += xDifference;
	pitch += yDifference;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front_vector;

	front_vector.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_vector.y = sin(glm::radians(pitch));
	front_vector.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	
	camera_front = glm::normalize(front_vector);
	camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0.0f, 1.0f, 0.0f)));
	camera_up = glm::normalize(glm::cross(camera_right, camera_front));

}