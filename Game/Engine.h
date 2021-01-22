#pragma once

#include "MyPlayer.h"
#include "Keyboard.h"
#include "MultiModelManager.h"
#include "TextClass.h"

#include <mutex>
#include <shared_mutex>
#include <thread>

class Engine
{
public:
	Engine();
	void run(float time);

	MyPlayer* getPlayer();
	bool getDrawPlayer();
	void setDrawPlayer(bool drawPlayer);
	
	glm::vec4 getCurrentColor();	

	std::vector<int32_t> modelsTD;
	std::vector <ModelN>* getModelsP();
	std::vector <ModelN>* getModelsbP();
	std::vector <ModelN>* getModelsDP();
	std::vector <ModelN> getModels();
	std::vector <ModelN> getModelsb();
	std::vector <ModelN> getModelsD();
	MultiModelManager* getMultiModelManagerP();
	void checkModelsToAdd(std::vector<ModelN> models, int32_t xx, int32_t yy, int32_t zz);

	TextClass* getTextClass();
	

private:
	void manageKeys();
	void resetMKP();
	void calculatePlayerPos(float time);

	void translateObjects();
	void findModelsToDelete();

	MyPlayer mPlayer;
	moveKeysPressed mkp;
	bool drawPlayer = true;
	bool checkMTA = true;
	glm::vec4 currentColor = { 1.0f, 1.0f, 1.0f, 0.0f};

	Keyboard* keyboard;	

	std::vector <ModelN> models;
	std::vector <ModelN> modelsb;
	std::vector <ModelN> modelsD;
	MultiModelManager mmm;
	TextClass txtClass;

};