#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include<fstream>
#include<algorithm>
#include<iomanip>
#include<vector>
#include"BundleGenerator.h"
#include"Order.h"

using namespace std;

const double DELAYED_PENALTY = 0.1;
const double CANCEL_PENALTY = 0.5;

inline bool dueDateAsc(Order& day1, Order& day2) {
	return (day1.getDueDate() < day2.getDueDate());
}

inline bool profitDueDes(Order& profit1, Order& profit2) {
	return (profit1.getProfit() > profit2.getProfit()
			&& profit1.getDueDate() == profit2.getDueDate());
}

inline bool profitDes(Order& day1, Order& day2) {
	return (day1.getProfit() > day2.getProfit());
}

class Schedule {
private:
	vector<Order> orderVector;
	vector<Order> satisfiedVector;
	vector<Order> unsatisfiedVector;
	vector<Order> delayedVector;
	vector<int> cyclesPerDay;
	vector<int> componentsUsed;
	vector<int> componentPerDay;

	int day0;
	int day1;
	int day2;
	int day3;
	int day4;

	int totalComponent;
	int totalPenalty;
	int totalCost;
	int totalProfit;
	int totalCycles;

	bool validForChange(int x, int i);
	bool late(int i);
public:
	Schedule();
	void run();
	void chooseDay(int currentCycles, int currentDay);
	int getTotalProfit();
	int vectorSize() {
		return orderVector.size();
	}
	;
	void sortOrders();
	void chooseOrders();
	void countTotalComponents();
	void countComponentPerDay();
	void calculateLateProfit();
	void calculateCancelProfit();
	void calculateTotalProfit();
	void calculateTotalCost();
	void getTotalCycles();
	void generateReport();

};

Schedule::Schedule() {
	char deliminator;
	ifstream fin;
	int orderId;
	int PCid;
	int quantity;
	int dueDate;
	int profit;
	if (fin.fail()) {
		cout << "Cannot open input file" << endl;
		exit(1);
	}
	fin.open("orderbundle.txt");
	while (!fin.eof()) {
		fin.get(deliminator);
		fin >> orderId;
		fin.get(deliminator);
		fin >> PCid;
		fin.get(deliminator);
		fin >> quantity;
		fin.get(deliminator);
		fin >> dueDate;
		fin.get(deliminator);
		fin >> profit;
		fin.get(deliminator);

		Order order(orderId, PCid, quantity, dueDate, profit);
		orderVector.push_back(order);
	}

	orderVector.pop_back();
	fin.close();

	totalComponent = 0;
	totalCost = 0;
	totalPenalty = 0;
	totalProfit = 0;
	totalCycles = 0;

	day0 = 0;
	day1 = 0;
	day2 = 0;
	day3 = 0;
	day4 = 0;

}

void Schedule::run() {
	BundleGenerator ran(orderVector.size());
	sortOrders();
	chooseOrders();
	countComponentPerDay();
	countTotalComponents();
	calculateLateProfit();
	calculateCancelProfit();
	calculateTotalProfit();
	calculateTotalCost();
	getTotalCycles();
	generateReport();
}

void Schedule::countTotalComponents() {
	bool comp;
	for (int x = 0; x < satisfiedVector.size(); x++) {
		int pcID = satisfiedVector[x].getPCid();
		Product prod(pcID, comp);
	}
	for (int x = 0; x < delayedVector.size(); x++) {
		int pcID = delayedVector[x].getPCid();
		Product prod(pcID, comp);
	}
	for (int x = 0; x < 10; x++) {
		totalComponent = totalComponent + Product::getComponentAmount(x);
		componentsUsed.push_back(Product::getComponentAmount(x));
	}
}

void Schedule::sortOrders() {
	int pos0;
	int pos1;
	int pos2;
	int pos3;

	bool foundPos0 = false;
	bool foundPos1 = false;
	bool foundPos2 = false;
	bool foundPos3 = false;

	std::sort(orderVector.begin(), orderVector.end(), dueDateAsc);

	for (int x = 0; x < orderVector.size(); x++) {
		if (orderVector[x].getDueDate() == 1 && !foundPos0) {
			pos0 = x - 1;
			foundPos0 = true;
		} else if (orderVector[x].getDueDate() == 2 && !foundPos1) {
			pos1 = x - 1;
			foundPos1 = true;
		} else if (orderVector[x].getDueDate() == 3 && !foundPos2) {
			pos2 = x - 1;
			foundPos2 = true;
		} else if (orderVector[x].getDueDate() == 4 && !foundPos3) {
			pos3 = x - 1;
			foundPos3 = true;
		}
	}

	std::partial_sort(orderVector.begin(), orderVector.begin() + pos0,
			orderVector.end(), profitDueDes);
	std::partial_sort(orderVector.begin() + pos0 + 1,
			orderVector.begin() + pos1, orderVector.end(), profitDueDes);
	std::partial_sort(orderVector.begin() + pos1 + 1,
			orderVector.begin() + pos2, orderVector.end(), profitDueDes);
	std::partial_sort(orderVector.begin() + pos2 + 1,
			orderVector.begin() + pos3, orderVector.end(), profitDueDes);
	std::partial_sort(orderVector.begin() + pos3 + 1, orderVector.end(),
			orderVector.end(), profitDueDes);
}

void Schedule::chooseOrders() {
	int currentCycles = 0;
	int currentDay = 0;
	bool stop = true;

	for (int i = 0; i < orderVector.size(); i++) {
		int dueDate = orderVector[i].getDueDate();
		currentCycles = currentCycles + orderVector[i].getTotalCycles();

		if (currentDay == 4 && i == orderVector.size() - 1
				&& currentCycles < CYCLES_LIMIT) {
			cyclesPerDay.push_back(
					currentCycles - orderVector[i].getTotalCycles());
		}

		if (currentCycles < CYCLES_LIMIT
				&& (currentDay == dueDate || currentDay < dueDate)) {

			orderVector[i].changeDoneDate(currentDay);
			satisfiedVector.push_back(orderVector[i]);

		} else if (currentCycles >= CYCLES_LIMIT && currentDay == dueDate) {

			if (stop) {
				cyclesPerDay.push_back(
						currentCycles - orderVector[i].getTotalCycles());
				stop = false;
			}

			//orderVector[i].changeDoneDate(LATE_INDICATOR);
			unsatisfiedVector.push_back(orderVector[i]);

		} else if (currentDay < dueDate) {
			stop = true;
			currentDay++;
			currentCycles = orderVector[i].getTotalCycles();
			satisfiedVector.push_back(orderVector[i]);
		}
	}

	std::sort(satisfiedVector.begin(), satisfiedVector.end(), profitDes);
	std::sort(unsatisfiedVector.begin(), unsatisfiedVector.end(),
			profitDes);

	int lastIndex = satisfiedVector.size() - 1;
	for (int i = 0; i < unsatisfiedVector.size(); i++) {

		for (int x = 0; x < satisfiedVector.size(); x++) {
			int day = satisfiedVector[x].getDoneDay();
			int due = satisfiedVector[x].getDueDate();
			int cycles = unsatisfiedVector[i].getCycles();

			if (unsatisfiedVector[i].getProfit()
					> satisfiedVector[x].getProfit() && validForChange(x, i)) {

				unsatisfiedVector[i].changeDoneDate(due);

				if (late(i)) {
					delayedVector.push_back(unsatisfiedVector[i]);
					cyclesPerDay[day] = cyclesPerDay[day] + cycles;
				} else {
					unsatisfiedVector.push_back(satisfiedVector[lastIndex]);
					satisfiedVector.pop_back();
					satisfiedVector.push_back(unsatisfiedVector[i]);
				}

				unsatisfiedVector.erase(unsatisfiedVector.begin() + i);
			}
		}
	}
}

void Schedule::countComponentPerDay() {
	for (int x = 0; x < DAY_AMOUNT; x++) {
		componentPerDay.push_back(0);
	}

	for (int i = 0; i < satisfiedVector.size(); i++) {
		for (int x = 0; x < DAY_AMOUNT; x++) {
			if (satisfiedVector[i].getDoneDay() == x) {
				componentPerDay[x] = componentPerDay[x] + COMPONENT_PER_ORDER;
			}
		}
	}

	for (int i = 0; i < delayedVector.size(); i++) {
		for (int x = 0; x < DAY_AMOUNT; x++) {
			if (satisfiedVector[i].getDoneDay() == x) {
				componentPerDay[x] = componentPerDay[x] + COMPONENT_PER_ORDER;
			}
		}
	}
}

void Schedule::calculateLateProfit() {
	for (int x = 0; x < delayedVector.size(); x++) {
		int dueDate = delayedVector[x].getDueDate();
		int doneDate = delayedVector[x].getDoneDay();
		int dayLate = doneDate - dueDate;
		int pcID = delayedVector[x].getPCid();

		Product prod(pcID);
		int cost = prod.getTotalCost();
		int quantity = delayedVector[x].getQuantity();
		double penalty = dayLate * DELAYED_PENALTY * cost * quantity;
		double tProfit = delayedVector[x].getProfit();
		tProfit = tProfit - penalty;

		totalPenalty = totalPenalty + penalty;
		totalProfit = totalProfit + tProfit;
	}

}

void Schedule::calculateCancelProfit() {
	for (int x = 0; x < unsatisfiedVector.size(); x++) {
		int pcID = unsatisfiedVector[x].getPCid();
		Product prod(pcID);
		int cost = prod.getTotalCost();
		int quantity = unsatisfiedVector[x].getQuantity();
		double penalty = CANCEL_PENALTY * cost * quantity;
		totalPenalty = totalPenalty + penalty;
	}
}

void Schedule::calculateTotalProfit() {
	for (int x = 0; x < satisfiedVector.size(); x++) {
		totalProfit = totalProfit + satisfiedVector[x].getProfit();
	}
}

void Schedule::calculateTotalCost() {
	for (int x = 0; x < satisfiedVector.size(); x++) {
		totalCost = totalCost + satisfiedVector[x].getTotalCost();

	}
	for (int x = 0; x < delayedVector.size(); x++) {
		totalCost = totalCost + delayedVector[x].getTotalCost();
	}
}

void Schedule::getTotalCycles() {
	for (int x = 0; x < cyclesPerDay.size(); x++) {
		totalCycles = totalCycles + cyclesPerDay[x];
	}
}

void Schedule::chooseDay(int currentCycles, int currentDay) {

	if (currentDay == 0) {
		day0 = currentCycles;
		day1 = 0;
		day2 = 0;
		day3 = 0;
		day4 = 0;
	} else if (currentDay == 1) {
		day0 = 0;
		day1 = currentCycles;
		day2 = 0;
		day3 = 0;
		day4 = 0;
	} else if (currentDay == 2) {
		day0 = 0;
		day1 = 0;
		day2 = currentCycles;
		day3 = 0;
		day4 = 0;
	} else if (currentDay == 3) {
		day0 = 0;
		day1 = 0;
		day2 = 0;
		day3 = currentCycles;
		day4 = 0;
	} else if (currentDay == 4) {
		day0 = 0;
		day1 = 0;
		day2 = 0;
		day3 = 0;
		day4 = currentCycles;
	} else {
		day0 = 0;
		day1 = 0;
		day2 = 0;
		day3 = 0;
		day4 = 0;
	}
}

void Schedule::generateReport() {
	ofstream fout;
	fout.open("result.txt");
	if (fout.fail()) {
		cout << "Cannot open output file." << endl;
	}

	fout << "List of satisfied order: " << satisfiedVector.size() << " in total" << endl;
	fout << "OrderId" << setw(10) << "PCid" << setw(13) << "Quantity"
			<< setw(13) << "Due Date" << setw(10) << "Profit" << setw(13)
			<< "TotalProfit" << setw(9) << "Day 0" << setw(9) << "Day 1"
			<< setw(9) << "Day 2" << setw(9) << "Day 3" << setw(9) << "Day 4"
			<< endl;

	for (int x = 0; x < satisfiedVector.size(); x++) {
		chooseDay(satisfiedVector[x].getTotalCycles(),
				satisfiedVector[x].getDoneDay());
		fout << satisfiedVector[x] << setw(10) << day0 << setw(9) << day1
				<< setw(9) << day2 << setw(9) << day3 << setw(9) << day4
				<< endl;
	}

	fout << endl << "List of delayed order: " << delayedVector.size() << " in total" << endl;
	fout << "OrderId" << setw(10) << "PCid" << setw(13) << "Quantity"
			<< setw(13) << "Due Date" << setw(10) << "Profit" << setw(13)
			<< "TotalProfit" << setw(9) << "Day 0" << setw(9) << "Day 1"
			<< setw(9) << "Day 2" << setw(9) << "Day 3" << setw(9) << "Day 4"
			<< endl;
	for (int x = 0; x < delayedVector.size(); x++) {
		chooseDay(delayedVector[x].getTotalCycles(),
				delayedVector[x].getDoneDay());
		fout << delayedVector[x] << setw(10) << day0 << setw(9) << day1
				<< setw(9) << day2 << setw(9) << day3 << setw(9) << day4
				<< endl;
	}

	fout << endl << "List of cancelled order: " << unsatisfiedVector.size() << " in total"
			<< endl;
	fout << "OrderId" << setw(10) << "PCid" << setw(13) << "Quantity"
			<< setw(13) << "Due Date" << setw(10) << "Profit" << setw(13)
			<< "TotalProfit" << setw(9) << "Day 0" << setw(9) << "Day 1"
			<< setw(9) << "Day 2" << setw(9) << "Day 3" << setw(9) << "Day 4"
			<< endl;
	for (int x = 0; x < unsatisfiedVector.size(); x++) {
		chooseDay(0, 5);
		fout << unsatisfiedVector[x] << setw(10) << day0 << setw(9) << day1
				<< setw(9) << day2 << setw(9) << day3 << setw(9) << day4
				<< endl;
	}

	fout << endl << "List of total components: " << totalComponent << " in total" <<  endl;
	fout << left << setw(14) << "ComponentId" << setw(21) << "ComponentName"
			<< setw(10) << "Amount" << endl;

	for (int x = 0; x < 10; x++) {
		Component component(x);
		fout << "     " << left << setw(9) << x << setw(21)
				<< component.getName() << componentsUsed[x] << endl;
	}

	fout << endl << "List of total components per day: " << totalComponent << " in total" << endl;
	fout << setw(7) << "Day 0" << setw(7) << "Day 1" << setw(7) << "Day 2"
			<< setw(7) << "Day 3" << setw(7) << "Day 4" << endl;
	for (int x = 0; x < componentPerDay.size(); x++) {
		fout << setw(7) << componentPerDay[x];
	}
	fout << endl;

	fout << endl << "List of cycles per day: " << totalCycles << " in total" << endl;
	fout << setw(7) << "Day 0" << setw(7) << "Day 1" << setw(7) << "Day 2"
			<< setw(7) << "Day 3" << setw(7) << "Day 4" << endl;
	for (int x = 0; x < cyclesPerDay.size(); x++) {
		fout << setw(7) << cyclesPerDay[x];
	}
	fout << endl;

	fout << endl << "Total penalty: " << totalPenalty << endl;
	fout << endl << "Total profit: " << totalProfit << endl;
	fout << endl << "Total cost: " << totalCost << endl;

	fout.close();
	cout << "Done" << endl;
}

bool Schedule::validForChange(int x, int i) {
	int day = satisfiedVector[x].getDoneDay();
	if ((cyclesPerDay[day] + unsatisfiedVector[i].getTotalCycles())
			< CYCLES_LIMIT) {
		return true;
	}
	return false;
}

bool Schedule::late(int i) {
	if (unsatisfiedVector[i].getDueDate() < unsatisfiedVector[i].getDoneDay()) {
		return true;
	}
	return false;
}

#endif /* SCHEDULING_H_ */
