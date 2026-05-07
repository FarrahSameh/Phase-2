#pragma once
#include <iostream>

enum ORD_STATUS {PENDING, COOKING, READY, INSERVICE, FINISHED, CANCELLED};
enum ORD_TYP {ODG, ODN, OVC, OVG, OVN, OT};

class Chef;
class Table;
class Scooter;



class Order
{
	

private:
	int ID; // sequence number for each order
	int SIZE; // Number of dishes
	double PRICE;

	// for OD orders
	int NoOfSeats; 
	int OrderDuration; 
	bool CanShare; 

	// for OV orders
	int Distance; 
	ORD_STATUS status;
	ORD_TYP type;

	int TQ; // Request time
	int TA; // Assignment time
	int TR; // Ready time
	int TS; // Service start time
	int TF; // Finish time

	Chef* assignedchef;
	Table* assignedtable;
	Scooter* assignedscooter;

public:

	// overloading << operator to allow printing Order object
	friend std::ostream& operator<<(std::ostream& os, const Order* o);

	const char* OrderType() const;

	Order(int id, int size, double price, ORD_TYP t, int tq, bool share = false, int seats = 0, int duration = 0, int distance = 0);

	// getters
	int getID() const;
	int getSIZE() const;
	double getPRICE() const;
	ORD_TYP getTYPE() const;
	int getTQ() const;
	int getSEATS() const;
	int getDURATION() const;
	int getDISTANCE() const;

	ORD_STATUS getSTATUS() const;
	bool getCanShare() const;

	int getCookPeriod() const; // Tc
	int getServiceDuration() const; // Tserv
	int getIdleTime() const; // Ti
	int getWaitTime() const; // Tw

	double getPriorityWeight() const;

	Chef* getChef() const;
	int getChefID() const;

	Table* getTable() const;
	int getTableID() const;

	Scooter* getScooter() const;
	int getScooterID() const;


	// setters
	void setSTATUS(ORD_STATUS s);

	void setTA(int time);
	void setTR(int time);
	void setTS(int time);
	void setTF(int time);

	void setChef(Chef* c);
	void setTable(Table* t);
	void setScooter(Scooter* s);



	/////////////////////////Mostafa////////////////////////////
	int getTA() const;
	int getTR() const;
	int getTS() const;
	int getTF() const;
	///////////////////////////////////////////////////////////


}; 


