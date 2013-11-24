#ifndef PLANE_H
#define PLANE_H

#include <GL/glew.h>
#include <GL/glut.h>
#include "glm/glm.hpp"
#include "constants.h"

class Plane
{
public:
	Plane(glm::vec2 = glm::vec2(1.0,1.0), glm::vec2 u = glm::vec2(-1.0,-1.0),int subdivX=2, int subdivY=2);
	GLuint getVBO() {return vbo;}
	GLuint getTBO() {return tbo;}
	GLuint getT_IBO() {return t_ibo;}	

	int getIndexMode();
	void setIndexMode(int mode);
	void draw(int positionLocation);
	void draw(int positionLocation, int texCoordsLocation);
	void toggleWireframe();
	void toggleIndexingMode();
	~Plane();

private:
	glm::vec2 lCorner;
	glm::vec2 uCorner;
	int divx;
	int divy;
	GLuint vbo;
	GLuint tbo;
	GLuint t_ibo; // with triangle indexing
	bool wireframe;
	int indexingMode;
	void initVAO();
	void deleteVAOs(); // clears the vbo, tbo, and ibo
};

#endif 