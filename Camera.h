/* 
 * File:   Camera.h
 * Author: student
 *
 * Created on September 17, 2014, 5:27 PM
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "Globals.h"

using namespace std;

class Camera {
public:
    Camera();
    Camera(const Camera& orig);
    virtual ~Camera();

    void refresh();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setYaw(GLfloat angle);
    void setPitch(GLfloat angle);

    void getPosition(GLfloat &x, GLfloat &y, GLfloat &z);
    void getDirection(GLfloat &x, GLfloat &y, GLfloat &z);
    void getPitch(GLfloat &angle);
    void getYaw(GLfloat &angle);
    
    void move(GLfloat incr);
    void strafe(GLfloat incr);
    void fly(GLfloat incr);
    void rotateYaw(GLfloat angle);
    void rotatePitch(GLfloat angle);


private:

    //Camera stuff
    GLfloat cameraDirectionX, cameraDirectionY, cameraDirectionZ;
    GLfloat cameraPositionX, cameraPositionY, cameraPositionZ;
    GLfloat cameraAngleYaw, cameraAnglePitch;
    GLfloat strafeL, strafeR;

//    float m_x, m_y, m_z; // Position
//    float m_lx, m_ly, m_lz; // Direction vector of where we are looking at
//    float m_yaw, m_pitch; // Various rotation angles
//    float m_strafe_lx, m_strafe_lz; // Always 90 degree to direction vector
};

#endif	/* CAMERA_H */

