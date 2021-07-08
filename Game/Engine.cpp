#include "Engine.h"

Engine::Engine(){
	keyboard = Keyboard::getInstance();
	mPlayer.getPlCam()->setCamVerAngle(70.0f);
	if (mainMenu == true) {
		initMainMenu();
	}
}

void Engine::run(float time){
	resetMKP();
	resetARP();
	manageKeys();
	if (mainMenu == true) {
		runMainMenu(time);
	}
	else {
		runGame(time);
	}

	if (mmm.getModelsWritting() == false) {
		mmm.setModelsWritting(true);
		std::thread wm(&MultiModelManager::writeModels, &mmm);
		wm.detach();
	}
}

bool Engine::getToRecreateBuffers(){
	return toRecreateBuffers;
}

void Engine::setToRecreateBuffers(bool trb){
	toRecreateBuffers = trb;
}

bool Engine::getShouldExit(){
	return shouldExit;
}

void Engine::setModelsAdding(bool set){
	modelsAdding = set;
}

bool Engine::getModelsAdding(){
	return modelsAdding;
}

void Engine::runMainMenu(float time){
	mPlayer.getPlCam()->getCamera();
	mPlayer.getPlCam()->rotateHor(time*20);
	translateObjects();
	
	if (ignoreKeys == true) {
		if (anyKeyPressed == false)
			ignoreKeys = false;
	}
	else {
		if (anyKeyPressed == true) {
			if (arp.enter == true) {
				Option op = mMenu.getChosenOption();
				if (op.name == "START") {
					initGame();
				}
				else if (op.name == "EXIT") {
					shouldExit = true;
				}
				
			}
			else {
				Option op = mMenu.getChosenOption();
				std::cout << mMenu.getChosenOptionId() << std::endl;
				txtClass.deleteWord(op.name);
				txtClass.addWord(op.name, op.fontSize, op.posX, op.posY, op.align, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				if (arp.up == true) {
					mMenu.optionDown();					
				}
				else if (arp.down == true) {
					mMenu.optionUp();
				}
				ignoreKeys = true;
				toRecreateBuffers = true;
				op = mMenu.getChosenOption();
				txtClass.deleteWord(op.name);
				txtClass.addWord(op.name, op.fontSize, op.posX, op.posY, op.align, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			}
		}		
	}
}

void Engine::initMainMenu(){
	checkModelsToAdd(0, 0, 0);
	drawPlayer = false;
	float ifontSize = 300.0f;
	float iposX = -2.0f; 
	float menuPos = mMenu.getOptions().size() / 2;
	char ialign = 'L';
	Option* o;
	for (int i = 0; i < mMenu.getOptions().size(); i++) {
		o = &mMenu.getOptionsP()->at(i);
		o->fontSize = ifontSize;
		o->posX = iposX;
		o->posY = menuPos;
		o->align = ialign;
		if (o->id == mMenu.getChosenOptionId()) {
			o->word = txtClass.addWord(o->name, ifontSize, iposX, menuPos, ialign, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else {
			o->word = txtClass.addWord(o->name, ifontSize, iposX, menuPos, ialign, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		menuPos -= 2.0f;
	}
	resetMKP();
	resetARP();
}

void Engine::initGame() {
	txtClass.clearWords();
	resetMKP();
	resetARP();
	mainMenu = false;
	ignoreKeys = false;
	drawPlayer = true;
	toRecreateBuffers = true;
}

void Engine::runGame(float time){
	mPlayer.setOldInfo();
	calculatePlayerPos(time);
	mPlayer.getPlCam()->setCamHorAngle(mPlayer.getPlPos().horAngle);
	mPlayer.getPlCam()->getCamera();

	findModelsToDelete();
	translateObjects();	

	if (mPlayer.getCordChanged() == true) {
		txtClass.deleteWordContain("Cords:");
		std::string cord = "Cords: " + std::to_string(mPlayer.getPlPos().xx) + ":" + std::to_string(mPlayer.getPlPos().yy);
		txtClass.addWord(cord, 150, -1.3f, -8.0f, 'L', glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		toRecreateBuffers = true;
	}
	if (modelsAdding == false && checkMTA == false && mPlayer.getCordChanged() == true) {
		mPlayer.setCordChanged(false);
		checkMTA = true;
		checkModelsToAdd(mPlayer.getPlPos().xx, mPlayer.getPlPos().yy, mPlayer.getPlPos().zz);
		//std::thread trMTA(&Engine::checkModelsToAdd, this, mPlayer.getPlPos().xx, mPlayer.getPlPos().yy, mPlayer.getPlPos().zz);
		//trMTA.detach();
	}
}

void Engine::manageKeys() {
	std::vector<int> keys = keyboard->getKeys();
	if (keys.size() > 0) {
		anyKeyPressed = true;
		for (int key : keys) {
			if (mainMenu == true) {
				resetARP();
			}
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

			case 265: //'up':
				arp.up = true;
				mkp.w = true;
				break;

			case 264: //'down':
				arp.down = true;
				mkp.s = true;
				break;

			case 263: //'left':
				arp.left = true;
				mkp.a = true;
				break;

			case 262: //'right':
				arp.right = true;
				mkp.d = true;
				break;

			case 257: //'enter':
				arp.enter = true;
				break;

			}
		}
	}
	else {
		anyKeyPressed = false;
	}
	if (mainMenu == true) {
		keyboard->clearKeys();
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

void Engine::resetARP(){
	arp.up = false;
	arp.down = false;
	arp.left = false;
	arp.right = false;
	arp.enter = false;
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
	int size = 3;
	for (int32_t i = models.size() - 1; i > 0; i--) {
		if (modulus(models.at(i).xx - mPlayer.getPlPos().xx) > size || 
			modulus(models.at(i).yy - mPlayer.getPlPos().yy) > size || 
			modulus(models.at(i).zz - mPlayer.getPlPos().zz) > size		) {
			modelsTD.push_back(i);
		}
	}
}

int Engine::modulus(int number) {
	return number < 0 ? -number : number;
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

void Engine::setCurrentColor(glm::vec4 color){
	currentColor = color;
}

std::vector<Texture> Engine::getTextures(){
	return textures;
}

std::vector<Texture>* Engine::getTexturesP(){
	return &textures;
}

std::vector<TrianglesData> Engine::getTrianglesDatas(){
	return trianglesDatas;
}

std::vector<TrianglesData>* Engine::getTrianglesDatasP() {
	return &trianglesDatas;
}

void Engine::checkModelsToAdd(int32_t xx, int32_t yy, int32_t zz){
	//if (mPlayer.getCordChanged() == true) {
	//	mPlayer.setCordChanged(false);
		mmm.checkModelsToAdd(models, xx, yy, zz);
	//}	
	checkMTA = false;
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

