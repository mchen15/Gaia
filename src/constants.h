#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <glm/glm.hpp>

using glm::vec2;

enum INDEX_MODE {
	TRIANGLES,
	QUADS
};

extern int NUM_QUADS;
extern vec2 SUBDIV;

// uniform ids
extern char* U_FARID;
extern char* U_NEARID;
extern char* U_MODELID;
extern char* U_VIEWID;
extern char* U_PERSPID;
extern char* U_TESSINNERID;
extern char* U_TESSOUTERID;

#endif