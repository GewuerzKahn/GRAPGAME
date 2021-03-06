/* 
 * File:   OwnObject.h
 * Author: student
 *
 * Created on September 17, 2014, 9:31 PM
 */

#include "Globals.h"

#include "enums.h"

using namespace glm;

#ifndef OWNOBJECT_H
#define	OWNOBJECT_H

class OwnObject {
public:
    OwnObject();
    OwnObject(const OwnObject& orig);
    virtual ~OwnObject();

	virtual void draw();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void getPosition(GLfloat& x, GLfloat& y, GLfloat& z);
	void setPosition(vec3 p);
	vec3 getPosition();
    void setScale(GLfloat x, GLfloat y, GLfloat z);
    void setMaterial(EnumMaterial m);
    void setAngle(GLfloat x, GLfloat y, GLfloat z);
    void getAngle(GLfloat& x, GLfloat& y, GLfloat& z);
	void setSize(vec3 s);
	vec3 getSize();
	bool isDead();
	void setDeadState(bool isDeadState);

protected:
    GLfloat angle;
    GLfloat x, y, z;
	vec3 position;
    GLfloat scaleX, scaleY, scaleZ;
    GLfloat angleX, angleY, angleZ;
	vec3 objectSize;
	bool isDeadState = false;

    typedef struct materialStruct {
        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat shininess;
    } materialStruct;

    materialStruct material;

	materialStruct brassMaterials;

	materialStruct redPlasticMaterials;

	materialStruct emeraldMaterials;

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

