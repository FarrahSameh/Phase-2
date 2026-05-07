#pragma once
#include "Action.h"

class Restaurant;

class CancelAction : public Action
{
private:
	int orderID;

public:
	CancelAction(int t, int id) : Action(t), orderID(id) {}
	void Act(Restaurant* pRest) override;

	
	void print(ostream& os) const override
	{
		os << "[X, " << timestamp << ", " << orderID << "]";
	}
};