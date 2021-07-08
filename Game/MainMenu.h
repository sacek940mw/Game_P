#pragma once

#include "TextClass.h"

#include <string>
#include <vector>
#include <iostream>

struct Option {
	int id = 0;
	std::string name = "";
	float posX = 0.0f, posY = 0.0f;
	char align = 'L';
	float fontSize = 100.0f;
	Word* word;
};

class MainMenu
{
public:
	MainMenu();
	std::vector<Option> getOptions();
	std::vector<Option>* getOptionsP();
	void optionUp();
	void optionDown();
	void setOption(int nr);
	Option getChosenOption();
	int getChosenOptionId();
	std::string getChosenOptionName();

private:
	std::vector<Option> options;
	int chosenOption;

};

