#pragma once
#include "ModelManager.h"

#include <fstream>
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

private:
	void addModel(uint32_t id, float x, float y, float z, std::string texturePath, std::string modelPath);
	void addModel(uint32_t id, float x, float y, float z, float xR, float yR, float zR, std::string texturePath, std::string modelPath);
	void addModel(ModelInfo mi);
	
	void findModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz);
	bool checkModelById(std::vector<ModelN> mds, uint32_t id);
	bool checkModelByCords(std::vector<ModelN> mds, uint32_t xx, uint32_t yy, uint32_t zz);

	std::vector<ModelInfo> modelsToAdd;
	//std::mutex mtx;
};

