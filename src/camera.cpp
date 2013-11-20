#include "camera.h"

Camera::Camera()
{
}

Camera::Camera(vec3 posv, vec3 viewDir, vec3 upv) :
	position(posv),
	view(viewDir),
	up(upv)
{
}

mat4 Camera::getView()
{
	return lookAt(position, vec3(0), up);
}
