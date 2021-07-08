#pragma once

#include "ModelManager.h"
#include "WordGenerator.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <shared_mutex>

class MultiModelManager : public ModelManager
{
public:
	MultiModelManager();

	void checkModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz);
	uint32_t getModelsToAddSize();
	ModelInfo getFirstModelInfo();
	bool isModelsToAddEmpty();
	void clearModelsToAdd();
	void clearModelsToAddSize(int j);
	void writeModels();
	bool getModelsWritting();
	void setModelsWritting(bool set);

private:
	void addModel(uint32_t id, float x, float y, float z, std::string texturePath, std::string modelPath);
	void addModel(uint32_t id, float x, float y, float z, float xR, float yR, float zR, std::string texturePath, std::string modelPath, float scale);
	void addModel(ModelInfo mi);
	
	void findModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz);
	bool checkModelById(std::vector<ModelN> mds, uint32_t id, uint32_t xx, uint32_t yy, uint32_t zz);
	bool checkModelByCords(std::vector<ModelN> mds, uint32_t xx, uint32_t yy, uint32_t zz);

	void writeModel(ModelInfo m);	
	bool modelsWritting = false;

	std::vector<ModelInfo> modelsToAdd;
	std::vector<ModelInfo> modelsToWrite;
	WordGenerator wg;
};

