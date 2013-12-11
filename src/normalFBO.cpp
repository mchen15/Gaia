#include "normalFBO.h"

NormalMapFBO::NormalMapFBO(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations) :
FrameBufferObject(w, h, shader, tex, outNames, texAttachLocations)
{
}

NormalMapFBO::~NormalMapFBO()
{
}

void NormalMapFBO::generateNormalMap(GLint inTexHandle, GLint referenceNormalMap)
{
	inputTexHandle = inTexHandle;
	referenceNormalMap = refNormalMap;

	setNormalMapProgUniforms();

}

void NormalMapFBO::setNormalMapProgUniforms()
{
	GLuint uniformLocation = glGetUniformLocation(shaderProg, U_HEIGHTMAPID);

	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inputTexHandle);
		glUniform1i(uniformLocation, 0);
	}

	uniformLocation = glGetUniformLocation(shaderProg, U_NORMALMAPID);

	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, refNormalMap);
		glUniform1i(uniformLocation, 1);
	}
}