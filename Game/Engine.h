#pragma once

#include "MyPlayer.h"
#include "Keyboard.h"
#include "MultiModelManager.h"
#include "TextClass.h"
#include "MainMenu.h"

#include <mutex>
#include <shared_mutex>
#include <thread>

class Engine
{
public:
	Engine();
	void run(float time);
	bool getToRecreateBuffers();
	void setToRecreateBuffers(bool trb);
	bool getShouldExit();
	void setModelsAdding(bool set);
	bool getModelsAdding();

	//player:
	MyPlayer* getPlayer();
	bool getDrawPlayer();
	void setDrawPlayer(bool drawPlayer);
	
	//color:
	glm::vec4 getCurrentColor();
	void setCurrentColor(glm::vec4 color);

	//Textures:
	std::vector<Texture> getTextures();
	std::vector<Texture>* getTexturesP();

	//TrianglesData:
	std::vector<TrianglesData> getTrianglesDatas();
	std::vector<TrianglesData>* getTrianglesDatasP();

	//models:
	std::vector<int32_t> modelsTD;
	std::vector <ModelN>* getModelsP();
	std::vector <ModelN>* getModelsbP();
	std::vector <ModelN>* getModelsDP();
	std::vector <ModelN> getModels();
	std::vector <ModelN> getModelsb();
	std::vector <ModelN> getModelsD();
	MultiModelManager* getMultiModelManagerP();
	void checkModelsToAdd(int32_t xx, int32_t yy, int32_t zz);

	//textClass:
	TextClass* getTextClass();
	

private:
	void runMainMenu(float time);
	void initMainMenu();
	void initGame();
	void runGame(float time);

	void manageKeys();
	void resetMKP();
	void resetARP();
	void calculatePlayerPos(float time);

	void translateObjects();
	void findModelsToDelete();
	//void writeModels();

	inline int modulus(int number);

	MyPlayer mPlayer;
	moveKeysPressed mkp;
	arrowsPressed arp;
	bool drawPlayer = true;
	bool checkMTA = false;
	glm::vec4 currentColor = { 1.0f, 1.0f, 1.0f, 0.0f};
	bool mainMenu = true;
	bool shouldExit = false;
	bool anyKeyPressed = false;
	bool ignoreKeys = false;
	bool toRecreateBuffers = false;
	bool modelsAdding = false;
	bool help = true;
	int helpInt = 0;

	Keyboard* keyboard;

	std::vector<Texture> textures;
	std::vector<TrianglesData> trianglesDatas;
	std::vector <ModelN> models;
	std::vector <ModelN> modelsb;
	std::vector <ModelN> modelsD;
	MultiModelManager mmm;
	TextClass txtClass;
	MainMenu mMenu;

	std::mutex mtx;

};