#include "Chef.h"
#include "Order.h"

Chef::Chef(int id, int s, CHEF_TYPE t) : 
	ID(id), Speed(s), type(t), status(Free), currentOrder(nullptr), TF(-1) { }

int Chef::getID() const { return ID; }
int Chef::getSpeed() const { return Speed; }
CHEF_STATUS Chef::getStatus() const { return status; }
int Chef::getFinishTime() const { return TF; }
Order* Chef::getCurrentOrder() const { return currentOrder; }
CHEF_TYPE Chef::getType() const { return type; }

void Chef::setCurrentOrder(Order* o) { currentOrder = o; }
void Chef::setStatus(CHEF_STATUS s) { status = s; }
void Chef::setType(CHEF_TYPE t) { type = t; }
void Chef::setTF(int time) { TF = time; }

void Chef::AssignOrder(Order* o, int currentTime)
{
	if (o)
	{
		currentOrder = o;
		status = Busy;

		currentOrder->setSTATUS(COOKING);
		currentOrder->setTA(currentTime);
		currentOrder->setChef(this);

		//calculating when the order be ready
		int duration = CookPeriod(o->getSIZE());

		TF = currentTime + duration;
	}
}
Order* Chef::Update(int currentTime)
{
	if (status == Busy && currentTime == TF)
	{
		Order* finishedOrder = currentOrder;

		finishedOrder->setTR(currentTime);
		finishedOrder->setSTATUS(READY);

		currentOrder = nullptr;
		status = Free;
		TF = -1;
		return finishedOrder;
	}

	return nullptr;
}
int Chef::CookPeriod(int orderSize)
{
	return (orderSize + Speed - 1) / Speed;
}

std::ostream& operator<<(std::ostream& os, const Chef* c)
{
	if (c)
	{
		os << c->getID();
	}
	return os;
}