#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform2.hpp"

using glm::vec3;
using glm::mat4;
using glm::lookAt;
class Camera 
{
public:
	Camera();
	Camera(vec3 posv, vec3 viewDir, vec3 upv,float fieldOfView,float nearPlane, float farPlane);
	mat4 getView();
	mat4 getPersp(float width, float height);
	float getNearPlane();
	float getFarPlane();
	
private:
	float rx;
	float ry;
	float rz;
	vec3 position;
	vec3 up;
	vec3 view;
	float fov;
	float nearp;
	float farp;

};

#endif