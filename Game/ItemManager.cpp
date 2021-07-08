#include "ItemManager.h"

ItemManager::ItemManager(){

}

std::vector<Item> ItemManager::getItemTypes(){
	return itemTypes;
}

Item ItemManager::getItemById(uint32_t id){
	for (Item item : itemTypes) {
		if (item.id == id) {
			return item;
		}
	}
	return Item();
}

Item ItemManager::getItemByName(std::string name){
	for (Item item : itemTypes) {
		if (item.name == name) {
			return item;
		}
	}
	return Item();
}
