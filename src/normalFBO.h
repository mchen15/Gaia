#ifndef NORMALFBO_H
#define NORMALFBO_H

#include "fboUtility.h"
#include "constants.h"

class NormalMapFBO : FrameBufferObject
{
public:
	NormalMapFBO(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations);
	~NormalMapFBO();

	void generateNormalMap(GLint inTexHandle, GLint referenceNormalMap = -1);

private:
	GLint inputTexHandle;
	GLint refNormalMap;

	void setNormalMapProgUniforms();

};

#endif