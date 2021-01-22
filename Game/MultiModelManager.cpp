#include "MultiModelManager.h"

MultiModelManager::MultiModelManager() {
	//addModel(2, 0, 2, 0, "textures/viking_room.png", "models/viking_room.obj");
}

void MultiModelManager::checkModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz){
	//if (modelsToAdd.size() == 0) {		
		for (int32_t x = xx - 3; x <= xx + 3; x++) {
			for (int32_t y = yy - 3; y <= yy + 3; y++) {
				//for (int32_t z = zz - 3; z <= zz + 3; z++) {
					findModelsToAdd(mds, x, y, 0);
					//std::vector<ModelN> mdst;
					//std::thread(&MultiModelManager::findModelsToAdd, this, mdst, x, y, 0).detach();
				//}
			}
		}
	//}
}

uint32_t MultiModelManager::getModelsToAddSize() {
	return modelsToAdd.size();
}

ModelInfo MultiModelManager::getFirstModelInfo()
{
	ModelInfo mi = modelsToAdd.back();
	modelsToAdd.pop_back();
	return mi;
}

bool MultiModelManager::isModelsToAddEmpty()
{
	if (modelsToAdd.empty()) {
		return true;
	}
	else {
		return false;
	}
}

void MultiModelManager::clearModelsToAdd(){
	modelsToAdd.clear();
}

void MultiModelManager::addModel(uint32_t id, float x, float y, float z, std::string texturePath, std::string modelPath) {
	ModelInfo mi;

	mi.id = id;
	mi.x = x;
	mi.y = y;
	mi.z = z;
	mi.texturePath = texturePath;
	mi.modelPath = modelPath;

	modelsToAdd.push_back(mi);
}

void MultiModelManager::addModel(uint32_t id, float x, float y, float z, float xR, float yR, float zR, std::string texturePath, std::string modelPath) {
	ModelInfo mi;

	mi.id = id;
	mi.x = x;
	mi.y = y;
	mi.z = z;
	mi.xR = xR;
	mi.yR = yR;
	mi.zR = zR;
	mi.texturePath = texturePath;
	mi.modelPath = modelPath;

	modelsToAdd.push_back(mi);
}

void MultiModelManager::addModel(ModelInfo mi) {
	modelsToAdd.push_back(mi);
}

void MultiModelManager::findModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz){
	size_t pos = 0;
	int8_t i = 0;
	ModelInfo mi;
	std::string delimiter = ";";
	std::string mData[9];
	std::string line;	
	std::string fileName = "files/model_load_info_txt/" + std::to_string(xx) + "_" + std::to_string(yy) + "_" + std::to_string(zz) + ".txt";
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
				if (checkModelById(mds, std::stoi(mData[0])) == false) {
					mi.xx = xx;
					mi.yy = yy;
					mi.zz = zz;
					mi.id = std::stoi(mData[0]);
					mi.modelPath = mData[1];
					mi.texturePath = mData[2];
					mi.x = std::stof(mData[3]);					
					mi.y = std::stof(mData[4]);
					mi.z = std::stof(mData[5]);
					mi.xR = std::stof(mData[6]);
					mi.yR = std::stof(mData[7]);
					mi.zR = std::stof(mData[8]);
					modelsToAdd.push_back(mi);
				}
			}
		}
		inFile.close();
	}
	else {
		if (checkModelByCords(mds, xx, yy, zz) == false) {
			mi.id = 1000;
			mi.xx = xx;
			mi.yy = yy;
			mi.zz = zz;
			mi.modelPath = "models/grass.obj";
			mi.texturePath = "textures/grass01.png";
			mi.x = 0.0f;
			mi.y = 0.0f;
			mi.z = 0.0f;
			mi.xR = 90.0f;
			mi.yR = 0.0f;
			mi.zR = 0.0f;
			modelsToAdd.push_back(mi);
		}		
	}
}

bool MultiModelManager::checkModelById(std::vector<ModelN> mds, uint32_t id){
	bool returned = false;
	for (ModelN m : mds) {		
		if (m.id == id) {
			returned = true;
			break;
		}
	}
	for (ModelInfo mi : modelsToAdd) {
		if (mi.id == id) {
			returned = true;
			break;
		}		
	}
	return returned;
}

bool MultiModelManager::checkModelByCords(std::vector<ModelN> mds, uint32_t xx, uint32_t yy, uint32_t zz){
	bool returned = false;
	for (ModelN m : mds) {		
		if (m.xx == xx && m.yy == yy && m.zz == zz) {
			returned = true;
			break;
		}
	}
	for (ModelInfo mi : modelsToAdd) {
		if (mi.xx == xx && mi.yy == yy && mi.zz == zz) {
			returned = true;
			break;
		}		
	}
	return returned;
}
