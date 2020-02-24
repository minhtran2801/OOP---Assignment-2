#ifndef ITEM_H_
#define ITEM_H_

const int cycleMap[] = { 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7 };

const int compMap[16][4] = { { 0, 4, 6, 8 }, { 0, 4, 6, 9 }, { 0, 4, 7, 8 }, {
		0, 4, 7, 9 }, { 1, 4, 6, 8 }, { 1, 4, 6, 9 }, { 1, 4, 7, 8 }, { 1, 4, 7,
		9 }, { 2, 5, 6, 8 }, { 2, 5, 6, 9 }, { 2, 5, 7, 8 }, { 2, 5, 7, 9 }, {
		3, 5, 6, 8 }, { 3, 5, 6, 9 }, { 3, 5, 7, 8 }, { 3, 5, 7, 9 } };


const int CYCLES_LIMIT = 2000;
const int LATE_INDICATOR = -1;
const int DAY_AMOUNT = 5;
const int COMPONENT_PER_ORDER = 4;

class Item {
protected:
	int id;
	int cost;
public:
	Item() {
		id = -1;
		cost = 0;
	}  // the id for an empty item is -1
	Item(int i) {
		id = i;
		cost = 0;
	}
	Item(int i, int c) {
		id = i;
		cost = c;
	}
	int getId() {
		return id;
	}
	int getCost() const {
		return cost;
	}
};

#endif /* ITEM_H_ */
