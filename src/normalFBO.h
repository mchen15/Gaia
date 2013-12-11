#ifndef NORMALFBO_H
#define NORMALFBO_H

#include "fboUtility.h"
#include "constants.h"

class NormalMapFBO : public FrameBufferObject
{
public:
	NormalMapFBO(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations,
		unsigned int quadVao = 0, unsigned int quadIBO = 0,unsigned int numQuadIndices = 0);
	~NormalMapFBO();

	void generateNormalMap(GLint inTexHandle, GLint referenceNormalMap = -1);
	void toggleNormalMap() {toggleNormalVal = !toggleNormalVal;}

private:
	GLint inputTexHandle;
	GLint refNormalMap;
	int toggleNormalVal;
	void setNormalMapProgUniforms();

};

#endif