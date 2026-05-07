#include "Table.h"
#include "Order.h"

#include <iostream>
using namespace std;

Table::Table(int id, int capacity) : 
	ID(id), Capacity(capacity), FreeSeats(capacity) {}

int Table::getID() const { return ID; }
int Table::getCapacity() const { return Capacity; }
int Table::getFreeSeats() const { return FreeSeats; }

bool Table::CanFitOrder(int ordersize, bool orderCanShare)
{
	if (ordersize > FreeSeats)
		return false;
	if (ordersize < FreeSeats && !orderCanShare)
		return false;
	return true;
}

void Table::OccupySeats(int count)
{
	if (count <= FreeSeats)
		FreeSeats -= count;
}

void Table::ReleaseSeats(int count)
{
	FreeSeats += count;
	
	//safety check to never exceed original capacity
	if (FreeSeats > Capacity)
		FreeSeats = Capacity;
}

std::ostream& operator<<(std::ostream& os, const Table* t)
{
	if (t)
	{
		os << "[T" << t->getID() << ", " << t->getCapacity() << ", " << t->getFreeSeats() << "]" << endl;
	}
	return os;
}
