#include "main.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_operation.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/verbose_operator.hpp>
#include <ctime>
#include <cstdlib>

void reshape(int w, int h)
{
    width = w;
    height = h;
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void updateFPS()
{
    frame++;
	int time=glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase > 1000) {
        fps = frame*1000.0f/(time-timebase);
        timebase = time;
        frame = 0;
    }

	char title[100];
	sprintf(title,"GAIA [%0.2f fps]", fps);
	glutSetWindowTitle(title);
}

// Given buf (index of the FBO array that we have created), this function will bind framebuf to the current OpenGL contex
void bindFBO(GLuint framebuf) {
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void unbindTextures()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT);
}

void terrainInit()
{
	unbindTextures();
	bindFBO(initTerrainFBO->getFBOHandle());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(terrain_init_prog);
	
	// setting up uniforms (e.g. input textures)
	setTerrainInitProgUniforms();

	drawQuad();
}

void renderToScreen()
{
	unbindTextures();
	glUseProgram(fbo_test_prog);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	// setting up uniforms (e.g. input textures)
	GLint uniformLocation = -1;
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, flux_tex);
	uniformLocation = glGetUniformLocation(fbo_test_prog, U_FLUXTEXID);
	glUniform1i(uniformLocation, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	uniformLocation = glGetUniformLocation(fbo_test_prog, U_TERRAINATTRTEXID);
	glUniform1i(uniformLocation, 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, velocity_tex);
	uniformLocation = glGetUniformLocation(fbo_test_prog, U_VELTEXID);
	glUniform1i(uniformLocation, 5);

	drawQuad();
}




void waterInc()
{
	unbindTextures();
	glUseProgram(water_inc_prog);
	bindFBO(waterIncFBO->getFBOHandle());
	//GLint uniformLocation = -1;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(water_inc_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 0);

	setWaterIncProgUniforms();
	drawQuad();
	
	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void flowSimFlux()
{
	unbindTextures();
	glUseProgram(flow_flux_prog);
	bindFBO(flowSimFluxFBO->getFBOHandle());
	//GLint uniformLocation = -1;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, flux_tex);
	//uniformLocation = glGetUniformLocation(flow_flux_prog, U_FLUXTEXID);
	//glUniform1i(uniformLocation, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(flow_flux_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 1);
	setFlowFluxProgUniforms();
	drawQuad();

		//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(flux_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void flowSimVel()
{
	unbindTextures();
	glUseProgram(flow_vel_prog);
	bindFBO(flowSimVelFBO->getFBOHandle());
	//GLint uniformLocation = -1;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, flux_tex);
	//uniformLocation = glGetUniformLocation(flow_vel_prog, U_FLUXTEXID);
	//glUniform1i(uniformLocation, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(flow_vel_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 1);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, velocity_tex);
	//uniformLocation = glGetUniformLocation(flow_vel_prog, U_VELTEXID);
	//glUniform1i(uniformLocation, 2);

	setFlowVelProgUniforms();
	drawQuad();

	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(velocity_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void flowSimWaterHeight()
{
	unbindTextures();
	glUseProgram(flow_water_height_prog);
	bindFBO(flowWatHeightFBO->getFBOHandle());
	//GLint uniformLocation = -1;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(flow_water_height_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, flux_tex);
	//uniformLocation = glGetUniformLocation(flow_water_height_prog, U_FLUXTEXID);
	//glUniform1i(uniformLocation, 1);
	setFlowWaterHeightProgUniforms();
	drawQuad();

	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void erosionDeposition()
{
	unbindTextures();
	glUseProgram(erosion_depo_prog);
	bindFBO(erosDepoFBO->getFBOHandle());
	//GLint uniformLocation = -1;
	//
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, flux_tex);
	//uniformLocation = glGetUniformLocation(erosion_depo_prog, U_FLUXTEXID);
	//glUniform1i(uniformLocation, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(erosion_depo_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 1);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, velocity_tex);
	//uniformLocation = glGetUniformLocation(erosion_depo_prog, U_VELTEXID);
	//glUniform1i(uniformLocation, 2);

	setErosionDepoProgUniforms();
	drawQuad();

	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void sedimentTransport()
{
	unbindTextures();
	glUseProgram(sediment_trans_prog);
	bindFBO(sedTransFBO->getFBOHandle());
	//GLint uniformLocation = -1;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(sediment_trans_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, velocity_tex);
	//uniformLocation = glGetUniformLocation(sediment_trans_prog, U_VELTEXID);
	//glUniform1i(uniformLocation, 1);

	setSedimentTransProgUniforms();
	drawQuad();

	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void evaporation()
{
	unbindTextures();
	glUseProgram(evapo_prog);
	bindFBO(evapFBO->getFBOHandle());
	//GLint uniformLocation = -1;

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	//uniformLocation = glGetUniformLocation(evapo_prog, U_TERRAINATTRTEXID);
	//glUniform1i(uniformLocation, 0);
	setEvapoProgUniforms();

	drawQuad();

	//COPY
	glUseProgram(copy_tex_prog);
	bindFBO( copyFBO->getFBOHandle());
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);
	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	copyFBO->changeTextureAttachments(fboTex,fboOutNames,attachLocations);
	setCopyTexProgUniforms();
	drawQuad();
}

void updateNormals()
{
	glUseProgram(normalmap_prog);
	bindFBO(normalMapFBO->getFBOHandle());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	normalMapFBO->generateNormalMap(terrainattr_tex, normalmap_tex);	 // render the normal map generated from terrainattr_tex into temp_tex
																		 // see setUpNormalsFBO() for the setup information
	
	smoothKernelFBO->changeTextureAttachments(computednormalmap_tex);	 // render to computednormalmap_tex
	smoothKernelFBO->smooth(temp_tex,false);							 // take temp_tex as the input
}

void renderTerrain()
{
	unbindTextures();
	glUseProgram(curr_prog);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	setCurrProgUniforms();
	plane->draw(triangle_attributes::POSITION);
}

void display(void)
{
	updateFPS();

	if (genNormalMap)
	{
		glUseProgram(normalmap_prog);
		bindFBO(normalMapFBO->getFBOHandle());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		normalMapFBO->generateNormalMap(heightmap_tex, normalmap_tex);
		smoothKernelFBO->smooth(temp_tex,true);

	}
	else if (enableErosion) // temporarily have erosion as a completely different part of our pipeline for debugging purposes
	{
		updateNormals();

		// Water Increment
		waterInc();
		
		// Flow Sim
		flowSimFlux();
		flowSimWaterHeight();
		flowSimVel();

		// Erosion Deposition
		erosionDeposition();

		// Sediment Transport
		sedimentTransport();

		// Evaporation
		evaporation();

		// Show results!
		renderTerrain();
	}
	else if (enableWaterTest)
	{
		glUseProgram(water_shading_prog);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setWaterTestUniforms();
		waterTestPlane->setIndexMode(INDEX_MODE::TRIANGLES);
		waterTestPlane->draw(triangle_attributes::POSITION, triangle_attributes::TEXCOORD);
	}
	else
	{
		glUseProgram(curr_prog);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		setCurrProgUniforms();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightmap_tex);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalmap_tex);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, diffusemap_tex);
		if (enableTexcoords)
		{
			// using pass shaders
			plane->setIndexMode(INDEX_MODE::TRIANGLES);
			plane->draw(triangle_attributes::POSITION, triangle_attributes::TEXCOORD);
		}
		else 
		{
			plane->draw(triangle_attributes::POSITION);
		}
	}
	
	if (!genNormalMap)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glUseProgram(skybox_prog);
		setSkyboxProgUniforms();
		skybox->drawSkybox(triangle_attributes::POSITION);
	}

	glutPostRedisplay();
    glutSwapBuffers();
}

void setCopyTexProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(copy_tex_prog, U_SOURCETEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, temp_tex);
		glUniform1i(uniformLocation, 0);	
	}
}

void setTerrainInitProgUniforms()
{	
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(terrain_init_prog, U_HEIGHTSCALEID);
	if (uniformLocation != -1)
	{
		glUniform1f(uniformLocation, heightScale);
	}
	uniformLocation = glGetUniformLocation(terrain_init_prog, U_HEIGHTMAPID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightmap_tex);
		glUniform1i(uniformLocation, 0);	
	}
}

void setWaterIncProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(water_inc_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(water_inc_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);

	uniformLocation = glGetUniformLocation(water_inc_prog,U_WTRSRCID);
	if (uniformLocation != -1)
		glUniform1i(uniformLocation,waterSource);

	uniformLocation = glGetUniformLocation(water_inc_prog,U_MANIPCENTERID);
	if (uniformLocation != -1)
		glUniform2fv(uniformLocation, 1, &terrainManipulatorCenter[0]);

	uniformLocation = glGetUniformLocation(water_inc_prog,U_MANIPRADIUSID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, terrainManipulatorRadius);

	uniformLocation = glGetUniformLocation(water_inc_prog,U_WTRSRCID);
	if (uniformLocation != -1)
		glUniform1i(uniformLocation, waterSource);

	srand((unsigned)time(0)); 
	float randomSeed = (float)rand()/RAND_MAX;

	uniformLocation = glGetUniformLocation(water_inc_prog,U_RANDSEED);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,randomSeed);

}

void setFlowFluxProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_FLUXTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, flux_tex);
	    glUniform1i(uniformLocation,1);
	}
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_VIRTUALPIPEAREAID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,virtualPipeArea);
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_VIRTUALPIPELENGTHID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,virtualPipeLength);
	uniformLocation = glGetUniformLocation(flow_flux_prog,U_GRIDSPACINGID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,gridSpacing);
}

void setFlowWaterHeightProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(flow_water_height_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(flow_water_height_prog,U_FLUXTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, flux_tex);
	    glUniform1i(uniformLocation,1);
	}
	uniformLocation = glGetUniformLocation(flow_water_height_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);
	uniformLocation = glGetUniformLocation(flow_water_height_prog,U_GRIDSPACINGID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,gridSpacing);
}

void setFlowVelProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(flow_vel_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(flow_vel_prog,U_FLUXTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, flux_tex);
	    glUniform1i(uniformLocation,1);
	}
	uniformLocation = glGetUniformLocation(flow_vel_prog,U_VELTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, velocity_tex);
	    glUniform1i(uniformLocation,2);
	}
	uniformLocation = glGetUniformLocation(flow_vel_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);
	uniformLocation = glGetUniformLocation(flow_vel_prog,U_GRIDSPACINGID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,gridSpacing);
}

void setErosionDepoProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_VELTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, velocity_tex);
	    glUniform1i(uniformLocation,1);
	}
	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_NORMALMAPID);
	if (uniformLocation !=-1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalmap_tex);
		glUniform1i(uniformLocation, 2);
	}
	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_KCID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,Kc);
	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_KSID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,Ks);	

	uniformLocation = glGetUniformLocation(erosion_depo_prog,U_UP);
	if (uniformLocation != -1)
	{
		glUniform3fv(uniformLocation, 1, &cam->getUp()[0]);
	}

}

void setSedimentTransProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(sediment_trans_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(sediment_trans_prog,U_VELTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, velocity_tex);
	    glUniform1i(uniformLocation,1);
	}
	uniformLocation = glGetUniformLocation(sediment_trans_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);
}

void setEvapoProgUniforms()
{
	GLint uniformLocation = -1;
	uniformLocation = glGetUniformLocation(evapo_prog,U_TERRAINATTRTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}
	uniformLocation = glGetUniformLocation(evapo_prog,U_DELTATID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,deltaT);
	uniformLocation = glGetUniformLocation(evapo_prog,U_KEID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation,Ke);
}

void setSkyboxProgUniforms()
{
	mat4 model = scale(mat4(1.0), vec3(2000, 2000, 2000));
	mat4 view = cam->getView();
	mat4 project = cam->getPersp(width,height);

	GLint uniformLocation = glGetUniformLocation(skybox_prog,U_PVMID);
	
	if (uniformLocation != -1)
	{
		mat4 pvm = project * view * model;
		glUniformMatrix4fv(uniformLocation, 1, false, &pvm[0][0]);
	}

	uniformLocation = glGetUniformLocation(skybox_prog, U_CUBEMAPID);

	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureHandle());
		glUniform1i(uniformLocation, 0);
	}
}

void setCurrProgUniforms()
{
	mat4 model(1.0f);
	mat4 view = cam->getView();
	mat4 persp = cam->getPersp(float(width), float(height));
  
	GLint uniformLocation = -1;

	uniformLocation = glGetUniformLocation(curr_prog, U_FARID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, cam->getFarPlane());

	uniformLocation = glGetUniformLocation(curr_prog, U_NEARID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, cam->getNearPlane());
	
	uniformLocation = glGetUniformLocation(curr_prog, U_TESSINNERID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, tessLevelInner);
	
	uniformLocation = glGetUniformLocation(curr_prog, U_TESSOUTERID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, tessLevelOuter);

	uniformLocation = glGetUniformLocation(curr_prog,U_MODELID);
	if (uniformLocation != -1)
		glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&model[0][0]);

	uniformLocation = glGetUniformLocation(curr_prog,U_VIEWID);
	if (uniformLocation != -1)
		glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&view[0][0]);
	
	uniformLocation = glGetUniformLocation(curr_prog,U_PERSPID);
	if (uniformLocation != -1)
		glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&persp[0][0]);

	uniformLocation = glGetUniformLocation(curr_prog,U_TEXSCALEID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, texScale);

	uniformLocation = glGetUniformLocation(curr_prog,U_NUMPATCHESID);
	if (uniformLocation != -1)
		glUniform2fv(uniformLocation, 1, &SUBDIV[0]);

	uniformLocation = glGetUniformLocation(curr_prog,U_HEIGHTSCALEID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, heightScale);

	uniformLocation = glGetUniformLocation(curr_prog,U_GRIDSPACINGID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, gridSpacing);

	uniformLocation = glGetUniformLocation(curr_prog,U_PVMID);
	if (uniformLocation != -1)
	{
		mat4 mvp = persp * view * model;
		glUniformMatrix4fv(uniformLocation,1,GL_FALSE,&mvp[0][0]);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_MVINVTRANSID);
	if (uniformLocation != -1)
	{
		mat4 imv = glm::inverse(view * model);
		imv[3][0] = 0.0;
		imv[3][1] = 0.0;
		imv[3][2] = 0.0;
		imv = glm::transpose(imv);

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &imv[0][0]);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_LIGHTPOSWORLDID);
	if (uniformLocation != -1)
	{
		glUniform3fv(uniformLocation, 1, &lightPosWorld[0]);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_HEIGHTMAPID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	    glUniform1i(uniformLocation,0);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_PIXELSPEREDGE);
	if (uniformLocation != -1)
	{
		glUniform1f(uniformLocation, pixelsPerEdge);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_NORMALMAPID);
	if (uniformLocation !=-1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalmap_tex);
		glUniform1i(uniformLocation, 1);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_DIFFUSEMAPID);
	if (uniformLocation !=-1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, diffusemap_tex);
		glUniform1i(uniformLocation, 2);
	}

	uniformLocation = glGetUniformLocation(curr_prog, U_COMPUTEDNORMALMAPID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, computednormalmap_tex);
		glUniform1i(uniformLocation, 3);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_TOGGLENORMALID);
	if (uniformLocation != -1)
	{
		glUniform1i(uniformLocation, toggleNormalVal);
	}

	uniformLocation = glGetUniformLocation(curr_prog,U_SCREENSIZEID);
	if (uniformLocation != -1)
	{
		vec2 screen_size = vec2((float)width, (float)height);
		glUniform2fv(uniformLocation, 1, &screen_size[0]);
	}

	uniformLocation = glGetUniformLocation(curr_prog, U_LIGHTDIRECTIONID);
	if (uniformLocation != -1)
	{
		vec3 lightDir = glm::normalize(vec3(1.0, 5.2, 4.5));
		glUniform3fv(uniformLocation, 1, &lightDir[0]);
	}

	uniformLocation = glGetUniformLocation(curr_prog, U_LIGHTCOLORID);
	if (uniformLocation)
	{
		glm::vec4 lightColor = glm::vec4(1.0, 0.95, 0.9, 1.0) * 1.1;
		glUniform4fv(uniformLocation, 1, &lightColor[0]);
	}
	uniformLocation = glGetUniformLocation(curr_prog,U_MANIPCENTERID);
	if (uniformLocation != -1)
		glUniform2fv(uniformLocation, 1, &terrainManipulatorCenter[0]);

	uniformLocation = glGetUniformLocation(curr_prog,U_MANIPRADIUSID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, terrainManipulatorRadius);

	uniformLocation = glGetUniformLocation(curr_prog,U_HEIGHTSCALEID);
	if (uniformLocation != -1)
		glUniform1f(uniformLocation, heightScale);

	uniformLocation = glGetUniformLocation(curr_prog,U_USERINTID);
	if (uniformLocation != -1)
		glUniform1i(uniformLocation, userInteraction);
}

void setWaterTestUniforms ()
{
	mat4 model(1.0f);
	mat4 view = cam->getView();
	mat4 persp = cam->getPersp(float(width), float(height));

	mat4 pvm = persp * view * model;
	GLint uniformLocation = glGetUniformLocation(water_shading_prog, U_PVMID);
	glUniformMatrix4fv(uniformLocation, 1,GL_FALSE ,&pvm[0][0]);
	
	uniformLocation = glGetUniformLocation(water_shading_prog, U_LIGHTDIRECTIONID);
	if (uniformLocation != -1)
	{
		vec3 lightDir = glm::normalize(vec3(1.0, 5.2, 4.5));
		glUniform3fv(uniformLocation, 1, &lightDir[0]);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_LIGHTCOLORID);
	if (uniformLocation)
	{
		glm::vec4 lightColor = glm::vec4(1.0, 0.95, 0.9, 1.0) * 1.1;
		glUniform4fv(uniformLocation, 1, &lightColor[0]);
	}
	
	uniformLocation = glGetUniformLocation(water_shading_prog,U_HEIGHTMAPID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
		glUniform1i(uniformLocation,0);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog,U_NORMALMAPID);
	if (uniformLocation !=-1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalmap_tex);
		glUniform1i(uniformLocation, 1);
	}

	// calculate fresnel term
	uniformLocation = glGetUniformLocation(water_shading_prog,U_FRESNELTERMID);
	if (uniformLocation != -1)
	{
		float IOR = 1.33333;
		float a = 0;
		float c = cos(a) * IOR;
		float g = sqrt(1 + pow(c,2) - pow(IOR, 2));
		float t1 = pow((g-c)/(g+c), 2);
		float t21 = c*(g+c) - pow(IOR,2);
		float t22 = c*(g-c) + pow(IOR,2);
		
		float R0 = 0.5 * t1 * (1 + pow((t21/t22),2));

		glUniform1f(uniformLocation, R0);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_CAMPOSID);
	if (uniformLocation != -1)
	{
		vec3 camPos = cam->getPosition();
		glUniform3fv(uniformLocation, 1, &camPos[0]);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_MODELID);
	if (uniformLocation != -1)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &model[0][0]);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_VIEWID);
	if (uniformLocation != -1)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &view[0][0]);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_FOAMTEXID);
	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, foammap_tex);
		glUniform1i(uniformLocation, 2);
	}

	uniformLocation = glGetUniformLocation(water_shading_prog, U_CUBEMAPID);

	if (uniformLocation != -1)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTextureHandle());
		glUniform1i(uniformLocation, 3);
	}
}
void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case(27):
			exit(0);
			break;
		case('n'):
			plane->toggleWireframe();
			break;
		case ('1'):
			tessLevelInner++;
			std::cout<<"Inner: "<<tessLevelInner<<std::endl;
			break;
		case ('2'):
			if (tessLevelInner > 0)
				tessLevelInner--;
			std::cout<<"Inner: "<<tessLevelInner<<std::endl;
			break;
		case ('3'):
			tessLevelOuter++;
			std::cout<<"Outer: "<<tessLevelOuter<<std::endl;
			break;
		case ('4'):
			if (tessLevelOuter > 0)
				tessLevelOuter--;
			std::cout<<"Outer: "<<tessLevelOuter<<std::endl;
			break;
		case ('m'):
			plane->toggleIndexingMode();
			break;
		case ('w'):
			cam->setPosition( cam->getPosition()+ vec3(0,5,0));
			cam->setLookAtPoint(cam->getLookAtPoint() + vec3(0,5,0));
			break;
		case ('s'):
			cam->setPosition( cam->getPosition()- vec3(0,5,0));
			cam->setLookAtPoint(cam->getLookAtPoint() - vec3(0,5,0));
			break;
		case ('a'):
			cam->setPosition( cam->getPosition()- vec3(5,0,0));
			cam->setLookAtPoint(cam->getLookAtPoint() - vec3(5,0,0));
			break;
		case ('d'):
			cam->setPosition( cam->getPosition()+vec3(5,0,0));
			cam->setLookAtPoint(cam->getLookAtPoint() + vec3(5,0,0));
			break;
		case ('r'):
			initShader();
			initErosionShaders();
			break;
		case ('t'):
			toggleNormalVal = !toggleNormalVal;
			normalMapFBO->toggleNormalMap();
			break;
		case ('i'):
			userInteraction = !userInteraction;
			break;
		case ('k'):
			if(userInteraction)
			{
				waterSource = RAIN;
			}
			break;
		case ('l'):
			if(userInteraction)
			{
				waterSource = NOSOURCE;
			}
			break;
		case('5'):
			pixelsPerEdge++;
			cout << "Lod Factor = " << pixelsPerEdge << endl;;
			break;
		case('6'):
			if (pixelsPerEdge > 0)
				pixelsPerEdge--;
			cout << "Lod Factor = " << pixelsPerEdge  << endl;;
			break;

		case('['):
			terrainManipulatorRadius -= 0.002;
			if(terrainManipulatorRadius<= 0.002)
				terrainManipulatorRadius= 0.002;
			break;

		case(']'):
			terrainManipulatorRadius += 0.002;
	}
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        mouse_buttons |= 1<<button;
		if(userInteraction)
			waterSource = LAKE;
    } else if (state == GLUT_UP) {
        mouse_buttons = 0;
		waterSource = NOSOURCE;
    }

    mouse_old_x = x;
    mouse_old_y = y;
}

void motion(int x, int y)
{
    float dx, dy;
    dx = (float)(x - mouse_old_x);
    dy = (float)(y - mouse_old_y);

    if (mouse_buttons & 1<<GLUT_RIGHT_BUTTON) {
        cam->adjust(0,0,dx,0,0,0);
    }
    else {
        cam->adjust(-dx*0.2f,-dy*0.2f,0,0,0,0);
    }

    mouse_old_x = x;
    mouse_old_y = y;
}

void passiveMotion(int x, int y)
{
	if ( userInteraction)
	{
		float px = (float)x/width;
		float py = 1.0 - (float)y/height;
	
		px = px*2.0 - 1.0;
		py = py*2.0 - 1.0;

		glm::mat4 pInv = glm::inverse(cam->getPersp(width,height));
		glm::mat4 viewInv = glm::inverse(cam->getView());


		glm::vec4 ndc = pInv*glm::vec4(px,py,0.0,1.0);
		ndc = glm::normalize(ndc);
		ndc.w = 0.0;
		glm::vec4 worldSpace = glm::normalize(viewInv*ndc);
		glm::vec3 eyeToSceneDir = glm::vec3(worldSpace.x,worldSpace.y,worldSpace.z);
	
		float t = -cam->getPosition().z / eyeToSceneDir.z;
		glm::vec3 point = cam->getPosition() + t*eyeToSceneDir;

		if( point.x>= 0 && point.x<=width && point.y >=0 && point.y<=height)
		{
			terrainManipulatorCenter.x = point.x/width;
			terrainManipulatorCenter.y = point.y/height;
		}
	}
}

void initTextures()
{
	heightmap_tex = (unsigned int)SOIL_load_OGL_texture(heightmapPath,0,0,0);
    glBindTexture(GL_TEXTURE_2D, heightmap_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

	normalmap_tex = (unsigned int)SOIL_load_OGL_texture(normalmapPath,0,0,0);
    glBindTexture(GL_TEXTURE_2D, normalmap_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

	diffusemap_tex = (unsigned int)SOIL_load_OGL_texture(diffusemapPath,0,0,0);
    glBindTexture(GL_TEXTURE_2D, diffusemap_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

	foammap_tex = (unsigned int)SOIL_load_OGL_texture(diffusemapPath,0,0,0);
	glBindTexture(GL_TEXTURE_2D, diffusemap_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void initScene()
{
	// camera set up for quads rendering using 2nd technique
	vec3 camPosition = vec3(0, -200,300);
	vec3 lookAtPoint = vec3(512,512,0);

	// camera set up for lower corner technique
	//vec3 camPosition = vec3(0, 0, 0);
	//vec3 lookAtPoint = vec3(5,0,0);

	vec3 up = vec3(0,0,1);
	float fov = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 5000.0f;

	cam = new Camera(camPosition, lookAtPoint, up,fov,nearPlane,farPlane);
	plane = new Plane(vec2(0), vec2(1), SUBDIV.x, SUBDIV.y); // LOOK: Our plane is from 0 to 1 with numPatches
	
	if (enableWaterTest)
		waterTestPlane = new Plane(vec2(0), vec2(1280, 720), 200, 100);

	vector<const char*> texNames;
	
	texNames.push_back(cubeMapRightImgPath);
	texNames.push_back(cubeMapLeftImgPath);

	texNames.push_back(cubeMapFrontImgPath);
	texNames.push_back(cubeMapBackImgPath);
	
	texNames.push_back(cubeMapUpImgPath);
	texNames.push_back(cubeMapDownImgPath);

	skybox = new Skybox(texNames);
}
void initErosionTextures()
{
	glGenTextures(1, &flux_tex);
	glGenTextures(1, &terrainattr_tex);
	glGenTextures(1, &velocity_tex);
	glGenTextures(1, &temp_tex);
	glGenTextures(1, &computednormalmap_tex);


	glBindTexture(GL_TEXTURE_2D, flux_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, terrainattr_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, velocity_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, temp_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	
	glBindTexture(GL_TEXTURE_2D, computednormalmap_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
}

void setUpCopyFBO()
{
	// setting up texture handles: flux, terrainAttr, velocity
	vector<GLuint> fboTex;
	fboTex.push_back(temp_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_destTex");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	// getting the outputs
	copyFBO = new FrameBufferObject(width, height, copy_tex_prog, fboTex, fboOutNames, attachLocations);
}

void setUpInitializationFBO()
{
	// setting up texture handles: flux, terrainAttr, velocity
	vector<GLuint> fboTex;
	fboTex.push_back(flux_tex);
	fboTex.push_back(terrainattr_tex);
	fboTex.push_back(velocity_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_flux");
	fboOutNames.push_back("out_terrainAttr");
	fboOutNames.push_back("out_velocity");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);
	attachLocations.push_back(GL_COLOR_ATTACHMENT1);
	attachLocations.push_back(GL_COLOR_ATTACHMENT2);

	// getting the outputs
	initTerrainFBO = new FrameBufferObject(width, height, terrain_init_prog, fboTex, fboOutNames, attachLocations);
}

void attachTempTexToFBO(FrameBufferObject** fbo, char* outName, GLuint shader_prog)
{
	vector<GLuint> fboTex;
	fboTex.push_back(temp_tex);
	
	vector<char*> fboOutName;
	fboOutName.push_back(outName); // This is used in Copy_Texture.vert / frag

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	*fbo = new FrameBufferObject(width, height, shader_prog, fboTex, fboOutName, attachLocations);
}

void setUpNormalsFBO()
{
	// setting up texture handles: temp_tex
	vector<GLuint> fboTex;
	fboTex.push_back(temp_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_Color");
	
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	normalMapFBO = new NormalMapFBO(width, height, normalmap_prog, fboTex, fboOutNames, attachLocations, vertex_array, vbo_indices, num_indices);
}

void setUpWaterIncFBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_terrainAttr");
	
	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	waterIncFBO = new FrameBufferObject(width, height, water_inc_prog, fboTex, fboOutNames, attachLocations);
}

void setUpFlowSimFluxFBO()
{
	// setting up texture handles: flux
	vector<GLuint> fboTex;
	fboTex.push_back(flux_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_flux");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	flowSimFluxFBO = new FrameBufferObject(width, height, flow_flux_prog, fboTex, fboOutNames, attachLocations);
}

void setUpFlowSimWaterHeightFBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_terrainAttrTex");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	flowWatHeightFBO = new FrameBufferObject(width, height, flow_water_height_prog, fboTex, fboOutNames, attachLocations);
}

void setUpFlowSimVelocityFBO()
{
	// setting up texture handles: vel
	vector<GLuint> fboTex;
	fboTex.push_back(velocity_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_vel");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	flowSimVelFBO = new FrameBufferObject(width, height, flow_vel_prog, fboTex, fboOutNames, attachLocations);
}

void setUpErosionDepoFBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_terrainAttr");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	erosDepoFBO = new FrameBufferObject(width, height, erosion_depo_prog, fboTex, fboOutNames, attachLocations);
}

void setUpSedTransFBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_terrainAttr");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	sedTransFBO = new FrameBufferObject(width, height, sediment_trans_prog, fboTex, fboOutNames, attachLocations);
}

void setUpEvapFBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(terrainattr_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_terrainAttr");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	evapFBO = new FrameBufferObject(width, height, evapo_prog, fboTex, fboOutNames, attachLocations);
}

void setUpSmoothPass1FBO()
{
	vector<GLuint> fboTex;
	fboTex.push_back(temp_tex); // note that this texture doesnt matter because SmoothKernelFBO will use its own intermediate texture.

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_Color");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	smooth1FBO = new FrameBufferObject(width, height, smooth_intermediate_prog, fboTex, fboOutNames, attachLocations, vertex_array, vbo_indices, num_indices);
}

void setUpSmoothPass2FBO()
{
	// setting up texture handles: terrainAttr
	vector<GLuint> fboTex;
	fboTex.push_back(temp_tex);

	// setting up the output variable names used in the shader
	vector<char*> fboOutNames;
	fboOutNames.push_back("out_Color");

	vector<GLenum> attachLocations;
	attachLocations.push_back(GL_COLOR_ATTACHMENT0);

	smooth2FBO = new FrameBufferObject(width, height, smooth_prog, fboTex, fboOutNames, attachLocations, vertex_array, vbo_indices, num_indices);
}

void setUpSmoothingKernelFBO()
{
	setUpSmoothPass1FBO();
	setUpSmoothPass2FBO();
	smoothKernelFBO = new SmoothKernelFBO(smooth1FBO, smooth2FBO);
}

void initNormalFBO()
{
	setUpNormalsFBO();
	setUpSmoothingKernelFBO();
}

void initErosionFBO()
{
	//Copy FBO to be used for copying
	//textures from source to destination
	setUpCopyFBO();

	// Initialization
	setUpInitializationFBO();

	// Water Increment
	attachTempTexToFBO(&waterIncFBO, "out_terrainAttr", water_inc_prog);

	// Flow Sim
	attachTempTexToFBO(&flowSimFluxFBO, "out_flux", flow_flux_prog);
	attachTempTexToFBO(&flowWatHeightFBO, "out_terrainAttrTex", flow_water_height_prog);
	attachTempTexToFBO(&flowSimVelFBO, "out_vel", flow_vel_prog);

	// Erosion Deposition
	attachTempTexToFBO(&erosDepoFBO, "out_terrainAttr", erosion_depo_prog);

	// Sediment Transport
	attachTempTexToFBO(&sedTransFBO, "out_terrainAttr", sediment_trans_prog);

	// Evaporation
	attachTempTexToFBO(&evapFBO, "out_terrainAttr", evapo_prog);

	//////////////
	// old


	// Water Increment
	//setUpWaterIncFBO();

	// Flow Sim
	//setUpFlowSimFluxFBO();
	//setUpFlowSimWaterHeightFBO();
	//setUpFlowSimVelocityFBO();

	// Erosion Deposition
	//setUpErosionDepoFBO();

	// Sediment Transport
	//setUpSedTransFBO();

	// Evaporation
	//setUpEvapFBO();
}



void deleteErosionFBO()
{
	delete copyFBO;
	delete initTerrainFBO;
	delete erosDepoFBO;
	delete evapFBO;
	delete flowSimFluxFBO;
	delete flowSimVelFBO;
	delete flowWatHeightFBO;
	delete sedTransFBO;
	delete waterIncFBO;
	delete normalMapFBO;
	delete smoothKernelFBO;
}

void initErosionShaders()
{
	fbo_test_prog = glslUtility::createProgram(vertFboTestPath, NULL, NULL, NULL, fragFboTestPath, attributeWithTexLocation, 2);
	terrain_init_prog = glslUtility::createProgram(vertTerrainTexInitPath, NULL, NULL, NULL, fragTerrainTexInitPath, attributeWithTexLocation, 2);
	erosion_depo_prog = glslUtility::createProgram(vertErosDepoPath, NULL, NULL, NULL, fragErosDepoPath, attributeWithTexLocation, 2);
	evapo_prog = glslUtility::createProgram(vertEvapPath, NULL, NULL, NULL, fragEvapPath, attributeWithTexLocation, 2);
	flow_flux_prog = glslUtility::createProgram(vertFlowSimFluxPath, NULL, NULL, NULL, fragFlowSimFluxPath, attributeWithTexLocation, 2);
	flow_vel_prog = glslUtility::createProgram(vertFlowSimVelPath, NULL, NULL, NULL, fragFlowSimVelPath, attributeWithTexLocation, 2);
	flow_water_height_prog = glslUtility::createProgram(vertFlowSimWatHeightPath, NULL, NULL, NULL, fragFlowSimWatHeightPath, attributeWithTexLocation, 2);
	sediment_trans_prog = glslUtility::createProgram(vertSedTransPath, NULL, NULL, NULL, fragSedTransPath, attributeWithTexLocation, 2);
	water_inc_prog = glslUtility::createProgram(vertWatIncPath, NULL, NULL, NULL, fragWatIncPath, attributeWithTexLocation, 2);
	copy_tex_prog = glslUtility::createProgram(vertCopyPath, NULL, NULL, NULL, fragCopyPath, attributeWithTexLocation, 2);
	water_shading_prog = glslUtility::createProgram(vertWaterPath, NULL, NULL, NULL, fragWaterPath, attributeWithTexLocation, 2);
	normalmap_prog = glslUtility::createProgram(nmapVertShaderPath, NULL, NULL, NULL, nmapFragShaderPath, attributeWithTexLocation, 2);
	smooth_intermediate_prog = glslUtility::createProgram(nmapVertShaderPath, NULL, NULL, NULL, smooth1FragShaderPath, attributeWithTexLocation, 2);
	smooth_prog = glslUtility::createProgram(nmapVertShaderPath, NULL, NULL, NULL, smoothFragShaderPath, attributeWithTexLocation, 2);
}

void initShader() {
	
	// set 1
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";
	const char * pass_tc =   "../../shaders/pass.tc";
	const char * pass_te =   "../../shaders/pass.te";
	
	std::cout << "Creating program." << std::endl;

	if (enableTexcoords)
	{
		plane->setIndexMode(INDEX_MODE::TRIANGLES);
		curr_prog = glslUtility::createProgram(pass_vert, NULL, NULL, NULL, pass_frag, attributeWithTexLocation, 2);
	}
	else
	{
		plane->setIndexMode(INDEX_MODE::QUADS);
		curr_prog = glslUtility::createProgram(vertQuadShaderPath, tessQuadCtrlShaderPath, tessQuadEvalShadePath, NULL, fragQuadShaderPath, attributeLocation, 1);
	}

	skybox_prog = glslUtility::createProgram(vertSkyboxShaderPath, NULL, NULL, NULL, fragSkyboxShaderPath, attributeLocation, 1);
}

void clearScene()
{
	delete plane;
	delete cam;
}

void drawQuad()
{
	glBindVertexArray(vertex_array);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT,0);
    glBindVertexArray(0);
}

void initQuad() 
{
	vec3 verts[] = {
		vec3(-1,1,0),
		vec3(-1,-1,0),
		vec3(1,-1,0),
		vec3(1,1,0) };

	vec2 texcoords[] = {
		vec2(0,1),
		vec2(0,0),
		vec2(1,0),
		vec2(1,1) };

    unsigned short indices[] = {0,1,2,0,2,3};

    //Allocate vertex array
    //Vertex arrays encapsulate a set of generic vertex attributes and the buffers they are bound too
    //Different vertex array per mesh.
    glGenVertexArrays(1, &(vertex_array));
    glBindVertexArray(vertex_array);

    //Allocate vbos for data
    glGenBuffers(1,&(vbo_data));
    glGenBuffers(1,&(vbo_indices));
	glGenBuffers(1,&(texcoord_data));

	glEnableVertexAttribArray(quad_attributes::POSITION);
    glEnableVertexAttribArray(quad_attributes::TEXCOORD);

    //Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_data);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glVertexAttribPointer(quad_attributes::POSITION, 3, GL_FLOAT, GL_FALSE,sizeof(vec3),0); 

	//Upload texture coordinates data
	glBindBuffer(GL_ARRAY_BUFFER, texcoord_data);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(quad_attributes::TEXCOORD, 2, GL_FLOAT, GL_FALSE,sizeof(vec2),0);

    //indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), indices, GL_STATIC_DRAW);
    num_indices = 6;

    //Unplug Vertex Array
    glBindVertexArray(0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    width = 1280;
    height = 720;
    glutInitWindowSize(width,height);
	char title[100];
	sprintf(title,"GAIA [%0.2f fps]", fps);
	glutCreateWindow(title);
    glewInit();
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        cout << "glewInit failed, aborting." << endl;
        exit (1);
    }
	
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << endl;
	
	glEnable(GL_DEPTH_TEST);

	initQuad();
	initTextures();
	initErosionTextures();
	initScene();
	initShader();
	initErosionShaders();
	initErosionFBO();
	initNormalFBO();

	// initialization of flex_tex, velocity_tex, and terrainattr_tex
	terrainInit();
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
	clearScene();
    return 0;
}