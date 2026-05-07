#pragma once
#include "priQueue.h"
#include "Order.h"

class Cooking_Orders : public priQueue<Order*>
{
public:

	void print() const 
	{
		cout << count << " cooking orders: ";
		priNode<Order*>* curr = head;
		while (curr != nullptr)
		{
			int p;
			Order* o = curr->getItem(p);
			cout << "[" << o->getID() << ", " << o->getChefID() << "] ";
			curr = curr->getNext();
		}
		cout << endl;
	}

	Order* CancelOrder(int id)
	{
		if (isEmpty()) return nullptr;

		priNode<Order*>* prev = nullptr;
		priNode<Order*>* curr = head;

		while (curr != nullptr)
		{
			int p;
			Order* pOrd = curr->getItem(p); 

			if (pOrd->getID() == id)
			{
				if (curr == head) 
				{
					head = head->getNext();
				}
				else 
				{
					prev->setNext(curr->getNext());
				}
				delete curr;
				count--;
				return pOrd;
			}
			
			prev = curr;
			curr = curr->getNext();
		}
		return nullptr;
	}
};