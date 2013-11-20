#ifndef PLANE_H
#define PLANE_H

#include "glm/glm.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

class Plane
{
public:
	Plane(glm::vec2 = glm::vec2(1.0,1.0), glm::vec2 u = glm::vec2(-1.0,-1.0),int subdivX=2, int subdivY=2);
	GLuint getVBO() {return vbo;}
	GLuint getTBO() {return tbo;}
	GLuint getIBO() {return ibo;}	
	void draw(int positionLocation, int texcoordsLocation);
	~Plane();

private:
	glm::vec2 lCorner;
	glm::vec2 uCorner;
	int divx;
	int divy;
	GLuint vbo;
	GLuint tbo;
	GLuint ibo;

	void initVAO();
};

#endif 