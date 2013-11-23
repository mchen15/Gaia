#include "main.h"
#include "constants.h"
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

    glUseProgram(pass_prog);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model(1.0f);
	mat4 view = cam->getView();
	mat4 persp = cam->getPersp(float(width), float(height));
    mat4 inverse_transposed = glm::transpose(glm::inverse(view*model));
    glUniform1f(glGetUniformLocation(pass_prog, "u_Far"), cam->getFarPlane());
	glUniform1f(glGetUniformLocation(pass_prog, "u_Near"), cam->getNearPlane());
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Model"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_View"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Persp"),1,GL_FALSE,&persp[0][0]);
    glUniform1f(glGetUniformLocation(pass_prog, "u_tessLevelInner"), tessLevelInner);
	glUniform1f(glGetUniformLocation(pass_prog, "u_tessLevelOuter"), tessLevelOuter);
	
	int mode = plane->getIndexMode();
	plane->draw(triangle_attributes::POSITION, triangle_attributes::TEXCOORD);

	glutPostRedisplay();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) 
{
	switch(key) 
	{
		case(27):
			exit(0);
			break;
		case('w'):
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
	}
}

void initScene()
{
	vec3 camPosition = vec3(0, -10, 2);
	vec3 viewDir = vec3(0, 0, -1);
	vec3 up = vec3(0,0,1);
	float fov = 45.0f;
	float nearPlane = 0.01f;
	float farPlane = 100.0f;

	cam = new Camera(camPosition, viewDir, up,fov,nearPlane,farPlane);
	plane = new Plane(vec2(0), vec2(1), 20, 20); // LOOK: Our plane is from 0 to 1 with numPatches
}


void initShader() {
	
	// set 1
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";
	const char * pass_tc =   "../../shaders/pass.tc";
	const char * pass_te =   "../../shaders/pass.te";

	// set 2
	//const char * pass_vert = "../../shaders/pass.vert";
	//const char * pass_frag = "../../shaders/pass.frag";
	//const char * pass_tc =   "../../shaders/tessNoise.tc";
	//const char * pass_te =   "../../shaders/tessNoise.te";
	
	std::cout << "Creating program." << std::endl;

	pass_prog= glslUtility::createProgram(pass_vert, pass_tc, pass_te, NULL, pass_frag, attributeLocations,2);   
	//pass_prog= glslUtility::createProgram(pass_vert, pass_frag, attributeLocations,2);   
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