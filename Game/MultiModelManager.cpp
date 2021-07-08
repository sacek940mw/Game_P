#include "MultiModelManager.h"

MultiModelManager::MultiModelManager() {
	//addModel(2, 0, 2, 0, "textures/viking_room.png", "models/viking_room.obj");
}

void MultiModelManager::checkModelsToAdd(std::vector<ModelN> mds, int32_t xx, int32_t yy, int32_t zz){
	//if (modelsToAdd.size() == 0) {
	int32_t size = 2;
		for (int32_t x = xx - size; x <= xx + size; x++) {
			for (int32_t y = yy - size; y <= yy + size; y++) {
				//for (int32_t z = zz - size; z <= zz + size; z++) {
					findModelsToAdd(mds, x, y, 0);
				//}
			}
		}
	//}
}

uint32_t MultiModelManager::getModelsToAddSize() {
	return modelsToAdd.size();
}

ModelInfo MultiModelManager::getFirstModelInfo(){
	ModelInfo mi = modelsToAdd.back();
	modelsToAdd.pop_back();
	return mi;
}

bool MultiModelManager::isModelsToAddEmpty(){
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

void MultiModelManager::clearModelsToAddSize(int size){
	modelsToAdd.erase(modelsToAdd.begin(), modelsToAdd.begin() + size);
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

void MultiModelManager::addModel(uint32_t id, float x, float y, float z, float xR, float yR, float zR, std::string texturePath, std::string modelPath, float scale) {
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
	mi.scale = scale;

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
	std::string mData[10];
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
				if (checkModelById(mds, std::stoi(mData[0]), xx, yy, zz) == false) {
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
					mi.scale = std::stof(mData[9]);
					modelsToAdd.push_back(mi);
				}
			}
		}
		inFile.close();
	}
	else {
		std::string fileName = "files/saves/" + std::to_string(xx) + "_" + std::to_string(yy) + "_" + std::to_string(zz) + ".txt";
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
					if (checkModelById(mds, std::stoi(mData[0]), xx, yy, zz) == false) {
						
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
						mi.scale = std::stof(mData[9]);
						modelsToAdd.push_back(mi);
					}
					
				}
			}
			inFile.close();			
		}
		else {
			if (checkModelByCords(mds, xx, yy, zz) == false) {
				int id = 1001;
				mi.id = id++;
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
				mi.scale = 1.0f;
				modelsToAdd.push_back(mi);
				modelsToWrite.push_back(mi);
				//std::cout << "modelsToAdd.push_back(mi);";
				
				wg.clearModelInfos();
				wg.makeModelInfos();
				
				for (int32_t i = 0; i < wg.getModelInfos().size(); i++) {
					ModelInfo* mi = &wg.getModelInfosP()->at(i);
					mi->id = id++;
					mi->xx = xx;
					mi->yy = yy;
					mi->zz = zz;
					modelsToAdd.push_back(*mi);
					modelsToWrite.push_back(*mi);
					//std::cout << "modelsToAdd.push_back(mi);";
				}
				
			}
		}
	}
}

bool MultiModelManager::checkModelById(std::vector<ModelN> mds, uint32_t id, uint32_t xx, uint32_t yy, uint32_t zz){
	bool returned = false;
	for (ModelN m : mds) {		
		if (m.id == id && m.xx == xx && m.yy == yy && m.zz == zz) {
			returned = true;
			break;
		}
	}
	/*
	for (ModelInfo mi : modelsToAdd) {
		if (mi.id == id && mi.xx == xx && mi.yy == yy && mi.zz == zz) {
			returned = true;
			break;
		}		
	}
	*/
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
	/*
	for (ModelInfo mi : modelsToAdd) {
		if (mi.xx == xx && mi.yy == yy && mi.zz == zz) {
			returned = true;
			break;
		}		
	}
	*/
	return returned;
}

void MultiModelManager::writeModel(ModelInfo m){
	
	std::string fileName = "files/saves/" + std::to_string(m.xx) + "_" + std::to_string(m.yy) + "_" + std::to_string(m.zz) + ".txt";
	std::ofstream MyFile;
	MyFile.open(fileName, std::ios_base::app);
	//std::string text = "tekst";
	std::string text = std::to_string(m.id) + ";" + m.modelPath + ";" + m.texturePath + ";"
		+ std::to_string(m.x) + ";" + std::to_string(m.y) + ";" + std::to_string(m.z) + ";"
		+ std::to_string(m.xR) + ";" + std::to_string(m.yR) + ";" + std::to_string(m.zR) + ";"
		+ std::to_string(m.scale) + ";";
	MyFile << text << std::endl;
	MyFile.close();
	
}

void MultiModelManager::writeModels(){
	int32_t size = 0;
	for (ModelInfo mi : modelsToWrite) {
		writeModel(mi);
		size++;
	}
	modelsToWrite.erase(modelsToWrite.begin(), modelsToWrite.begin() + size);
	modelsWritting = false;
}

bool MultiModelManager::getModelsWritting(){
	return modelsWritting;
}

void MultiModelManager::setModelsWritting(bool set){
	modelsWritting = set;
}
