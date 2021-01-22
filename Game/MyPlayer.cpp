#include "MyPlayer.h"

MyPlayer::MyPlayer() {
	this->plInfo.plCam = new Camera();
	mi.id = 0;
	mi.modelPath = "models/qube.obj";
	mi.texturePath = "textures/texture.jpg";
}

MyPlayer::MyPlayer(float x, float y, float z, int32_t xx, int32_t yy, int32_t zz, float verAngle, float horAngle, float tiltAngle, float speed, ModelInfo mi){
	plInfo.plPos.posVec = glm::vec3(x, y, z);
	plInfo.plPos.xx = xx;
	plInfo.plPos.yy = yy;
	plInfo.plPos.zz = zz;
	plInfo.plPos.verAngle = verAngle;
	plInfo.plPos.horAngle = horAngle;
	plInfo.plPos.tiltAngle = tiltAngle;
	plInfo.speed = speed;
}

void MyPlayer::setPos(float x, float y, float z, int32_t xx, int32_t yy, int32_t zz, float verAngle, float horAngle, float tiltAngle){
	plInfo.plPos.posVec = glm::vec3(x, y, z);
	plInfo.plPos.xx = xx;
	plInfo.plPos.yy = yy;
	plInfo.plPos.zz = zz;
	plInfo.plPos.verAngle = verAngle;
	plInfo.plPos.horAngle = horAngle;
	plInfo.plPos.tiltAngle = tiltAngle;
}

void MyPlayer::setRotation(float verAngle, float horAngle, float tiltAngle){
	plInfo.plPos.verAngle = verAngle;
	plInfo.plPos.horAngle = horAngle;
	plInfo.plPos.tiltAngle = tiltAngle;
}

void MyPlayer::setPos(float x, float y, float z){
	plInfo.plPos.posVec = glm::vec3(x, y, z);
}

void MyPlayer::setPos(int32_t xx, int32_t yy, int32_t zz){
	plInfo.plPos.xx = xx;
	plInfo.plPos.yy = yy;
	plInfo.plPos.zz = zz;
}

void MyPlayer::setSpeed(float speed){
	plInfo.speed = speed;
}

void MyPlayer::setModelN(ModelN& m){
	modeln = m;
}

void MyPlayer::setCordChanged(bool change){
	cordChanged = change;
}

void MyPlayer::setOldInfo(){
	oldInfo = plInfo;
}

void MyPlayer::resetPlInfo(){
	resetPlCam();
	resetPlInfo();
}

void MyPlayer::resetPos(){
	plInfo.plPos.posVec = glm::vec3(0.0f, 0.0f, 0.0f);
	plInfo.plPos.xx = 0;
	plInfo.plPos.yy = 0;
	plInfo.plPos.zz = 0;
}

void MyPlayer::resetPlCam(){
	plInfo.plCam->resetCamera();
}

void MyPlayer::resetSpeed(){
	plInfo.speed = 0.0f;
}

float MyPlayer::getPlSpeed(){
	return plInfo.speed;
}

playerInfo MyPlayer::getPlInfo(){
	return plInfo;
}

playerPos MyPlayer::getPlPos(){
	return plInfo.plPos;
}

glm::vec3 MyPlayer::getPlPosVec()
{
	return plInfo.plPos.posVec;
}

Camera* MyPlayer::getPlCam(){
	return plInfo.plCam;
}

ModelManager* MyPlayer::getObjectManager(){
	return &mm;
}

ModelInfo* MyPlayer::getModelInfo(){
	return &mi;
}

ModelN* MyPlayer::getModelN(){
	return &modeln;
}

bool MyPlayer::getCordChanged(){
	return cordChanged;
}

playerInfo MyPlayer::getOldInfo(){
	return oldInfo;
}

void MyPlayer::rotatePlayer(float ver, float hor, float tilt){
	plInfo.plPos.verAngle += ver;
	plInfo.plPos.horAngle += hor;
	plInfo.plPos.tiltAngle += tilt;

	while (plInfo.plPos.verAngle < 0) {
		plInfo.plPos.verAngle += 360;
	}
	while (plInfo.plPos.verAngle >= 0) {
		plInfo.plPos.verAngle -= 360;
	}

	while (plInfo.plPos.horAngle < 0) {
		plInfo.plPos.horAngle += 360;
	}
	while (plInfo.plPos.horAngle >= 0) {
		plInfo.plPos.horAngle -= 360;
	}

	while (plInfo.plPos.tiltAngle < 0) {
		plInfo.plPos.tiltAngle += 360;
	}
	while (plInfo.plPos.tiltAngle >= 0) {
		plInfo.plPos.tiltAngle -= 360;
	}
}

void MyPlayer::translatePlayer(glm::vec3 trVec){
	float tmpSize = 20.0f;
	plInfo.plPos.posVec += trVec;

	while (plInfo.plPos.posVec.x > tmpSize) {
		plInfo.plPos.posVec.x -= tmpSize;
		plInfo.plPos.xx++;
		cordChanged = true;
	}
	while (plInfo.plPos.posVec.x < -tmpSize) {
		plInfo.plPos.posVec.x += tmpSize;
		plInfo.plPos.xx--;
		cordChanged = true;
	}

	while (plInfo.plPos.posVec.y > tmpSize) {
		plInfo.plPos.posVec.y -= tmpSize;
		plInfo.plPos.yy++;
		cordChanged = true;
	}
	while (plInfo.plPos.posVec.y < -tmpSize) {
		plInfo.plPos.posVec.y += tmpSize;
		plInfo.plPos.yy--;
		cordChanged = true;
	}

	while (plInfo.plPos.posVec.z > tmpSize) {
		plInfo.plPos.posVec.z -= tmpSize;
		plInfo.plPos.zz++;
		cordChanged = true;
	}
	while (plInfo.plPos.posVec.z < -tmpSize) {
		plInfo.plPos.posVec.z += tmpSize;
		plInfo.plPos.zz--;
		cordChanged = true;
	}
}
