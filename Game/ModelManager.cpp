#include "ModelManager.h"

void ModelManager::rotateModel(glm::mat4* mRot, float xR, float yR, float zR){
	if (xR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(xR), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (yR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(yR), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (zR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(zR), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void ModelManager::rotateModelX(glm::mat4* mRot, float xR){
	*mRot = glm::rotate(*mRot, glm::radians(xR), glm::vec3(1.0f, 0.0f, 0.0f));
}

void ModelManager::rotateModelY(glm::mat4* mRot, float yR) {
	*mRot = glm::rotate(*mRot, glm::radians(yR), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ModelManager::rotateModelZ(glm::mat4* mRot, float zR) {
	*mRot = glm::rotate(*mRot, glm::radians(zR), glm::vec3(0.0f, 0.0f, 1.0f));
}

void ModelManager::setModelRotation(glm::mat4* mRot, float xR, float yR, float zR){
	*mRot = glm::mat4(1.0f);
	if (xR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(xR), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (yR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(yR), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (zR != 0) {
		*mRot = glm::rotate(*mRot, glm::radians(zR), glm::vec3(0.0f, 0.0f, 1.0f));
	}
}

void ModelManager::resetModelRotation(glm::mat4* mRot){
	*mRot = glm::mat4(1.0f);
}

void ModelManager::translateModel(glm::vec3* mPos, float x, float y, float z){
	*mPos += glm::vec3(x, y, z);
}

void ModelManager::translateModel(glm::vec3* mPos, glm::vec3 vec){
	*mPos += vec;
}
