#include "Order.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"

Order::Order(int id, int size, double price, ORD_TYP t, int tq, bool share, int seats, int duration, int distance) : 

	ID(id), SIZE(size), PRICE(price), NoOfSeats(seats), OrderDuration(duration), CanShare(share), Distance(distance), type(t), TQ(tq)
{
	status = PENDING;
	TA = TR = TS = TF = -1; // not yet assigned
	assignedchef = nullptr;
	assignedscooter = nullptr;
	assignedtable = nullptr;
}

int Order::getID() const { return ID; }

int Order::getSIZE() const { return SIZE; }

int Order::getDISTANCE() const { return Distance; }

ORD_STATUS Order::getSTATUS() const { return status; }

bool Order::getCanShare() const { return CanShare; }

int Order::getIdleTime() const 
{ 
	int T1 = 0 , T2 = 0;
	// checking if time is = -1
	if (TA != -1) { T1 = TA - TQ; }
	if (TS != -1 && TR != -1) { T2 = TS - TR; }
	return T1 + T2; 
}

int Order::getWaitTime() const { return getIdleTime() + getCookPeriod(); }
int Order::getCookPeriod() const 
{ 
	if (TR != -1 && TA != -1) { return TR - TA; }
	return 0;
}
int Order::getServiceDuration() const 
{ 
	if (TF != -1 && TS != -1) { return TF - TS; }
	return 0;
}

int Order::getSEATS() const { return NoOfSeats; }

int Order::getDURATION() const { return OrderDuration; }

double Order::getPRICE() const { return PRICE; }

ORD_TYP Order::getTYPE() const { return type; }

int Order::getTQ() const { return TQ; }

double Order::getPriorityWeight() const // to prioritize OVG orders
{
	if (type != OVG)
		return 0.0;
	return (PRICE * 2.0) / (SIZE + Distance);
}

Chef* Order::getChef() const { return assignedchef; }
int Order::getChefID() const
{
	if (assignedchef)
		return assignedchef->getID();
	return -1; // if no chef is assigned 
}

Table* Order::getTable() const { return assignedtable; }
int Order::getTableID() const
{
	if (assignedtable)
		return assignedtable->getID();
	return -1; // if no table is assigned 
}

Scooter* Order::getScooter() const { return assignedscooter; }
int Order::getScooterID() const
{
	if (assignedscooter)
		return assignedscooter->getID();
	return -1; // if no scooter is assigned
}

void Order::setSTATUS(ORD_STATUS s) { status = s; }

void Order::setTA(int time) { TA = time; }
void Order::setTR(int time) { TR = time; }
void Order::setTS(int time) { TS = time; }
void Order::setTF(int time) { TF = time; }

void Order::setChef(Chef* c) { assignedchef = c; }
void Order::setScooter(Scooter* s) { assignedscooter = s; }
void Order::setTable(Table* t) { assignedtable = t; }


// printing the type of order
const char* Order::OrderType() const
{
	switch (type)
	{
	case ODG: return "ODG";
	case ODN: return "ODN";
	case OT: return "OT";
	case OVG: return "OVG";
	case OVN: return "OVN";
	case OVC: return "OVC";
	default: return "UNKNOWN";
	}
}

std::ostream& operator<<(std::ostream& os, const Order* o)
{
	if (o) // safety check if null pointer
	{
		if (o->getSTATUS() == PENDING || o->getSTATUS() == READY || o->getSTATUS() == CANCELLED || o->getSTATUS() == FINISHED) 
			os << o->getID();
		
		else if (o->getSTATUS() == COOKING)
			os << "[" << o->getID() << "," << o->getChefID() << "]";

		else if (o->getSTATUS() == INSERVICE) // OD and OV orders only
		{
			os << "[" << o->getID() << ",";
			if (o->getTYPE() == OVG || o->getTYPE() == OVN || o->getTYPE() == OVC)
			{
				os << "S" << o->getScooterID();
			}
			else if (o->getTYPE() == ODG || o->getTYPE() == ODN)
			{
				os << "T" << o->getTableID();
			}
			os << "]";
		}
	}
	return os;
}

/////////////////////////Mostafa////////////////////////////

// This parts retrurn the assigment time, ready time , service start time and finish time

int Order::getTA() const { return TA; }
int Order::getTR() const { return TR; }
int Order::getTS() const { return TS; }
int Order::getTF() const { return TF; }
///////////////////////////////////////////////////////////


