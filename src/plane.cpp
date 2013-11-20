#include "plane.h"

Plane::Plane(glm::vec2 l, glm::vec2 u, int subdivisionsX, int subdivisionsY)
	:lCorner(l),
	 uCorner(u),
	 divx(subdivisionsX),
	 divy(subdivisionsY),
	 wireframe(false)
{
	initVAO();
}


Plane::~Plane()
{
}

void Plane::initVAO()
{
    const int fw_1 = divx-1;
    const int fh_1 = divy-1;

    int num_verts = divx*divy;
    int num_faces = fw_1*fh_1;

    GLfloat *vertices  = new GLfloat[2*num_verts];
    GLfloat *texcoords = new GLfloat[2*num_verts]; 
    GLuint *indices    = new GLuint[6*num_faces];

	//Texture co-ordinates
	glm::vec2 texL(0.0,0.0);
	glm::vec2 texU(1.0,1.0);
    
	glm::vec4 lr(lCorner.x,lCorner.y,texL.x,texL.y);
    glm::vec4 ul(uCorner.x,uCorner.y,texU.x,texU.y);


    for(int i = 0; i < divx; ++i)
    {
        for(int j = 0; j < divy; ++j)
        {
            float alpha = float(i) / float(fw_1);
            float beta = float(j) / float(fh_1);
            vertices[(j*divx + i)*2  ] = alpha*lr.x + (1-alpha)*ul.x;
            vertices[(j*divx + i)*2+1] = beta*lr.y + (1-beta)*ul.y;
            texcoords[(j*divx + i)*2  ] = alpha*lr.z + (1-alpha)*ul.z;
            texcoords[(j*divx + i)*2+1] = beta*lr.w + (1-beta)*ul.w;
        }
    }

    for(int i = 0; i < fw_1; ++i)
    {
        for(int j = 0; j < fh_1; ++j)
        {
            indices[6*(i+(j*fw_1))    ] = divx*j + i;
            indices[6*(i+(j*fw_1)) + 1] = divx*j + i + 1;
            indices[6*(i+(j*fw_1)) + 2] = divx*(j+1) + i;
            indices[6*(i+(j*fw_1)) + 3] = divx*(j+1) + i;
            indices[6*(i+(j*fw_1)) + 4] = divx*(j+1) + i + 1;
            indices[6*(i+(j*fw_1)) + 5] = divx*j + i + 1;
        }
    }


    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ibo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2*num_verts*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 2*num_verts*sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*num_faces*sizeof(GLuint), indices, GL_STATIC_DRAW);

    delete[] vertices;
    delete[] texcoords;
    delete[] indices;
}

void Plane::draw(int positionLocation, int texcoordsLocation)
{
    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(texcoordsLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer((GLuint)positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glVertexAttribPointer((GLuint)texcoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glDrawElements(GL_TRIANGLES, 6*divx*divy,  GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(texcoordsLocation);
}

void Plane::toggleWireframe()
{
	wireframe = !wireframe;
}