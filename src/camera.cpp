#include "camera.h"


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
	rx = 0.0;
	ry = 0.0;
	z = posv.y;
	pos2 = vec2(position.x, position.y);
}

mat4 Camera::getView()
{
	//return lookAt(position, lookAtPoint, up);
	vec3 start_dir = lookAtPoint - position;
	vec3 start_left = glm::cross(start_dir,up);
	vec3 inclin = glm::gtx::rotate_vector::rotate(start_dir,ry,start_left);
    vec3 spun = glm::gtx::rotate_vector::rotate(inclin,rx,up);
    vec3 cent(pos2, z);
    return lookAt(cent, cent + spun, up);
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



void Camera::adjust(
		float dx, // look left right
        float dy, //look up down
        float dz,
        float tx, //strafe left right
        float ty,
        float tz)//go forward) //strafe up down
{
	vec3 start_dir = lookAtPoint - position;
    if (abs(dx) > 0) {
        rx += dx;
        rx = fmod(rx,360.0f);
    }

    if (abs(dy) > 0) {
        ry += dy;
        ry = clamp(ry,-70.0f, 70.0f);
    }

    if (abs(tx) > 0) {
        vec3 dir = glm::gtx::rotate_vector::rotate(start_dir,rx + 90,up);
        vec2 dir2(dir.x,dir.y);
        vec2 mag = dir2 * tx;
        pos2 += mag;	
    }

    if (abs(ty) > 0) {
        z += ty;
    }

    if (abs(tz) > 0) {
        vec3 dir = glm::gtx::rotate_vector::rotate(start_dir,rx,up);
        vec2 dir2(dir.x,dir.y);
        vec2 mag = dir2 * tz;
        pos2 += mag;
    }

	position = vec3(pos2.x, pos2.y, position.z);
}