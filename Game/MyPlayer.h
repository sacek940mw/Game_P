#pragma once

#include "Camera.h"
#include "ModelManager.h"

#include <cstdint>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct moveKeysPressed {
	bool w = false, a = false, s = false, d = false;
	bool q = false, e = false;
};

struct playerPos {
	glm::vec3 posVec = glm::vec3(0.0f, 0.0f, 0.0f);
	int32_t xx = 0 , yy = 0, zz = 0;
	float verAngle = 0.0f, horAngle = 0.0f, tiltAngle = 0.0f;
};

struct playerInfo {
	playerPos plPos;	
	Camera* plCam = new Camera();
	float speed = 0.0f;
};

class MyPlayer
{
public:
	MyPlayer();
	MyPlayer(float x, float y, float z, int32_t xx, int32_t yy, int32_t zz, float verAngle, float horAngle, float tiltAngle, float speed, ModelInfo mi);
	//set:
	void setPos(float x, float y, float z, int32_t xx, int32_t yy, int32_t zz, float verAngle, float horAngle, float tiltAngle);
	void setRotation(float verAngle, float horAngle, float tiltAngle);
	void setPos(float x, float y, float z);
	void setPos(int32_t xx, int32_t yy, int32_t zz);
	void setSpeed(float speed);
	void setModelN(ModelN& m);
	void setCordChanged(bool change);
	void setOldInfo();
	//reset:
	void resetPlInfo();
	void resetPos();
	void resetPlCam();
	void resetSpeed();
	//get:
	float getPlSpeed();
	playerInfo getPlInfo();
	playerPos getPlPos();
	glm::vec3 getPlPosVec();
	Camera* getPlCam();
	ModelManager* getObjectManager();
	ModelInfo* getModelInfo();
	ModelN* getModelN();
	bool getCordChanged();
	playerInfo getOldInfo();
	//move:
	void rotatePlayer(float ver, float hor, float tilt);
	void translatePlayer(glm::vec3 trVec);

private:
	playerInfo plInfo;
	playerInfo oldInfo;
	ModelManager mm;
	ModelInfo mi;
	ModelN modeln;
	bool cordChanged = true;
};

