#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "SOIL.h"
#include "constants.h"

using std::vector;
using std::cout;
using std::endl;

class Skybox {
public:
	Skybox(vector<const char*> texNames);
	~Skybox();
	
	GLuint getTextureHandle() {return textureHandle;}
	void drawSkybox(int positionLocation);

private:
	void initSkybox();
	void setupVBO();

private:
	int textureWidth;
	int textureHeight;
	GLuint textureHandle;
	vector<const char*> textureNames;
	GLuint vbo;
	GLuint ibo;
};


#endif