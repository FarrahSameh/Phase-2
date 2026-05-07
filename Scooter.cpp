#include "Scooter.h"
#include "Order.h"


Scooter::Scooter(int id, int speed, int MainD, int MainO) : 
ID(id), Speed(speed), Main_Dur(MainD), Main_Ords(MainO), status (SAvailable), TripsSinceMain(0), DistanceSoFar(0), finishTime(-1), maintime(0), assignedOrder(nullptr){}

std::ostream& operator<<(std::ostream& os, const Scooter* s)
{
	if (s)
	{
		os << s->getID();
	}
	return os;
}

int Scooter::getID() const { return ID; }
int Scooter::getSpeed() const { return Speed; }

int Scooter::getDistSoFar() const { return DistanceSoFar; }

SCOOTER_STATUS Scooter::getStatus() const { return status; }

Order* Scooter::getassignedOrder() const { return assignedOrder; }


void Scooter::setDistSoFar(int d) { DistanceSoFar = d; }
void Scooter::setTripsSinceMain(int T) { TripsSinceMain = T; }

void Scooter::setStatus(SCOOTER_STATUS s) { status = s; }

// calculating how long the trip takes 
void Scooter::startDelivery(Order* o, int currentTime) 
{
	assignedOrder = o;
	status = SDelivering;

	assignedOrder->setTS(currentTime);
	assignedOrder->setSTATUS(INSERVICE);

	int Traveltime = (o->getDISTANCE() + Speed - 1) / Speed;

	if (Traveltime == 0) Traveltime = 1; // first order

	finishTime = Traveltime + currentTime;

	DistanceSoFar += o->getDISTANCE();
	TripsSinceMain++;

}

void Scooter::update(int currentTime)
{
	// maintenance
	if (status == SInMain)
	{ 
		maintime--;

	     if (maintime <= 0)
	     {
		    status = SAvailable;
		    TripsSinceMain = 0;
		    maintime = 0;
	     }
	}

	// Delivering
	else if (status == SDelivering && currentTime >= finishTime)
	{
		assignedOrder->setTF(currentTime);
		assignedOrder->setSTATUS(FINISHED);


		status = SBack;
		//assuming the distance taken to reach customer is same as returning back to restaurant
		
		int ReturnTrip = (assignedOrder->getDISTANCE() + Speed - 1) / Speed;
		if (ReturnTrip == 0) ReturnTrip = 1;
		finishTime = currentTime + ReturnTrip;
		assignedOrder = nullptr;
	}

	// back
	else if (status == SBack && currentTime >= finishTime)
	{
		if (TripsSinceMain >= Main_Ords)
		{
			status = SInMain;
			maintime = Main_Dur;
		}
		else
		{
			status = SAvailable;
		}
		finishTime = -1; // reset
	} 

}
