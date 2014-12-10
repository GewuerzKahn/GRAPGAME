/* 
 * File:   OwnCube.h
 * Author: student
 *
 * Created on September 17, 2014, 1:13 PM
 */

#ifndef OWNCUBE_H
#define	OWNCUBE_H

#include "Globals.h"
#include <stdio.h>
#include <string.h>
#include "OwnObject.h"


class OwnCube : public OwnObject {
public:
    OwnCube();
    OwnCube(const OwnCube& orig);
    virtual ~OwnCube();

    void draw();
    void drawTexture();
    void setTextId(GLuint txt, GLuint txt2);

private:
    GLuint textureId1;
    GLuint textureId2;
    
    void setUpTexture();
    //
    //    GLfloat vertices[24] = {
    //        -0.5f, -0.5f, 0.5f,
    //        -0.5f, 0.5f, 0.5f,
    //        0.5f, 0.5f, 0.5f,
    //        0.5f, -0.5f, 0.5f,
    //        -0.5f, -0.5f, -0.5f,
    //        -0.5f, 0.5f, -0.5f,
    //        0.5f, 0.5f, -0.5f,
    //        0.5f, -0.5f, -0.5f
    //    };

	 GLfloat* vertices;

	 GLshort* indices;

	 GLfloat* normals;

	 GLfloat* cubeVertices;

	 GLfloat* cubeTexcoords;

	 GLubyte* cubeIndices;

};

#endif	/* OWNCUBE_H */

