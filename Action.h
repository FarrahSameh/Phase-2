#pragma once
#include <iostream>  
using namespace std; 

class Restaurant;

class Action
{
protected:
	int timestamp; // timestep when this action should execute

public:
	Action(int t) : timestamp(t) {}
	int getTimestamp() const { return timestamp; }
	virtual void Act(Restaurant* pRest) = 0; // pure virtual as spec says
	virtual ~Action() {}

	// ← ADDED: pure virtual so each subclass prints its own format
	virtual void print(ostream& os) const = 0;

	// ← ADDED: stream operator calls virtual print
	friend ostream& operator<<(ostream& os, const Action* a)
	{
		a->print(os);
		return os;
	}
};