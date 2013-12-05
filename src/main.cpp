#include "main.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_operation.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/verbose_operator.hpp>

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

void display(void)
{
	updateFPS();

	if (genNormalMap)
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
		drawQuad();
	}
	else if (enableErosion) // temporarily have erosion as a completely different part of our pipeline for debugging purposes
	{
		// ------------------------------------------------------------------
		// initialization of flex_tex, velocity_tex, and terrainattr_tex
		unbindTextures();
		bindFBO(initTerrainFBO->getFBOHandle());
		//glBindFramebuffer(GL_FRAMEBUFFER, initTerrainFBO->getFBOHandle());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(terrain_init_prog);
		drawQuad();









		// Testing fbo: bind the default framebuffer to render to screen
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE, GL_ONE);
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
		uniformLocation = glGetUniformLocation(fbo_test_prog, U_VELOCITYTEXID);
		glUniform1i(uniformLocation, 5);

		drawQuad();

//		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
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

	glutPostRedisplay();
    glutSwapBuffers();
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
		glBindTexture(GL_TEXTURE_2D, heightmap_tex);
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

	}
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        mouse_buttons |= 1<<button;
    } else if (state == GLUT_UP) {
        mouse_buttons = 0;
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
}

void initScene()
{
	// camera set up for quads rendering using 2nd technique
	vec3 camPosition = vec3(0, -200,400);
	vec3 lookAtPoint = vec3(512,512,0);

	// camera set up for lower corner technique
	//vec3 camPosition = vec3(0, -3, 500);
	//vec3 lookAtPoint = vec3(0,0,0);


	vec3 up = vec3(0,0,1);
	float fov = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 5000.0f;

	cam = new Camera(camPosition, lookAtPoint, up,fov,nearPlane,farPlane);
	plane = new Plane(vec2(0), vec2(1), SUBDIV.x, SUBDIV.y); // LOOK: Our plane is from 0 to 1 with numPatches
	//plane = new Plane(vec2(-10), vec2(10), 10, 10);
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

void initErosionTextures()
{
	glGenTextures(1, &flux_tex);
	glGenTextures(1, &terrainattr_tex);
	glGenTextures(1, &velocity_tex);

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
}

void initErosionFBO()
{
	// Initialization
	setUpInitializationFBO();

	// Water Increment

	// Erosion Deposition

	// Sediment Transport

	// Evaporation

}

void deleteErosionFBO()
{
	delete initTerrainFBO;
}

void initErosionShaders()
{
	fbo_test_prog = glslUtility::createProgram(vertFboTestPath, NULL, NULL, NULL, fragFboTestPath, attributeWithTexLocation, 2);
	terrain_init_prog = glslUtility::createProgram(vertTerrainTexInitPath, NULL, NULL, NULL, fragTerrainTexInitPath, attributeLocation, 1);
	erosion_depo_prog;
	evapo_prog;
	flow_flux_prog;
	flow_vel_prog;
	flow_water_height_prog;
	sediment_trans_prog;
	water_inc_prog;
}

void initShader() {
	
	// set 1
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";
	const char * pass_tc =   "../../shaders/pass.tc";
	const char * pass_te =   "../../shaders/pass.te";
	
	std::cout << "Creating program." << std::endl;

	if (genNormalMap)
	{
		curr_prog = glslUtility::createProgram(nmapVertShaderPath, NULL, NULL, NULL, nmapFragShaderPath, attributeWithTexLocation, 2);
	}
	else
	{
		if (enableTexcoords)
		{
			plane->setIndexMode(INDEX_MODE::TRIANGLES);
			curr_prog = glslUtility::createProgram(pass_vert, NULL, NULL, NULL, pass_frag, attributeWithTexLocation, 2);
		}
		else
		{
			//plane->setIndexMode(INDEX_MODE::CORNER);
			//curr_prog = glslUtility::createProgram(pass_vert, NULL, NULL, NULL, pass_frag, attributeWithTexLocation, 1);

			plane->setIndexMode(INDEX_MODE::QUADS);
			curr_prog = glslUtility::createProgram(vertQuadShaderPath, tessQuadCtrlShaderPath, tessQuadEvalShadePath, NULL, fragQuadShaderPath, attributeLocation, 1);
		}
		//curr_prog = glslUtility::createProgram(vertShaderPath, tessCtrlShaderPath, tessEvalShadePath, NULL, fragShaderPath, attributeLocation, 1);
	}
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
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
	clearScene();
    return 0;
}