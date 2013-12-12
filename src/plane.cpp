#include "plane.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_operation.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/verbose_operator.hpp>
#include <iostream>


Plane::Plane(glm::vec2 l, glm::vec2 u, int subdivisionsX, int subdivisionsY)
	:lCorner(l),
	 uCorner(u),
	 divx(subdivisionsX+1),
	 divy(subdivisionsY+1),
	 wireframe(false),
	 indexingMode(INDEX_MODE::QUADS)
{
	initVAO();
}


Plane::~Plane()
{
	deleteVAOs();
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
	GLuint *qIndices    = new GLuint[4*num_faces];
	//Texture co-ordinates
	glm::vec2 texL(0.0,0.0);
	glm::vec2 texU(1.0,1.0);
    
	glm::vec4 lr(lCorner.x,lCorner.y,texL.x,texL.y);
    glm::vec4 ul(uCorner.x,uCorner.y,texU.x,texU.y);

	//glm::mat4 debug = glm::lookAt(glm::vec3(0,-10,2), glm::vec3(0.0), glm::vec3(0,0,1));

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

			//glm::vec4 test = debug*glm::vec4( alpha*lr.x + (1-alpha)*ul.x,beta*lr.y + (1-beta)*ul.y,0.0,1.0);
			//float debug2 = abs(test.z)/30.0f; 
			//std::cout<< test.x<<" "<<test.y<<" "<<test.z<<" "<<debug2<<std::endl;
		}
    }

	// set up triangle indices
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

            qIndices[4*(i+(j*fw_1))    ] = divx*j + i;
            qIndices[4*(i+(j*fw_1)) + 1] = divx*j + i + 1;
            qIndices[4*(i+(j*fw_1)) + 3] = divx*(j+1) + i;
            qIndices[4*(i+(j*fw_1)) + 2] = divx*(j+1) + i + 1;		

        }
    }

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &t_ibo);
	glGenBuffers(1, &q_ibo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2*num_verts*sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, 2*num_verts*sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*num_faces*sizeof(GLuint), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, q_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*num_faces*sizeof(GLuint), qIndices, GL_STATIC_DRAW);

    delete[] vertices;
    delete[] texcoords;
    delete[] indices;
	delete[] qIndices;
}

void Plane::draw(int positionLocation)
{
    glEnableVertexAttribArray(positionLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer((GLuint)positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 

	
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	if (indexingMode == INDEX_MODE::TRIANGLES)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_ibo);
		glPatchParameteri(GL_PATCH_VERTICES, 3);
		//glDrawElements(GL_PATCHES, 6*divx*divy,  GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6*divx*divy,  GL_UNSIGNED_INT, 0);
	}
	if (indexingMode == INDEX_MODE::QUADS)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, q_ibo);
		glPatchParameteri(GL_PATCH_VERTICES, 4);
		//glDrawElements(GL_PATCHES, 6*divx*divy,  GL_UNSIGNED_INT, 0);
		glDrawElements(GL_PATCHES, 4*(divx-1)*(divy-1),  GL_UNSIGNED_INT, 0);
	}
	else
	{
		glPatchParameteri(GL_PATCH_VERTICES, NUM_QUADS);
		glDrawArrays(GL_PATCHES, 0, divx*divy);
	}

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDisableVertexAttribArray(positionLocation);
}

// for debugging textures
void Plane::draw(int positionLocation, int texCoordsLocation)
{
    glEnableVertexAttribArray(positionLocation);
    glEnableVertexAttribArray(texCoordsLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer((GLuint)positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 

	glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glVertexAttribPointer((GLuint)texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	
	//if(wireframe)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	if (indexingMode == INDEX_MODE::TRIANGLES)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_ibo);
		//glPatchParameteri(GL_PATCH_VERTICES, 3);
		//glDrawElements(GL_PATCHES, 6*divx*divy,  GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6*divx*divy,  GL_UNSIGNED_INT, 0);
	}
	else
	{
		glPatchParameteri(GL_PATCH_VERTICES, NUM_QUADS);
		glDrawArrays(GL_PATCHES, 0, divx*divy);
	}

    glDisableVertexAttribArray(positionLocation);
	glDisableVertexAttribArray(texCoordsLocation);
}

// clean up code
void Plane::deleteVAOs()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &t_ibo);
	glDeleteBuffers(1, &tbo);
}

void Plane::toggleWireframe()
{
	wireframe = !wireframe;
}

void Plane::toggleIndexingMode()
{	
	if (indexingMode == INDEX_MODE::TRIANGLES)
		indexingMode = INDEX_MODE::QUADS;
	else
		indexingMode = INDEX_MODE::TRIANGLES;
}

int Plane::getIndexMode()
{
	return indexingMode;
}

void Plane::setIndexMode(int mode)
{
	indexingMode = mode;
}
