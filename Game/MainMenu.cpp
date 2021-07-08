#include "MainMenu.h"

MainMenu::MainMenu(){
	Option o;
	int8_t id = 0;

	o.id = id++;	o.name = "START";
	options.push_back(o);

	o.id = id++;	o.name = "ABOUT";
	options.push_back(o);

	o.id = id++;	o.name = "EXIT";
	options.push_back(o);

	chosenOption = 0;
}

std::vector<Option> MainMenu::getOptions(){
	return options;
}

std::vector<Option>* MainMenu::getOptionsP() {
	return &options;
}

void MainMenu::optionUp(){
	chosenOption++;
	if (chosenOption == options.size()) {
		chosenOption--;
	}
}

void MainMenu::optionDown(){
	chosenOption--;
	if (chosenOption < 0) {
		chosenOption = 0;
	}
}

void MainMenu::setOption(int nr){
	chosenOption = nr;
}

Option MainMenu::getChosenOption(){
	return options.at(chosenOption);
}

int MainMenu::getChosenOptionId(){
	return chosenOption;
}

std::string MainMenu::getChosenOptionName(){
	return options.at(chosenOption).name;
}
