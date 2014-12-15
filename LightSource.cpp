/* 
 * File:   LightSource.cpp
 * Author: student
 * 
 * Created on September 18, 2014, 6:18 PM
 */

#include "LightSource.h"
#include "OwnObject.h"

LightSource::LightSource() {
    cube = new OwnCube();
    cube->setPosition(0, 2, 0);
    cube->setScale(0.2f, 0.2f, 0.2f);

    setPosition(0, 2, 0);
}

LightSource::LightSource(GLfloat x, GLfloat y, GLfloat z) {
    cube = new OwnCube();
    cube->setPosition(x, y, z);
    cube->setScale(0.2, 0.2, 0.2);

    setPosition(x, y, z);

}

LightSource::LightSource(const LightSource& orig) {
}

LightSource::~LightSource() {
}

void LightSource::moveLight() {
    GLfloat positionLightX, positionLightY, positionLightZ;

    getPosition(positionLightX, positionLightY, positionLightZ);

    if (positionLightX < -3) {
        lightDirection = lightSpeed;
    } else
        if (positionLightX > 3) {
        lightDirection = -lightSpeed;
    }

    printf("--- %4.2f \n", positionLightX);

    setPosition(positionLightX + lightDirection, positionLightY, positionLightZ);

}

void LightSource::draw() {


    if (isMovingLight) {
       moveLight();
    }
  
	/*
	GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat diffuse[] = {100.0, 100.0, 100.0, 1.0};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat spotDirection[] = {-1.0, -1.0, 0.0};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2);
	*/
	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };  // Ambient Light Values
	GLfloat LightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };  // Diffuse Light Values
	GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Light Position
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);      // Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);      // Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);    // Position The Light
	glEnable(GL_LIGHT1);   // Enable Light One
	
	GLfloat light_position[] = { x, y, z, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    cube->setPosition(x, y, z);
    //cube->draw();

}

void LightSource::toggleMovingLight() {
    if (isMovingLight) {
        isMovingLight = false;
    } else {
        isMovingLight = true;
    }
}

