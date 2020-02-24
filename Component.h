#ifndef COMPONENT_H_
#define COMPONENT_H_

#include<iostream>
#include<string>
using namespace std;
#include "Item.h"

class Component: public Item {
private:
	void setCost();

public:
	Component() :
			Item() {
	}
	Component(int cid) :
			Item(cid) {
		setCost();
	}
	string getName() const;

};

void Component::setCost() {
	int costMap[] = { 1000, 1500, 1000, 1500, 250, 250, 100, 200, 300, 400 };
	if (id < 0 || id > 9) {
		cout << "Component Id is out of range!" << endl;
	} else {
		cost = costMap[id];
	}
}

string Component::getName() const {
	string nameMap[] = { "Pintel CPU 3 GHz", "Pintel CPU 5 GHz",
			"IMD CPU 3 GHz", "IMD CPU 5 GHz", "Pintel motherboard",
			"IMD motherboard", "Memory 4 GB", "Memory 8 GB", "Hard disk 1 TB",
			"Hard disk 2 TB" };

	if (id < 0 || id > 9) {
		cout << "Component Id has not been initialized!" << endl;
		return "";
	} else {
		return nameMap[id];
	}
}

#endif /* COMPONENT_H_ */
