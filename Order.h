#ifndef ORDER_H_
#define ORDER_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

#include "Item.h"
#include "Component.h"
#include "Product.h"

class Order {
	friend ostream &operator<<(ostream &, const Order &o);
protected:
	int orderId;
	int PCid;
	int quantity;
	int dueDate;
	int profit;
	int totalProfit;
	int doneDate;
	int cycles;
	int totalCost;
public:
	Order();
	Order(int oId, int pcId, int quant, int due, int prof);
	int getCycles();
	int getDueDate() {
		return dueDate;
	}

	int getID() {
		return orderId;
	}
	int getProfit() {
		return totalProfit;
	}

	int getDoneDay() {
		return doneDate;
	}

	int getTotalCycles() {
		return cycles;
	}

	int getQuantity() {
		return quantity;
	}

	int getPCid() {
		return PCid;
	}

	int getTotalCost() {
		return totalCost;
	}

	void changeDoneDate(int currentDate) {
		doneDate = currentDate;
	}
};

Order::Order() {
	orderId = 0;
	PCid = 0;
	quantity = 0;
	dueDate = -1;
	profit = 0;
	totalProfit = 0;
	cycles = 0;
	doneDate = -1;
	totalCost = 0;

}

Order::Order(int oId, int pcId, int quant, int due, int prof) {
	orderId = oId;
	PCid = pcId;
	quantity = quant;
	dueDate = due;
	profit = prof;
	totalProfit = profit * quantity;
	cycles = getCycles() * quantity;
	doneDate = dueDate;

	Product prod(PCid);
	totalCost = prod.getTotalCost() * quantity;

}

int Order::getCycles() {
	Product prod(PCid);
	return prod.getCycles();
}

ostream &operator<<(ostream &output, const Order &o) {
	output << setw(5) << o.orderId << setw(11) << o.PCid << setw(11)
			<< o.quantity << setw(12) << o.dueDate << setw(14) << o.profit
			<< setw(10) << o.totalProfit;

	return output; // enables cascading
} // end function operator<<

#endif /* ORDER_H_ */
