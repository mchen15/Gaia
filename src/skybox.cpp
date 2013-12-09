#include "skybox.h"

Skybox::Skybox(vector<const char*> texNames) :
	textureNames(texNames)
{
	initSkybox();
	setupVBO();	
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &textureHandle);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Skybox::initSkybox()
{
	if (textureNames.size() < 6)
	{
		cout << "Skybox: Error: Need at 6 textures" << endl;
		return;
	}

	textureHandle = SOIL_load_OGL_cubemap(
		textureNames[0],
		textureNames[1],
		textureNames[2],
		textureNames[3],
		textureNames[4],
		textureNames[5], 0, 0, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::setupVBO()
{
	GLfloat cube_verts[] = {
		-1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(GLfloat), cube_verts, GL_STATIC_DRAW);

	GLushort cube_indices[] = {
		0, 1, 2, 3,
		3, 2, 6, 7,
		7, 6, 5, 4,
		4, 5, 1, 0,
		0, 3, 7, 4,
		1, 2, 6, 5
	};

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24*sizeof(GLushort), cube_indices, GL_STATIC_DRAW);
}

void Skybox::drawSkybox(int positionLocation)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(positionLocation);
}