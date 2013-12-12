#include "camera.h"
#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
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
	rx = 0;
	ry = 0;
	z = posv.z;
	startDirection = glm::normalize(lp - posv);
	startLeft = glm::cross(startDirection, up);
}

mat4 Camera::getView()
{
	//return lookAt(position, lookAtPoint, up);

	vec3 inclin = glm::gtx::rotate_vector::rotate(startDirection,ry,startLeft);
    vec3 spun = glm::gtx::rotate_vector::rotate(inclin,rx,up);
    return lookAt(position, position + spun, up);
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

  
void Camera::adjust(float dx, // look left right
        float dy, //look up down
        float dz,
        float tx, //strafe left right
        float ty,
        float tz)//go forward) //strafe up down
{
	vec2 pos = vec2(position.x, position.y);

    if (abs(dx) > 0) {
        rx += dx;
        rx = fmod(rx,360.0f);
    }

    if (abs(dy) > 0) {
        ry += dy;
        ry = clamp(ry,-70.0f, 70.0f);
    }

    if (abs(tx) > 0) {
        vec3 dir = glm::gtx::rotate_vector::rotate(startDirection,rx + 90,up);
        vec2 dir2(dir.x,dir.y);
        vec2 mag = dir2 * tx;
        pos += mag;	
    }

    if (abs(ty) > 0) {
        z += ty;
    }

    if (abs(tz) > 0) {
        vec3 dir = glm::gtx::rotate_vector::rotate(startDirection,rx,up);
        vec2 dir2(dir.x,dir.y);
        vec2 mag = dir2 * tz;
        pos += mag;
    }

	position.x = pos.x;
	position.y = pos.y;
}

//class Camera
//{
//public:
//        Camera(glm::vec3 cameraPosition, glm::vec3 lookAt, glm::vec2 res)
//                :center(lookAt),
//                 radius( glm::length(lookAt-cameraPosition) ),
//                 resolution(res)
//                
//        {
//                glm::vec3 relCameraPos = cameraPosition - center;
//
//
//                theta = atan2f(relCameraPos.x,relCameraPos.y);
//                if (theta < 0.0f)
//                        theta = 2*PI+ theta;
//                phi = acosf( relCameraPos.z/radius);
//                originalPhi = phi;
//                originalTheta = theta;
//                originalCenter = center;
//                originalRadius = radius;
//
//        }
//
//        glm::vec3 getCameraPosition()
//        {
//                float y = radius*sinf(phi)*cosf(theta);
//                float z = radius*cosf(phi);
//                float x = radius*sinf(theta)*sinf(phi);
//
//                return center + glm::vec3(x,y,z);
//                
//        }
//
//        glm::vec3 getLookAtPosition()
//        {
//                return center;
//        }
//
//        void rotate (glm::vec2 delta)
//        {
//                const float epsilon = 0.0001f;
//                if ( abs(delta.x) > abs(delta.y))
//                {
//                        theta+= float(delta.x)/resolution.x;
//                }
//                else
//                {
//                        phi+=2.0f*float(delta.y)/resolution.y;
//                        if (phi > PI -epsilon )
//                                phi = PI-epsilon;
//
//                        else if (phi<epsilon)
//                                phi = epsilon;
//                }
//        }
//
//        void zoom( int delta)
//        {
//                radius= max(0.2f,radius+2*(float)delta/resolution.y);
//        }
//
//        void pan( glm::vec2 delta)
//        {
//                if( abs(delta.x) > abs(delta.y))
//                        center.x+= -0.0001f*delta.x;
//                else
//                        center.y+= 0.0001f*delta.y;
//        }
//        
//        void mouseClick(int button, int state, int x, int y)
//        {
//                currentMouseX = x;
//                currentMouseY = y;
//                currentMouseBtn = button;
//        }
//
//        void mouseMove(int x, int y)
//        {
//
//                if(currentMouseBtn == GLUT_LEFT_BUTTON)
//                {
//                        rotate(glm::vec2(x-currentMouseX,y-currentMouseY));
//                }
//                else if (currentMouseBtn == GLUT_RIGHT_BUTTON)
//                {
//                        int yDelta = y-currentMouseY;
//                        zoom(yDelta);
//                        
//                }
//                else if (currentMouseBtn == GLUT_MIDDLE_BUTTON)
//                {
//                        pan(glm::vec2(x-currentMouseX,y-currentMouseY));
//                        
//                }
//        }
//
//        void reset()
//        {
//                theta = originalTheta;
//                phi = originalPhi;
//                radius = originalRadius;
//                center = originalCenter;
//        }
//
//private:
//        int currentMouseX;
//        int currentMouseY;
//        int currentMouseBtn;
//        glm::vec3 center;
//        glm::vec3 originalCenter;
//        float originalTheta;
//        float originalPhi;
//        float originalRadius;
//        float phi;
//        float theta;
//        float radius;
//        glm::vec2 resolution;
//};

