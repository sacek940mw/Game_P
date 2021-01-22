#include "Engine.h"

Engine::Engine(){
	keyboard = Keyboard::getInstance();
	mPlayer.getPlCam()->setCamVerAngle(70.0f);
}

void Engine::run(float time){
	resetMKP();
	manageKeys();

	mPlayer.setOldInfo();
	calculatePlayerPos(time);	
	mPlayer.getPlCam()->setCamHorAngle(mPlayer.getPlPos().horAngle);
	mPlayer.getPlCam()->getCamera();	

	findModelsToDelete();
	translateObjects();

	/*
	if (mPlayer.getCordChanged() == true) {
		mPlayer.setCordChanged(false);
		mmm.checkModelsToAdd(models, mPlayer.getPlPos().xx, mPlayer.getPlPos().yy, mPlayer.getPlPos().zz);
	}
	*/
}

void Engine::manageKeys() {
	std::vector<int> keys = keyboard->getKeys();
	for (int key : keys) {
		switch (key) {
		case 87: //'w'
			mkp.w = true;
			break;

		case 65: //'a':
			mkp.a = true;
			break;

		case 83: //'s'
			mkp.s = true;
			break;

		case 68: //'d':
			mkp.d = true;
			break;

		case 81: //'q'
			mkp.q = true;
			break;

		case 69: //'e':
			mkp.e = true;
			break;

		}
	}
}

void Engine::resetMKP(){
	mkp.w = false;
	mkp.a = false;
	mkp.s = false;
	mkp.d = false;
	mkp.q = false;
	mkp.e = false;
}

void Engine::calculatePlayerPos(float time){
	glm::vec3 tmpTranslation;
	if (mkp.w == false && mkp.s == false) {
		if (mkp.a == false && mkp.d == false) {
			tmpTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else if (mkp.a == true && mkp.d == false) {
			tmpTranslation = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		else if (mkp.a == false && mkp.d == true) {
			tmpTranslation = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			tmpTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else if (mkp.w == true && mkp.s == false) {
		if (mkp.a == false && mkp.d == false) {
			tmpTranslation = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else if (mkp.a == true && mkp.d == false) {
			tmpTranslation = glm::vec3(-1.0f, 1.0f, 0.0f);
		}
		else if (mkp.a == false && mkp.d == true) {
			tmpTranslation = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else {
			tmpTranslation = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}
	else if (mkp.w == false && mkp.s == true) {
		if (mkp.a == false && mkp.d == false) {
			tmpTranslation = glm::vec3(0.0f, -1.0f, 0.0f);
		}
		else if (mkp.a == true && mkp.d == false) {
			tmpTranslation = glm::vec3(-1.0f, -1.0f, 0.0f);
		}
		else if (mkp.a == false && mkp.d == true) {
			tmpTranslation = glm::vec3(1.0f, -1.0f, 0.0f);
		}
		else {
			tmpTranslation = glm::vec3(0.0f, -1.0f, 0.0f);
		}
	}
	else {
		if (mkp.a == false && mkp.d == false) {
			tmpTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else if (mkp.a == true && mkp.d == false) {
			tmpTranslation = glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		else if (mkp.a == false && mkp.d == true) {
			tmpTranslation = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else {
			tmpTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
		}
	}

	if (mkp.q == true) {
		if (mkp.e == false) {
			mPlayer.rotatePlayer(0, 90 * time, 0);
		}
	}
	else if (mkp.e == true) {
		mPlayer.rotatePlayer(0, -90 * time, 0);
	}
	tmpTranslation = glm::rotate(tmpTranslation, glm::radians(mPlayer.getPlPos().horAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	tmpTranslation *= time*16;
	
	mPlayer.translatePlayer(tmpTranslation);	
}

void Engine::translateObjects(){
	glm::vec3 trVec = mPlayer.getPlPos().posVec - mPlayer.getOldInfo().plPos.posVec;
	trVec.x += 20.0f * (mPlayer.getPlPos().xx - mPlayer.getOldInfo().plPos.xx);
	trVec.y += 20.0f * (mPlayer.getPlPos().yy - mPlayer.getOldInfo().plPos.yy);
	trVec.z += 20.0f * (mPlayer.getPlPos().zz - mPlayer.getOldInfo().plPos.zz);

	for (ModelN& m : models) {
		m.mPos -= trVec;
	}
}

void Engine::findModelsToDelete(){
	modelsTD.clear();
	for (int32_t i = models.size()-1; i > 0; i--) {
		if (models.at(i).mPos.x < -75.0f || models.at(i).mPos.x > 75.0f || 
			models.at(i).mPos.y < -75.0f || models.at(i).mPos.y > 75.0f || 
			models.at(i).mPos.z < -75.0f || models.at(i).mPos.z > 75.0f) {
			modelsTD.push_back(i);
		}
	}	
}

MyPlayer* Engine::getPlayer(){
	return &mPlayer;
}

bool Engine::getDrawPlayer(){
	return drawPlayer;
}

void Engine::setDrawPlayer(bool drawPlayer){
	this->drawPlayer = drawPlayer;
}

glm::vec4 Engine::getCurrentColor(){
	return currentColor;
}

void Engine::checkModelsToAdd(std::vector<ModelN> models, int32_t xx, int32_t yy, int32_t zz){
	if (mPlayer.getCordChanged() == true) {		
		mmm.checkModelsToAdd(models, xx, yy, zz);
		mPlayer.setCordChanged(false);
	}
}

TextClass* Engine::getTextClass(){
	return &txtClass;
}

std::vector<ModelN>* Engine::getModelsP(){
	return &models;
}

std::vector<ModelN>* Engine::getModelsbP() {
	return &modelsb;
}

std::vector<ModelN>* Engine::getModelsDP() {
	return &modelsD;
}

std::vector<ModelN> Engine::getModels() {
	return models;
}

std::vector<ModelN> Engine::getModelsb() {
	return modelsb;
}

std::vector<ModelN> Engine::getModelsD() {
	return modelsD;
}

MultiModelManager* Engine::getMultiModelManagerP(){
	return &mmm;
}

