#ifndef SMOOTHFBO_H
#define SMOOTHFBO_H

#include "constants.h"
#include "fboUtility.h"

class SmoothKernelFBO
{
public:
	SmoothKernelFBO(FrameBufferObject *pass1, FrameBufferObject *pass2);
	~SmoothKernelFBO();
	void smooth(GLuint intex, bool bindToDefaultFBO = false,int kernelSizeX = 1, int kernelSizeY = 1);
	void changeTextureAttachments(GLuint tex);

private:
	FrameBufferObject *smoothPass1FBO;
	FrameBufferObject *smoothPass2FBO;
	GLuint intermediateTexture;
	GLuint inputTexture;
private:
	void setUpPass1FBOUniforms(int kernelSize);
	void setUpPass2FBOUniforms(int kernelSize);
	void bindFBO(GLuint framebuf);
};

#endif