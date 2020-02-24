#ifndef PRODUCT_H_
#define PRODUCT_H_

#include<iostream>
#include<vector>
using namespace std;

#include "Item.h"
class Product: public Item {
private:
	int cycles;
	void setCycles();
	void setCost();
	static vector<int> componentVector;
public:
	Product() :
			Item() {
		setCycles();
	}
	Product(int cid) :
			Item(cid) {
		setCycles();
		setCost();
	}
	Product(int cid, bool comp) : Item(cid) {
		for (int x = 0; x < COMPONENT_PER_ORDER; x++) {
			int currentComponent = getComp(x);
			countComponent(currentComponent);
		}
	}

	int getComp(int index) const;
	int getTotalCost() {return cost;};
	int getCycles() {
		return cycles;
	}
	;
	void countComponent(int currentComponent);
	static int getComponentAmount(int id) {
		return componentVector[id];
	}
	;
};

vector<int> Product::componentVector = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void Product::setCost() {
	int totalCost = 0;
	int currentComponent;
	for (int x = 0; x < 4; x++) {
		currentComponent = getComp(x);
		Component c(currentComponent);
		totalCost = totalCost + c.getCost();
	}
	cost = totalCost + cycles * 100;
}

int Product::getComp(int index) const {
	if (id < 0 || id > 15) {
		cout << "Product Id has not been initialized!" << endl;
		return -1;
	} else {
		return compMap[id][index];
	}
}

void Product::setCycles() {
	if (id < 0 || id > 15) {
		cycles = -1;
	} else {
		cycles = cycleMap[id];
	}
}

void Product::countComponent(int currentComponent) {
	for (int x = 0; x < 10; x++) {
		if (x == currentComponent) {
			componentVector[x] = componentVector[x]  + 1;
			break;
		}
	}
}

#endif /* PRODUCT_H_ */
