#include "main.h"
#include "Utility.h"
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


void display(void)
{
    float FARP = 100.0f;
    float NEARP = 0.1f;

    glUseProgram(pass_prog);


    mat4 model(1.0f);
	mat4 view = cam->getView();
    mat4 persp = glm::perspective(45.0f,(float)width/(float)height,NEARP,FARP);
    mat4 inverse_transposed = glm::transpose(glm::inverse(view*model));
    glUniform1f(glGetUniformLocation(pass_prog, "u_Far"), FARP);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Model"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_View"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Persp"),1,GL_FALSE,&persp[0][0]);


	plane->draw(0,1);
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
	}
}

void initScene()
{
	cam = new Camera(vec3(-10), vec3(0,0,-1), vec3(0,0,1));
	plane = new Plane();
}


void initShader() {
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";

	Utility::shaders_t shaders = Utility::loadShaders(pass_vert, pass_frag);
    pass_prog = glCreateProgram();

    glBindAttribLocation(pass_prog, 0, "Position");
    glBindAttribLocation(pass_prog, 1, "Texcoord");
    Utility::attachAndLinkProgram(pass_prog,shaders);
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    width = 1280;
    height = 720;
    glutInitWindowSize(width,height);
	glutCreateWindow("GAIA Frame");
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
    return 0;
}