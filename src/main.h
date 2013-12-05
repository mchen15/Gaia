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
#include "constants.h"
#include "fboUtility.h"

using std::cout;
using std::endl;
using glm::vec3;
using glm::vec2;
using std::vector;

namespace triangle_attributes {
    enum {
        POSITION,
        TEXCOORD
    };
}

namespace quad_attributes {
    enum {
        POSITION,
        TEXCOORD
    };
}

Camera* cam;
Plane* plane;

// shader programs
GLuint curr_prog;
GLuint fbo_test_prog;
GLuint terrain_init_prog;
GLuint erosion_depo_prog;
GLuint evapo_prog;
GLuint flow_flux_prog;
GLuint flow_vel_prog;
GLuint flow_water_height_prog;
GLuint sediment_trans_prog;
GLuint water_inc_prog;


// textures
GLuint heightmap_tex;
GLuint normalmap_tex;
GLuint diffusemap_tex;
GLuint flux_tex;
GLuint terrainattr_tex;
GLuint velocity_tex;


// FBOs
FrameBufferObject* initTerrainFBO;
FrameBufferObject* erosDepoFBO;
FrameBufferObject* evapFBO;
FrameBufferObject* flowSimFluxFBO;
FrameBufferObject* flowSimVelFBO;
FrameBufferObject* flowWatHeightFBO;
FrameBufferObject* sedTransFBO;
FrameBufferObject* waterIncFBO;

int mouse_buttons = 0;
int mouse_old_x = 0;
int mouse_old_y = 0;
int width;
int height;
int frame;
int timebase;
int toggleNormalVal = 0;
static float fps = 0;

const char *attributeWithTexLocation[] = {"Position", "Texcoord"};
const char *attributeLocation[] = {"Position"};

void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void initShader();
void setCurrProgUniforms();
void reshape(int w, int h);
void display(void);
void initTextures();
void drawQuad();
void initErosionFBO();
void initErosionShaders();
void initErosionTextures();
void deleteErosionFBO();
void setUpInitializationFBO();
void unbindTextures();
void setUpWaterIncFBO();
void setUpFlowSimFluxFBO();
void setUpFlowSimWaterHeightFBO();
void setUpFlowSimVelocityFBO();
void setUpErosionDepoFBO();
void setUpSedTransFBO();
void setUpEvapFBO();

// quad for outputting normal map
unsigned int vertex_array;
unsigned int vbo_indices;
unsigned int num_indices;
//Don't need these to get it working, but needed for deallocation
unsigned int vbo_data;
unsigned int texcoord_data;

void initQuad();

#endif
