#ifndef SMOOTHFBO_H
#define SMOOTHFBO_H

#include "constants.h"
#include "fboUtility.h"

class SmoothKernelFBO
{
public:
	SmoothKernelFBO(FrameBufferObject *pass1, FrameBufferObject *pass2);
	~SmoothKernelFBO();
	void smooth(GLuint intex, bool bindToDefaultFBO = false);
	void changeTextureAttachments(GLuint tex);

private:
	FrameBufferObject *smoothPass1FBO;
	FrameBufferObject *smoothPass2FBO;
	GLuint intermediateTexture;
	GLuint inputTexture;
private:
	void setUpPass1FBOUniforms();
	void setUpPass2FBOUniforms();
	void bindFBO(GLuint framebuf);
};

#endif