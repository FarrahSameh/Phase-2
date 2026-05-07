#pragma once
#include <iostream>

enum SCOOTER_STATUS {SBack, SInMain, SDelivering, SAvailable};

class Order;
class Scooter
{
private:

	int ID;
	int Speed;
	int Main_Dur; // time needed to fix a scooter
	             // each scooter should go to maintainance after being assigned to a Main_Ords
	int Main_Ords; //orders allowed before maintenance
	int DistanceSoFar;
	int TripsSinceMain;
	SCOOTER_STATUS status;

	int finishTime; //timestep when current task ends
	int maintime; //time remaining in maintenance

	Order* assignedOrder; // order it is currently delivering

public:

	friend std::ostream& operator<<(std::ostream& os, const Scooter* s);

	Scooter( int id, int speed, int MainD, int MainO);


	//getters
	int getID() const;
	int getSpeed() const;
	
	int getDistSoFar() const;
	SCOOTER_STATUS getStatus() const;
	
	Order* getassignedOrder() const;


	//setters
	void setDistSoFar(int d);
	void setTripsSinceMain(int T);

	void setStatus(SCOOTER_STATUS s);

	void startDelivery(Order* o, int currentTime);
	void update(int currentTime);
};