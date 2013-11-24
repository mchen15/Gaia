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

void display(void)
{
	updateFPS();

    glUseProgram(curr_prog);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	setUniforms();
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, heightmap_tex);

#if ENABLE_TEXCOORDS == 1
	plane->setIndexMode(INDEX_MODE::TRIANGLES);
	plane->draw(triangle_attributes::POSITION, triangle_attributes::TEXCOORD);
#else
	plane->draw(triangle_attributes::POSITION);
#endif

	glutPostRedisplay();
    glutSwapBuffers();
}

void setUniforms()
{
	mat4 model(1.0f);
	mat4 view = cam->getView();
	mat4 persp = cam->getPersp(float(width), float(height));
    mat4 inverse_transposed = glm::transpose(glm::inverse(view*model));

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
		//glUniformMatrix4fv(uniformLocation,1, GL_TRUE, &imv[0][0]);
		glUniformMatrix4fv(uniformLocation,1, GL_FALSE, &inverse_transposed[0][0]);
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

	uniformLocation = glGetUniformLocation(curr_prog,U_LODFACTOR);
	if (uniformLocation != -1)
	{
		glUniform1f(uniformLocation, lodFactor);
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
			tessLevelInner--;
			std::cout<<"Inner: "<<tessLevelInner<<std::endl;
			break;
		case ('3'):
			tessLevelOuter++;
			std::cout<<"Outer: "<<tessLevelOuter<<std::endl;
			break;
		case ('4'):
			tessLevelOuter--;
			std::cout<<"Outer: "<<tessLevelOuter<<std::endl;
			break;
		case ('m'):
			plane->toggleIndexingMode();
			break;
		case ('w'):
			cam->setPosition( cam->getPosition()+ vec3(0,0.1,0));
			cam->setLookAtPoint(cam->getLookAtPoint() + vec3(0,0.1,0));
			break;
		case ('s'):
			cam->setPosition( cam->getPosition()- vec3(0,0.1,0));
			cam->setLookAtPoint(cam->getLookAtPoint() - vec3(0,0.1,0));
			break;
		case ('a'):
			cam->setPosition( cam->getPosition()- vec3(0.1,0,0));
			cam->setLookAtPoint(cam->getLookAtPoint() - vec3(0.1,0,0));
			break;
		case ('d'):
			cam->setPosition( cam->getPosition()+vec3(0.1,0,0));
			cam->setLookAtPoint(cam->getLookAtPoint() + vec3(0.1,0,0));
			break;
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
}

void initScene()
{
	vec3 camPosition = vec3(0, -200,100);
	vec3 lookAtPoint = vec3(512,512,0);
	vec3 up = vec3(0,0,1);
	float fov = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 1500.0f;

	cam = new Camera(camPosition, lookAtPoint, up,fov,nearPlane,farPlane);
	plane = new Plane(vec2(0), vec2(1), SUBDIV.x, SUBDIV.y); // LOOK: Our plane is from 0 to 1 with numPatches
	//plane = new Plane(vec2(-10), vec2(10), 10, 10);
}


void initShader() {
	
	// set 1
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";
	const char * pass_tc =   "../../shaders/pass.tc";
	const char * pass_te =   "../../shaders/pass.te";
	
	std::cout << "Creating program." << std::endl;

#if ENABLE_TEXCOORDS
	curr_prog = glslUtility::createProgram(pass_vert, NULL, NULL, NULL, pass_frag, attributeWithTexLocation, 2);
#else
	//curr_prog = glslUtility::createProgram(pass_vert, NULL, NULL, NULL, pass_frag, attributeWithTexLocation, 1);
	curr_prog = glslUtility::createProgram(vertShaderPath, tessCtrlShaderPath, tessEvalShadePath, NULL, fragShaderPath, attributeLocation, 1);
#endif
	
	//curr_prog = glslUtility::createProgram(vertShaderPath, tessCtrlShaderPath, tessEvalShadePath, NULL, fragShaderPath, attributeLocation, 1);
}

void clearScene()
{
	delete plane;
	delete cam;
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
	
	initTextures();
	initScene();
	initShader();
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);

    glutMainLoop();
	clearScene();
    return 0;
}