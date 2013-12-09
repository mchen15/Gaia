#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>
#include "SOIL.h"
#include "constants.h"

using std::vector;
using std::cout;
using std::endl;

class Skybox {
public:
	Skybox(vector<char*> texNames);
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
	vector<char*> textureNames;
	GLuint vbo;
	GLuint ibo;
};


#endif