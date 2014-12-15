/* 
 * File:   OwnObject.cpp
 * Author: student
 * 
 * Created on September 17, 2014, 9:31 PM
 */

#include "OwnObject.h"

OwnObject::OwnObject() {
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    angleX = 0;
    angleY = 0;
    angleZ = 0;


	 brassMaterials = {
		{ 0.33, 0.22, 0.03, 1.0 },
		{ 0.78, 0.57, 0.11, 1.0 },
		{ 0.99, 0.91, 0.81, 1.0 },
		27.8
	};

	 redPlasticMaterials = {
		{ 0.3, 0.0, 0.0, 1.0 },
		{ 0.6, 0.0, 0.0, 1.0 },
		{ 0.8, 0.6, 0.6, 1.0 },
		32.0
	};

	emeraldMaterials = {
		{ 0.0215, 0.1745, 0.0215, 1.0 },
		{ 0.07568, 0.61424, 0.07568, 1.0 },
		{ 0.633, 0.727811, 0.633, 1.0 },
		50.6
	 };
	  	 	 	 	 	 	 	 	 	
	 noMaterials = {
		{ 0.2, 0.2, 0.2, 1.0 },
		{ 1.0, 1.0, 1.0, 1.0 },
		{ 0.0, 0.0, 0.0, 0.0 },
		0
	};

	material = noMaterials;

}

OwnObject::OwnObject(const OwnObject& orig) {
}

OwnObject::~OwnObject() {
}

void OwnObject::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void OwnObject::getPosition(GLfloat& x, GLfloat& y, GLfloat& z) {
    x = this->x;
    y = this->y;
    z = this->z;
}

void OwnObject::setScale(GLfloat x, GLfloat y, GLfloat z) {
    this->scaleX = x;
    this->scaleY = y;
    this->scaleZ = z;
}

void OwnObject::setMaterial(EnumMaterial m) {
    switch (m) {
        case EnumMaterial::NO_MATERIAL:
            material = noMaterials;
            break;
        case EnumMaterial::BRASS:
            material = brassMaterials;
            break;
        case EnumMaterial::RED_PLASTIK:
            material = redPlasticMaterials;
            break;
		case EnumMaterial::EMERALD:
			material = emeraldMaterials;
			break;
    }
}

void OwnObject::setAngle(GLfloat x, GLfloat y, GLfloat z) {
    this->angleX = x;
    this->angleY = y;
    this->angleZ = z;
}

void OwnObject::getAngle(GLfloat& x, GLfloat& y, GLfloat& z) {
    x = this->angleX;
    y = this->angleY;
    z = this->angleZ;
}


bool OwnObject::isDead(){
	return isDeadState;
}

void OwnObject::setDeadState(bool isD){
	isDeadState = isD;
}

void OwnObject::setSize(vec3 s){
	objectSize = s;
}

vec3 OwnObject::getSize(){
	return objectSize;
}

void OwnObject::draw(){

}

vec3 OwnObject::getPosition(){
	return position;
}

void OwnObject::setPosition(vec3 p){
	position = p;
}