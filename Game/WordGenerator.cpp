#include "WordGenerator.h"

WordGenerator::WordGenerator(){
	loadModels();
}

void WordGenerator::makeModelInfos(){
	//srand(time(NULL));
	float LO = -9.0f;
	float HI = 9.0f;
	float rLO = 0.0f;
	float rHI = 359.0f;
	ModelInfo mi;
	mi.z = 0.0f;
	float random = 0.0f;
	
	for (GeneratingModel gm : modelsList) {		
		mi.modelPath = gm.modelPath;
		mi.texturePath = gm.texturePath;
		mi.scale = ((float)rand()) / (float)RAND_MAX;
		random = gm.rarity * (((float)rand()) / (float)RAND_MAX);	

		for (int16_t i = 0; i < random; i++) {
			mi.x = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			mi.y = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
			if (gm.xR < 0) {
				mi.xR = rLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (rHI - rLO)));
			}
			else {
				mi.xR = gm.xR;
			}
			if (gm.yR < 0) {
				mi.yR = rLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (rHI - rLO)));
			}
			else {
				mi.yR = gm.xR;
			}
			if (gm.zR < 0) {
				mi.zR = rLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (rHI - rLO)));
			}
			else {
				mi.zR = gm.xR;
			}
			modelInfos.push_back(mi);
		}
	}
}

std::vector<ModelInfo> WordGenerator::getModelInfos(){
	return modelInfos;
}

std::vector<ModelInfo>* WordGenerator::getModelInfosP() {
	return &modelInfos;
}

void WordGenerator::clearModelInfos(){
	modelInfos.clear();
}

void WordGenerator::loadModels(){
	size_t pos = 0;
	int8_t i = 0;
	std::string delimiter = ";";
	std::string mData[10];
	std::string line;
	GeneratingModel gm;

	std::string fileName = "files/generator_models.txt";
	std::ifstream inFile(fileName);

	if (inFile.is_open()) {
		while (std::getline(inFile, line)) {
			if (line.find(delimiter) != std::string::npos) {

				i = 0;
				pos = 0;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					mData[i] = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());
					i++;
				}
					gm.modelPath = mData[0];
					gm.texturePath = mData[1];
					gm.scaleMin = std::stof(mData[2]);
					gm.scaleMax = std::stof(mData[3]);
					gm.xR = std::stof(mData[4]);
					gm.yR = std::stof(mData[5]);
					gm.zR = std::stof(mData[6]);
					gm.rarity = std::stof(mData[7]);

					modelsList.push_back(gm);				
			}
		}
		inFile.close();
	}
}
