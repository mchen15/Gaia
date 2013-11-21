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
#include "glslUtility.h"

using std::cout;
using std::endl;
using glm::vec2;

namespace triangle_attributes {
    enum {
        POSITION,
        TEXCOORD
    };
}

Camera* cam;
Plane* plane;
GLuint pass_prog;
float tessLevelInner = 1.0;
float tessLevelOuter = 1.0;

int width;
int height;
int frame;
int timebase;
static float fps = 0;

const char *attributeLocations[] = { "Position", "Texcoords" };

void reshape(int w, int h);
void display(void);

#endif
