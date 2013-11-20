#include "main.h"


void reshape(int w, int h)
{
    width = w;
    height = h;
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void display(void)
{
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

void initCamera()
{
	cam = Camera(vec3(-10), vec3(0,0,-1), vec3(0,1,0));
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
	
	initCamera();
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}