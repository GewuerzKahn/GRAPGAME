/* 
 * File:   OwnCube.cpp
 * Author: student
 * 
 * Created on September 17, 2014, 1:13 PM
 */

#include "OwnCube.h"

OwnCube::OwnCube() {

    angle = 0;

}

OwnCube::OwnCube(const OwnCube& orig) {
}

OwnCube::~OwnCube() {


}

void OwnCube::draw() {
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5
	};

	GLshort indices[] = {
		0, 3, 2, 1,
		2, 3, 7, 6,
		0, 4, 7, 3,
		1, 2, 6, 5,
		4, 5, 6, 7,
		0, 1, 5, 4
	};

	GLfloat normals[24] = {
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, -1.0,
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0
	};

	GLfloat cubeVertices[16 * 3] = {
		-0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, 0.5f,
		0.5f, 1.0f, 0.5f,
		-0.5f, 1.0f, 0.5f,
		-0.5f, 1.0f, -0.5f,
		0.5f, 1.0f, -0.5f,
		0.5f, 0.0f, -0.5f,
		-0.5f, 0.0f, -0.5f,
		0.5f, 0.0f, 0.5f,
		0.5f, 0.0f, -0.5f,
		0.5f, 1.0f, -0.5f,
		0.5f, 1.0f, 0.5f,
		-0.5f, 0.0f, -0.5f,
		-0.5f, 0.0f, 0.5f,
		-0.5f, 1.0f, 0.5f, -
		0.5f, 1.0f, -0.5f
	};

	GLfloat cubeTexcoords[8 * 4] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0
	};

	GLubyte cubeIndices[24] = { 0, 1, 2, 3, 4, 5, 6, 7, 3, 2, 5, 4, 7, 6, 1, 0,
		8, 9, 10, 11, 12, 13, 14, 15 };


    glPushMatrix();

    glScalef(scaleX, scaleY, scaleZ);

    glTranslatef(x, y, z);
    glRotatef(angleX, 0.0, 1.0, 0.0);
    glRotatef(angleY, 0.0, 0.0, 1.0);
    //    glRotatef(angleZ, 0.0, 0.0, 1.0);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, cubeTexcoords);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);

	glEnable(GL_TEXTURE_2D);

    for (int i = 0; i < 24; i += 4) {


        //TEXTURE STUFF
        GLfloat face[4];
        memcpy(face, cubeIndices + i, 4);

        

        if (0 == i || 4 == i) {
            glBindTexture(GL_TEXTURE_2D, textureId2);
        } else {
            glBindTexture(GL_TEXTURE_2D, textureId1);
        }

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glColor3f(1.0f, 1.0f, 1.0f);

        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, face);

    }
	glDisable(GL_TEXTURE_2D);

    glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    //glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}
//
//void OwnCube::setPosition(GLshort x, GLshort y, GLshort z) {
//
//    this->x = x;
//    this->y = y;
//    this->z = z;
//
//}

void OwnCube::setTextId(GLuint txt, GLuint txt2) {
    textureId1 = txt;
    textureId2 = txt2;
}


