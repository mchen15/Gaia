#include "camera.h"
#include <glm/gtc/matrix_projection.hpp>

Camera::Camera()
{
}

Camera::Camera(vec3 posv, vec3 lp, vec3 upv,float fieldOfView,float np, float fp) :
	position(posv),
	up(upv),
	fov(fieldOfView),
	nearp(np),
	farp(fp),
	lookAtPoint(lp)
{
}

mat4 Camera::getView()
{
	return lookAt(position, lookAtPoint, up);
}


float Camera::getNearPlane()
{
	return nearp;
}

float Camera::getFarPlane()
{
	return farp;
}

mat4 Camera::getPersp(float width, float height)
{
	return glm::perspective(fov,(float)width/(float)height,nearp,farp);
}