#include "smoothFBO.h"

SmoothKernelFBO::SmoothKernelFBO(FrameBufferObject *pass1, FrameBufferObject *pass2) :
	smoothPass1FBO(pass1),
	smoothPass2FBO(pass2)
{
	glGenTextures(1, &intermediateTexture);
	glBindTexture(GL_TEXTURE_2D, intermediateTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, pass2->getWidth(), pass2->getHeight(), 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D,0);
}

SmoothKernelFBO::~SmoothKernelFBO()
{
	delete smoothPass1FBO;
	delete smoothPass2FBO;
	glDeleteTextures(1, &intermediateTexture);
}

void SmoothKernelFBO::smooth(GLuint intex, bool bindToDefaultFBO, int kernelSizeX, int kernelSizeY)
{
	inputTexture = intex;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(smoothPass1FBO->getShaderProg());
	bindFBO(smoothPass1FBO->getFBOHandle());
	smoothPass1FBO->changeTextureAttachments(intermediateTexture);
	setUpPass1FBOUniforms(kernelSizeX);
	smoothPass1FBO->render();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(smoothPass2FBO->getShaderProg());

	if (bindToDefaultFBO)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
		bindFBO(smoothPass2FBO->getFBOHandle());

	setUpPass2FBOUniforms(kernelSizeY);
	smoothPass2FBO->render();
}

void SmoothKernelFBO::setUpPass1FBOUniforms(int kernelSize)
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(smoothPass1FBO->getShaderProg(), "u_InputTex");
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputTexture);
		glUniform1i(uniformLocation, 0);	
	}

	uniformLocation = glGetUniformLocation(smoothPass2FBO->getShaderProg(),U_KERNELSIZEXID);
	if (uniformLocation != -1)
		glUniform1i(uniformLocation,kernelSize);
}

void SmoothKernelFBO::setUpPass2FBOUniforms(int kernelSize)
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(smoothPass2FBO->getShaderProg(), "u_SmoothPass1tex");
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, intermediateTexture);
		glUniform1i(uniformLocation, 0);	
	}

	uniformLocation = glGetUniformLocation(smoothPass2FBO->getShaderProg(),U_KERNELSIZEYID);
	if (uniformLocation != -1)
		glUniform1i(uniformLocation,kernelSize);
}

void SmoothKernelFBO::bindFBO(GLuint framebuf)
{
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void SmoothKernelFBO::changeTextureAttachments(GLuint tex)
{
	smoothPass2FBO->changeTextureAttachments(tex);
}