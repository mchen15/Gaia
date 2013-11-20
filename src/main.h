#ifndef MAIN_H
#define MAIN_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>
#include "SOIL.h"
#include "camera.h"
#include "plane.h"

using std::cout;
using std::endl;
using glm::vec2;

Camera* cam;
Plane* plane;
GLuint pass_prog;

int width;
int height;

void reshape(int w, int h);
void display(void);


#endif
