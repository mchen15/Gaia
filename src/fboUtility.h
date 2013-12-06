#ifndef FBOUTILITY_H
#define FBOUTILITY_H

#include <GL/glew.h>
#include <iostream>
#include <vector>


using std::vector;
using std::cout;
using std::endl;

class FrameBufferObject
{
public:
	FrameBufferObject();
	FrameBufferObject(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations);
	~FrameBufferObject();
	void changeTextureAttachments(vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations);
	GLuint getFBOHandle() { return FBOHandle; }

private:
	void checkFrameBufferStatus(GLenum framebufferStatus);
	void textureAttach();

private:
	int width;						// width and height of the output texture
	int height;
	GLuint shaderProg;				// give one of the shader programs that will be using this fbo to write output textures
									// make sure that the indexing of the output are consistent

	GLuint FBOHandle;				// handle to this fbo
	vector<GLuint> textures;		// textures that have already been generated and will be used to store the outputs of this fbo
	vector<char*> shaderOut;		// names of the output attributes that the shaders using this FBO will write to
	vector<GLenum> texAttachLoc;	// attachment points for the textures (e.g. GL_COLOR_ATTACHMENTi)
};

#endif