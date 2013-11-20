// GLSL Utility: A utility class for loading GLSL shaders, for Patrick Cozzi's CIS565: GPU Computing at the University of Pennsylvania
// Written by Varun Sampath and Patrick Cozzi, Copyright (c) 2012 University of Pennsylvania

#ifndef GLSLUTILITY_H_
#define GLSLUTILITY_H_

#ifdef __APPLE__
	#include <GL/glfw.h>
#else
	#include <GL/glew.h>
#endif

namespace glslUtility
{
	typedef struct {
		GLuint vertex;
		GLuint fragment;
		GLuint tess_control;
		GLuint tess_eval;
		GLuint geometry;
	} shaders_t;

GLuint createProgram(const char *vertexShaderPath, const char *fragmentShaderPath, const char *attributeLocations[], GLuint numberOfLocations);
GLuint createProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath, const char *attributeLocations[], GLuint numberOfLocations);
GLuint createProgram(const char *vertexShaderPath, const char *tessControlShaderPath, const char *tessEvalShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath, const char *attributeLocations[], GLuint numberOfLocations);

}
 
#endif
