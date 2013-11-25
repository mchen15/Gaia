#include "constants.h"

int NUM_QUADS = 1;
vec2 SUBDIV = vec2(32, 32);
float texScale = 1;
float heightScale = 200;
vec3 lightPosWorld = vec3(0, 0, 10);
float gridSpacing = 1;
float lodFactor = 1;
float tessLevelInner = 3;
float tessLevelOuter = 3;
bool genNormalMap = false;
bool enableTexcoords = false;

//const char* heightmapPath = "../../textures/earthbump1024.png";
//const char* heightmapPath = "../../textures/random.png";
//const char* heightmapPath = "../../textures/test2.png";
//const char* heightmapPath = "../../textures/test1024.png";
const char* heightmapPath = "../../textures/terrain_heights.jpg";
const char* normalmapPath = "../../textures/terrain_normals.jpg";
const char* diffusemapPath = "../../textures/terrain_diffuse.jpg";

const char* vertShaderPath = "../../shaders/static_heightmap.vert";
const char* tessCtrlShaderPath = "../../shaders/static_heightmap.tc";
const char* tessEvalShadePath = "../../shaders/static_heightmap.te"; 
const char* fragShaderPath = "../../shaders/static_heightmap.frag";

const char* vertQuadShaderPath = "../../shaders/static_quad_heightmap.vert";
const char* tessQuadCtrlShaderPath = "../../shaders/static_quad_heightmap.tc";
const char* tessQuadEvalShadePath = "../../shaders/static_quad_heightmap.te"; 
const char* fragQuadShaderPath = "../../shaders/static_quad_heightmap.frag";

const char* nmapVertShaderPath = "../../shaders/nmap.vert";
const char* nmapFragShaderPath = "../../shaders/nmap.frag";

//const char* heightmapPath = "D:/Dropbox/UPenn/CIS-565 (GPU Programming)/Assignments/Project7-FinalProject/Gaia/textures/heightmap.png";

// uniform ids
char* U_FARID = "u_Far";
char* U_NEARID = "u_Near";
char* U_MODELID = "u_Model";
char* U_VIEWID = "u_View";
char* U_PERSPID = "u_Persp";
char* U_TESSINNERID = "u_tessLevelInner";
char* U_TESSOUTERID = "u_tessLevelOuter";
char* U_TEXSCALEID = "u_texScale";
char* U_GRIDSPACINGID = "u_gridSpacing";
char* U_NUMPATCHESID = "u_numPatches";
char* U_HEIGHTMAPID = "u_heightMap";
char* U_HEIGHTSCALEID = "u_heightScale";
char* U_MVINVTRANSID = "u_mvInvTrans";
char* U_PVMID = "u_pvm";
char* U_LIGHTPOSWORLDID = "u_lightPosWorld";
char* U_LODFACTOR = "u_lodFactor";
char* U_NORMALMAPID = "u_normalMap";
char* U_DIFFUSEMAPID = "u_diffuseMap";
char* U_TOGGLENORMALID = "u_toggleNormal";