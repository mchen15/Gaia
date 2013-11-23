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

#endif