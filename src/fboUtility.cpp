#include "fboUtility.h"

FrameBufferObject::FrameBufferObject()
{
	width = 800;
	height = 800;
	FBOHandle = -1;
	glGenFramebuffers(1, &FBOHandle);
	textureAttach();
}

FrameBufferObject::FrameBufferObject(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations) :
	width(w),
	height(h),
	textures(tex),
	shaderOut(outNames),
	shaderProg(shader),
	texAttachLoc(texAttachLocations)
{
	if (outNames.size() != texAttachLocations.size())
		cout << "FrameBufferObject::FrameBufferObj: WARNING: number of shader output texture names is not equal to number of  attach locations" << endl;

	FBOHandle = 0;
	glGenFramebuffers(1, &FBOHandle);
	textureAttach();
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1, &FBOHandle);
}

void FrameBufferObject::checkFrameBufferStatus(GLenum framebufferStatus)
{
	switch (framebufferStatus) {
	case GL_FRAMEBUFFER_COMPLETE_EXT: break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		printf("Attachment Point Unconnected\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		printf("Missing Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		printf("Dimensions do not match\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		printf("Formats\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		printf("Draw Buffer\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		printf("Read Buffer\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		printf("Unsupported Framebuffer Configuration\n");
		break;
	default:
		printf("Unkown Framebuffer Object Failure\n");
		break;
	}
}

void FrameBufferObject::textureAttach()
{
	GLenum FBOstatus;
	glBindFramebuffer(GL_FRAMEBUFFER, FBOHandle);

	glReadBuffer(GL_NONE);
	vector<GLint> loc;

	for (int j = 0; j < shaderOut.size(); ++j)
	{
		GLint location = glGetFragDataLocation(shaderProg, (const GLchar*)shaderOut[j]);
		loc.push_back(location);
	}

	int numTextures = textures.size();
	GLenum* drawBufs = new GLenum[numTextures];

	for (int k = 0; k < texAttachLoc.size(); ++k)
	{
		GLint location = loc[k];
		drawBufs[location] = texAttachLoc[k];
	}

	glDrawBuffers(numTextures, drawBufs);

	for (int m = 0; m < numTextures; ++m)
	{
		glBindTexture(GL_TEXTURE_2D, textures[m]);
		//glFramebufferTexture(GL_FRAMEBUFFER, drawBufs[loc[m]], textures[m], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, drawBufs[loc[m]], GL_TEXTURE_2D, textures[m], 0);
	}

	FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("GL_FRAMEBUFFER_COMPLETE FAILED\n");
		checkFrameBufferStatus(FBOstatus);
	}

	delete drawBufs;
}

void FrameBufferObject::changeTextureAttachments(vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations)
{
	textures = tex;
	shaderOut = outNames;
	texAttachLoc = texAttachLocations;

	textureAttach();
}