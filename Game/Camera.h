#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>

class Camera
{
public:
	//get:
	glm::vec3 getCamera();
	glm::vec3 getCameraFast();
	glm::vec3 getBasicCam();
	float getScale();
	float getVerAngle();
	float getHorAngle();
	float getTiltAngle();
	//set:
	void setScale(float scale);
	void setCamVerAngle(float angle);
	void setCamHorAngle(float angle);
	void setCamTiltAngle(float angle);
	//reset:
	void resetCamera();
	void resetCamVec();
	void resetScale();
	void resetCamVerAngle();
	void resetCamHorAngle();
	void resetCamTiltAngle();
	//rotate:
	void rotateVer(float angle);
	void rotateHor(float angle);
	
	glm::vec3 cam = basicCam;
	float scale = 1.0f;
	float camVerAngle = 0.0f;
	float camHorAngle = 0.0f;
	float camTiltAngle = 0.0f;

private:
	glm::vec3 basicCam = glm::vec3(0.0001, 42.0, 0.0);

};