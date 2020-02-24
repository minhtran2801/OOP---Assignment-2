
#ifndef BUNDLEGENERATOR_H_
#define BUNDLEGENERATOR_H_
#include<fstream>

#include "Order.h"

class BundleGenerator : public Order {
private:
	double profitRate;
public:
	BundleGenerator(int size);
};

BundleGenerator::BundleGenerator(int size) {

	ofstream fout;
	fout.open("orderbundle.txt", std::ios::app);
	if (fout.fail()) {
		cout << "Cannot open output file (orderbundle)" << endl;
	}

	srand(time(0));
	orderId = size;
	PCid = rand() % 10;
	quantity = rand() % 20 + 1;
	dueDate = rand() % 4;
	profitRate = rand() % 60 + 15;
	profitRate = profitRate / 100;
	Product prod(PCid);
	profit = prod.getTotalCost() * profitRate;
	fout << '[' << orderId << ',' << PCid << ',' << quantity << ',' << dueDate << ',' << profit << ']';
	fout.close();
}

#endif /* BUNDLEGENERATOR_H_ */
