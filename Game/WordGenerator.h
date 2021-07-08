#pragma once

#include "ModelManager.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <time.h> 

struct GeneratingModel {
	std::string modelPath = "";
	std::string texturePath = "";
	float scaleMin = 1.0f;
	float scaleMax = 1.0f;
	float xR = -1.0f, yR = -1.0f, zR = -1.0f;
	float rarity = 0.0f;
};

class WordGenerator
{
public:
	WordGenerator();

	void makeModelInfos();
	std::vector<ModelInfo> getModelInfos();
	std::vector<ModelInfo>* getModelInfosP();
	void clearModelInfos();

private:
	void loadModels();

	std::vector<GeneratingModel> modelsList;
	std::vector<ModelInfo> modelInfos;

};

