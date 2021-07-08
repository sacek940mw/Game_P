#pragma once

#include "Renderer_Structs.h"

struct Item {
	uint32_t id = 0;
	std::string name = "item";
	float rarity = 1.0f; //0.0f-1.0f
	float weight = 1.0f;
};

class ItemManager
{
public:
	ItemManager();
	std::vector<Item> getItemTypes();
	Item getItemById(uint32_t id);
	Item getItemByName(std::string name);

private:
	std::vector<Item> itemTypes;

};

