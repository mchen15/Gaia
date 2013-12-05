#include "constants.h"

// Program flow switches
bool genNormalMap = false;
bool enableErosion = true;
bool enableTexcoords = false;


int NUM_QUADS = 1;
vec2 SUBDIV = vec2(128, 128);
float texScale = 1;
float heightScale = 200;
vec3 lightPosWorld = vec3(0, 0, 10);
float gridSpacing = 1;
float pixelsPerEdge = 1;
float tessLevelInner = 3;
float tessLevelOuter = 3;

float deltaT = 0.05;
float virtualPipeLength = 1.0;
float virtualPipeArea = virtualPipeLength*virtualPipeLength;
float Kc = 0.001;
float Ks = 0.001;
float Ke = 0.001;
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

const char* vertTerrainTexInitPath = "../../shaders/Terrain_Initialization.vert";
const char* fragTerrainTexInitPath = "../../shaders/Terrain_Initialization.frag";

const char* vertFboTestPath = "../../shaders/fbo_test.vert";
const char* fragFboTestPath = "../../shaders/fbo_test.frag";

const char* vertTerrainTexInitPath;
const char* fragTerrainTexInitPath;
const char* vertErosDepoPath;
const char* fragErosDepoPath;
const char* vertEvapPath;
const char* fragEvapPath;
const char* vertFlowSimFluxPath;
const char* fragFlowSimFluxPath;
const char* vertFlowSimVelPath;
const char* fragFlowSimVelPath;
const char* vertFlowSimWatHeightPath;
const char* fragFlowSimWatHeightPath;
const char* vertWatIncPath;
const char* fragWatIncPath;

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
char* U_PIXELSPEREDGE = "u_pixelsPerEdge";
char* U_NORMALMAPID = "u_normalMap";
char* U_DIFFUSEMAPID = "u_diffuseMap";
char* U_TOGGLENORMALID = "u_toggleNormal";
char* U_SCREENSIZEID = "u_screen_size";
char* U_TERRAINATTRTEXID = "u_terrainAttrTex";
char* U_DELTATID = "u_deltaT";
char* U_FLUXTEXID = "u_fluxTex";
char* U_VELTEXID = "u_velTex";
char* U_VIRTUALPIPEAREAID = "u_virtualPipeArea";
char* U_VIRTUALPIPELENGTHID = "u_virtualPipeLength";
char* U_KCID = "u_Kc";
char* U_KSID = "u_Ks";
char* U_KEID = "u_Ke";