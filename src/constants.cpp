#include "constants.h"

int NUM_QUADS = 1;
vec2 SUBDIV = vec2(18, 18);
float texScale = 1;
float heightScale = 1;
vec3 lightPosWorld = vec3(0, 0, 10);
float gridSpacing = 1;

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