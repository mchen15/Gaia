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
    glUseProgram(pass_prog);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 model(1.0f);
	mat4 view = cam->getView();
	mat4 persp = cam->getPersp(float(width), float(height));
    mat4 inverse_transposed = glm::transpose(glm::inverse(view*model));
    glUniform1f(glGetUniformLocation(pass_prog, "u_Far"), cam->getFarPlane());
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Model"),1,GL_FALSE,&model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_View"),1,GL_FALSE,&view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(pass_prog,"u_Persp"),1,GL_FALSE,&persp[0][0]);
	plane->draw(triangle_attributes::POSITION,triangle_attributes::TEXCOORD);
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
	plane = new Plane(vec2(-1), vec2(1), 5,5);
}


void initShader() {
	const char * pass_vert = "../../shaders/pass.vert";
	const char * pass_frag = "../../shaders/pass.frag";

	Utility::shaders_t shaders = Utility::loadShaders(pass_vert, pass_frag);
    pass_prog = glCreateProgram();

    glBindAttribLocation(pass_prog, triangle_attributes::POSITION, "Position");
    glBindAttribLocation(pass_prog, triangle_attributes::TEXCOORD, "Texcoord");
    Utility::attachAndLinkProgram(pass_prog,shaders);
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
	clearScene();
    return 0;
}