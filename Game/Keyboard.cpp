#include "Keyboard.h"

Keyboard* Keyboard::instance = NULL;

Keyboard* Keyboard::getInstance()
{
	if (instance == NULL) {
		instance = new Keyboard();
	}
	return(instance);
}

void Keyboard::addKey(int key){
	int8_t i = 0;
	bool tmp = false;
	for (auto& k : keys) {
		if (k == key) {
			tmp = true;
			break;
		}
		i++;
	}
	if(tmp == false)
		keys.push_back(key);
}

void Keyboard::removeKey(int key){
	int8_t i = 0;
	for (auto& k : keys) {
		if (k == key) {
			keys.erase(keys.begin()+i);
		}
		i++;
	}
}

void Keyboard::clearKeys(){
	keys.clear(); 
}

std::vector<int> Keyboard::getKeys(){
	return keys;
}
