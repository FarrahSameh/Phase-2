#pragma once
#include "priQueue.h"
#include "Table.h"

class Fit_Tables : public priQueue<Table*>
{
public:

	void print() const  
	{
		cout << count << " tables: ";
		priNode<Table*>* curr = head;
		while (curr != nullptr)
		{
			int p;
			Table* t = curr->getItem(p);
			cout << "[T" << t->getID() << ", " << t->getCapacity() << ", " << t->getFreeSeats() << "] ";
			curr = curr->getNext();
		}
		cout << endl;
	}

	Table* getBest(int requestedSeats)
	{
		if (isEmpty()) return nullptr;

		priNode<Table*>* prev = nullptr;
		priNode<Table*>* curr = head; 

		while (curr != nullptr)
		{
			int p;
			Table* pTbl = curr->getItem(p);

			if (pTbl->getFreeSeats() >= requestedSeats)
			{
				// this is the best fit as the list is sorted by seat count
				if (curr == head)
					head = head->getNext();
				else
					prev->setNext(curr->getNext());

				delete curr;
				count--;
				return pTbl;
			}

			prev = curr;
			curr = curr->getNext();
		}
		return nullptr;
	}
};