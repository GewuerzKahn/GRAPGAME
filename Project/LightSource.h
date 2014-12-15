/* 
 * File:   LightSource.h
 * Author: student
 *
 * Created on September 18, 2014, 6:18 PM
 */

#ifndef LIGHTSOURCE_H
#define	LIGHTSOURCE_H

#include "OwnCube.h"

class LightSource : public OwnObject {
public:
    LightSource();
    LightSource(GLfloat x, GLfloat y, GLfloat z);
    LightSource(const LightSource& orig);
    virtual ~LightSource();

    void toggleMovingLight();

    void draw();
private:
    OwnCube *cube;

    bool isMovingLight = false;

    int minX = -3, maxX = 3;
    GLfloat lightSpeed = 0.02;
    GLfloat lightDirection = lightSpeed;

    void moveLight();

};

#endif	/* LIGHTSOURCE_H */

