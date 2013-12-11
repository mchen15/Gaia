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
#include "skybox.h"
#include "normalFBO.h"
#include "smoothFBO.h"

using std::cout;
using std::endl;
using glm::vec3;
using glm::vec2;
using std::vector;
using glm::scale;

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
Plane* waterTestPlane;
Skybox* skybox;

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
GLuint copy_tex_prog;
GLuint skybox_prog;
GLuint water_shading_prog;
GLuint normalmap_prog;
GLuint smooth_intermediate_prog;
GLuint smooth_prog;

// textures
GLuint heightmap_tex;
GLuint normalmap_tex;
GLuint diffusemap_tex;
GLuint flux_tex;
GLuint terrainattr_tex;
GLuint velocity_tex;
GLuint temp_tex;
GLuint foammap_tex;

//Smooth
//FrameBufferObject* normalsFBO;
FrameBufferObject* smooth1FBO;
FrameBufferObject* smooth2FBO;

// FBOs
FrameBufferObject* copyFBO;
FrameBufferObject* initTerrainFBO;
FrameBufferObject* erosDepoFBO;
FrameBufferObject* evapFBO;
FrameBufferObject* flowSimFluxFBO;
FrameBufferObject* flowSimVelFBO;
FrameBufferObject* flowWatHeightFBO;
FrameBufferObject* sedTransFBO;
FrameBufferObject* waterIncFBO;
NormalMapFBO* normalMapFBO;
SmoothKernelFBO* smoothKernelFBO;

int mouse_buttons = 0;
int mouse_old_x = 0;
int mouse_old_y = 0;
int width;
int height;
int frame;
int timebase;
int toggleNormalVal = 0;
static float fps = 0;
bool userInteraction = false;
glm::vec2 terrainManipulatorCenter(-100,-100);
float terrainManipulatorRadius(0.0f);

const char *attributeWithTexLocation[] = {"Position", "Texcoord"};
const char *attributeLocation[] = {"Position"};

void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void initShader();
void setWaterTestUniforms ();
void setCurrProgUniforms();
void setSkyboxProgUniforms();
void reshape(int w, int h);
void display(void);
void initTextures();
void drawQuad();
void attachTempTexToFBO(FrameBufferObject** fbo, GLuint shader_prog);
void initErosionFBO();
void initNormalFBO();
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
void setCopyTexProgUniforms();
void setTerrainInitProgUniforms();
void setWaterIncProgUniforms();
void setFlowFluxProgUniforms();
void setFlowVelProgUniforms();
void setFlowWaterHeightProgUniforms();
void setErosionDepoProgUniforms();
void setSedimentTransProgUniforms();
void setEvapoProgUniforms();
void setSkyboxProgUniforms();

//Smooth
void setSmoothProgUniforms();
void setSmoothIntermediateProgUniforms();

// quad for outputting normal map
unsigned int vertex_array;
unsigned int vbo_indices;
unsigned int num_indices;
//Don't need these to get it working, but needed for deallocation
unsigned int vbo_data;
unsigned int texcoord_data;

void initQuad();

#endif
