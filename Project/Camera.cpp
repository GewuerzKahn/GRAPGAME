/* 
 * File:   Camera.cpp
 * Author: student
 * 
 * Created on September 17, 2014, 5:27 PM
 */

#include "Camera.h"

Camera::Camera() {

    cameraAngleYaw = 0.0f;
    cameraAnglePitch = 0.0f;

    setPosition(5.0f, 5.0f, 5.0f);
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

void Camera::refresh() {

    cameraDirectionX = cos(cameraAngleYaw) * cos(cameraAnglePitch);
    cameraDirectionY = sin(cameraAnglePitch);
    cameraDirectionZ = sin(cameraAngleYaw) * cos(cameraAnglePitch);

    strafeL = cos(cameraAngleYaw - M_PI_2);
    strafeR = sin(cameraAngleYaw - M_PI_2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
            cameraPositionX, cameraPositionY, cameraPositionZ,
            cameraPositionX + cameraDirectionX, cameraPositionY + cameraDirectionY, cameraPositionZ + cameraDirectionZ,
            0.0f, 1.0f, 0.0f);


}

void Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    cameraPositionX = x;
    cameraPositionY = y;
    cameraPositionZ = z;

    refresh();
}

void Camera::getPosition(GLfloat& x, GLfloat& y, GLfloat& z) {
    x = cameraPositionX;
    y = cameraPositionY;
    z = cameraPositionZ;
}

void Camera::getDirection(GLfloat& x, GLfloat& y, GLfloat& z) {
    x = cameraDirectionX;
    y = cameraDirectionY;
    z = cameraDirectionZ;
}

void Camera::move(float incr) {
    float directionX = cos(cameraAngleYaw) * cos(cameraAnglePitch);
    float directionY = sin(cameraAnglePitch);
    float directionZ = sin(cameraAngleYaw) * cos(cameraAnglePitch);

    cameraPositionX += incr*directionX;
    cameraPositionY += incr*directionY;
    cameraPositionZ += incr*directionZ;

    refresh();
}

void Camera::strafe(float incr) {
    cameraPositionX += incr*strafeL;
    cameraPositionZ += incr*strafeR;

    refresh();
}

void Camera::fly(float incr) {
    cameraPositionY += incr;

    refresh();
}

void Camera::rotateYaw(GLfloat angle) {
    cameraAngleYaw += angle;

    refresh();
}

void Camera::rotatePitch(GLfloat angle) {
    const GLfloat limit = 89.0f * M_PI / 180.0f;

    cameraAnglePitch += angle;

    if (cameraAnglePitch < -limit)
        cameraAnglePitch = -limit;

    if (cameraAnglePitch > limit)
        cameraAnglePitch = limit;

    refresh();
}

void Camera::setYaw(float angle) {
    cameraAngleYaw = angle;

    refresh();
}

void Camera::setPitch(float angle) {
    cameraAnglePitch = angle;

    refresh();
}

void Camera::getPitch(GLfloat& angle) {
    angle = cameraAnglePitch;
}

void Camera::getYaw(GLfloat& angle) {
    angle = cameraAngleYaw;
}
