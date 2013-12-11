#include "normalFBO.h"

NormalMapFBO::NormalMapFBO(int w, int h, GLuint shader, vector<GLuint> tex, vector<char*> outNames, vector<GLenum> texAttachLocations,
	unsigned int quadVao, unsigned int quadIBO,unsigned int numQuadIndices) :
	FrameBufferObject(w, h, shader, tex, outNames, texAttachLocations, quadVao, quadIBO, numQuadIndices)
{
	toggleNormalVal = 0;
}

NormalMapFBO::~NormalMapFBO()
{
}

void NormalMapFBO::generateNormalMap(GLint inTexHandle, GLint referenceNormalMap)
{
	inputTexHandle = inTexHandle;
	refNormalMap = referenceNormalMap;

	setNormalMapProgUniforms();
	renderToTextureAttachments();
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

	uniformLocation = glGetUniformLocation(shaderProg,U_TOGGLENORMALID);
	if (uniformLocation != -1 && refNormalMap != -1)
	{
		glUniform1i(uniformLocation, toggleNormalVal);
	}

	uniformLocation = glGetUniformLocation(shaderProg,U_HEIGHTSCALEID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, heightScale);
}