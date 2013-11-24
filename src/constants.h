#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;

#define ENABLE_TEXCOORDS 1;

enum INDEX_MODE {
	TRIANGLES,
	QUADS
};

extern int NUM_QUADS;
extern vec2 SUBDIV;
extern float texScale;
extern float heightScale;
extern vec3 lightPosWorld;
extern float gridSpacing;		// how far away each grid is
extern float lodFactor;

// paths
const extern char* heightmapPath;
const extern char* vertShaderPath;
const extern char* fragShaderPath;
const extern char* tessCtrlShaderPath;
const extern char* tessEvalShadePath;

// uniform ids
extern char* U_FARID;
extern char* U_NEARID;
extern char* U_MODELID;
extern char* U_VIEWID;
extern char* U_PERSPID;
extern char* U_TESSINNERID;
extern char* U_TESSOUTERID;
extern char* U_TEXSCALEID;
extern char* U_NUMPATCHESID;
extern char* U_HEIGHTSCALEID;
extern char* U_GRIDSPACINGID;
extern char* U_HEIGHTMAPID;
extern char* U_MVINVTRANSID;
extern char* U_PVMID;
extern char* U_LIGHTPOSWORLDID;
extern char* U_LODFACTOR;

#endif