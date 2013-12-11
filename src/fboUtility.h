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
	FrameBufferObject(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations, 
						unsigned int quadVao = 0, unsigned int quadIBO = 0,unsigned int numQuadIndices = 0);
	~FrameBufferObject();
	void changeTextureAttachments(GLuint tex);
	void changeTextureAttachments(vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations);
	GLuint getFBOHandle() { return FBOHandle; }
	GLuint getShaderProg() { return shaderProg; }
	int getWidth() { return width; }
	int getHeight() {return height; }
	void render();

protected:
	void checkFrameBufferStatus(GLenum framebufferStatus);
	void textureAttach();

protected:
	int width;						// width and height of the output texture
	int height;
	GLuint shaderProg;				// give one of the shader programs that will be using this fbo to write output textures
									// make sure that the indexing of the output are consistent

	GLuint FBOHandle;				// handle to this fbo
	vector<GLuint> textures;		// textures that have already been generated and will be used to store the outputs of this fbo
	vector<char*> shaderOut;		// names of the output attributes that the shaders using this FBO will write to
	vector<GLenum> texAttachLoc;	// attachment points for the textures (e.g. GL_COLOR_ATTACHMENTi)

	unsigned int vertex_array; 
	unsigned int vbo_indices;
	unsigned int num_indices;
};

#endif