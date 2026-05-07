#pragma once
#include "Action.h"
#include "Order.h"

class Restaurant;

class RequestAction : public Action
{
private:
	Order* pOrder;

public:
	RequestAction(int t, Order* pOrd) : Action(t), pOrder(pOrd) {}
	void Act(Restaurant* pRest) override;
	~RequestAction() {}

	// ← ADDED: prints [OrderType, Tq, orderID]
	void print(ostream& os) const override
	{
		os << "[" << pOrder->OrderType() << ", " << timestamp << ", " << pOrder->getID() << "]";
	}
};