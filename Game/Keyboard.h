#pragma once

#include <vector>
#include <iostream>

class Keyboard
{
public:
	static Keyboard* getInstance();

	void addKey(int key);
	void removeKey(int key);
	void clearKeys();
	std::vector<int> getKeys();

private:
	static Keyboard* instance;
	Keyboard() {};
	Keyboard(const Keyboard&) {};
	Keyboard& operator=(Keyboard const&) {};
	
	std::vector<int> keys;
};