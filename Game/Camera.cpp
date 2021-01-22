#include "Camera.h"

glm::vec3 Camera::getCamera() {
    cam = basicCam;
    cam = glm::rotate(cam, glm::radians(camVerAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    cam = glm::rotate(cam, glm::radians(camHorAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    return cam;
}

glm::vec3 Camera::getCameraFast()
{
    return cam;
}

glm::vec3 Camera::getBasicCam(){
    return basicCam;
}

float Camera::getScale(){
    return scale;
}

float Camera::getVerAngle(){
    return camVerAngle;
}

float Camera::getHorAngle(){
    return camHorAngle;
}

float Camera::getTiltAngle(){
    return camTiltAngle;
}

void Camera::setScale(float scale){
    this->scale = scale;
}

void Camera::setCamVerAngle(float angle){
    camVerAngle = angle;
    if (camVerAngle <= -90.0f) {
        camVerAngle = -89.99f;
    }
    else if (camVerAngle >= 90.0f) {
        camVerAngle = 89.99f;
    }
}

void Camera::setCamHorAngle(float angle){
    camHorAngle = angle;
}

void Camera::setCamTiltAngle(float angle){
    camTiltAngle = angle;
}

void Camera::resetCamera() {
    cam = basicCam;
    scale = 1.0f;
    camVerAngle = 0.0f;
    camHorAngle = 0.0f;
    camTiltAngle = 0.0f;
}

void Camera::resetCamVec(){
    cam = basicCam;
}

void Camera::resetScale(){
    scale = 1.0f;
}

void Camera::resetCamVerAngle(){
    camVerAngle = 0.0f;
}

void Camera::resetCamHorAngle(){
    camHorAngle = 0.0f;
}

void Camera::resetCamTiltAngle(){
    camTiltAngle = 0.0f;
}

void Camera::rotateVer(float angle){
    camVerAngle += angle;
    if (camVerAngle <= -90.0f) {
        camVerAngle = -89.99f;
    }
    else if (camVerAngle >= 90.0f) {
        camVerAngle = 89.99f;
    }
}

void Camera::rotateHor(float angle){
    camHorAngle += angle;
    while (camHorAngle >= 360.0f) {
        camHorAngle -= 360.0f;
    }
    while (camHorAngle < 0.0f) {
        camHorAngle += 360.0f;
    }
}
