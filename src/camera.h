#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform2.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_operation.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/verbose_operator.hpp>
#include <iostream>

using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::lookAt;
using glm::clamp;

class Camera 
{
public:
	Camera();
	Camera(vec3 posv, vec3 lookAtPoint, vec3 upv,float fieldOfView,float nearPlane, float farPlane);
	mat4 getView();
	mat4 getPersp(float width, float height);
	float getNearPlane();
	float getFarPlane();
	void setPosition(glm::vec3 pos) { position = pos;}
	glm::vec3 getPosition() { return position;}
	void setLookAtPoint(glm::vec3 pt) { lookAtPoint = pt;}
	glm::vec3 getLookAtPoint(){ return lookAtPoint;}
	glm::vec3 getUp() { return up;}
	void adjust(float dx, float dy, float dz, float tx, float ty, float tz);
	
private:
	float rx;
	float ry;
	float z;
	vec3 startDirection;
	vec3 startLeft;
	vec3 position;
	vec3 up;
	vec3 lookAtPoint;
	float fov;
	float nearp;
	float farp;

};

#endif