#pragma once
#include <iostream>

class Order;

class Table
{
private:
	int ID;
	int Capacity;   // Total no. of seats
	int FreeSeats;

public:

	Table(int id, int capacity);

	friend std::ostream& operator<<(std::ostream& os, const Table* t);

	//getters
	int getID() const;
	int getCapacity() const;
	int getFreeSeats() const;


	bool CanFitOrder(int Oodersize, bool orderCanShare);
	void OccupySeats(int count);
	void ReleaseSeats(int count);



};

