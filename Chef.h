#pragma once
#include <iostream>

enum CHEF_TYPE { CS, CN };
enum CHEF_STATUS { Busy, Free};

class Order;

class Chef
{
private:

	int ID;
	int Speed; // number of dishes in 1 timestep
	CHEF_TYPE type;
	CHEF_STATUS status;
	Order* currentOrder; // the Order the chef is currently working on
	int TF; // the timestep the chef will finish the order



public:
	Chef(int id, int s, CHEF_TYPE t);
	friend std::ostream& operator<<(std::ostream& os, const Chef* c);


	//getters

	int getID() const;
	int getSpeed() const;
	CHEF_TYPE getType() const;
	CHEF_STATUS getStatus() const;
	Order* getCurrentOrder() const;
	int getFinishTime() const;




	//setters

	void setType(CHEF_TYPE t);
	void setStatus(CHEF_STATUS s);
	void setCurrentOrder(Order* o);
	void setTF(int time);

	void AssignOrder(Order* o, int currentTime);
	Order* Update(int currentTime);
	int CookPeriod(int ordersize);


};

