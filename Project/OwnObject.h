/* 
 * File:   OwnObject.h
 * Author: student
 *
 * Created on September 17, 2014, 9:31 PM
 */

#include "Globals.h"

#include "enums.h"

#ifndef OWNOBJECT_H
#define	OWNOBJECT_H

class OwnObject {
public:
    OwnObject();
    OwnObject(const OwnObject& orig);
    virtual ~OwnObject();

    void draw();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void getPosition(GLfloat& x, GLfloat& y, GLfloat& z);
    void setScale(GLfloat x, GLfloat y, GLfloat z);
    void setMaterial(EnumMaterial m);
    void setAngle(GLfloat x, GLfloat y, GLfloat z);
    void getAngle(GLfloat& x, GLfloat& y, GLfloat& z);


protected:
    GLfloat angle;
    GLfloat x, y, z;
    GLfloat scaleX, scaleY, scaleZ;
    GLfloat angleX, angleY, angleZ;

    typedef struct materialStruct {
        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat shininess;
    } materialStruct;

    materialStruct material;

	materialStruct brassMaterials;

	materialStruct redPlasticMaterials;

	materialStruct noMaterials;

    void materials(materialStruct *materials) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials->ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials->diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials->specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials->shininess);
    }



private:




};

#endif	/* OWNOBJECT_H */

