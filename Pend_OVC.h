#pragma once
#include "LinkedQueue.h"
#include "Order.h"

class Pend_OVC : public LinkedQueue<Order*>
{
public: 

	Order* CancelOrder(int id)
	{
		if (isEmpty()) return nullptr;

		Node<Order*>* prev = nullptr;
		Node<Order*>* curr = frontPtr;

		while (curr != nullptr)
		{
			if (curr->getItem()->getID() == id)
			{
				Order* orderToCancell = curr->getItem();
				
				// first case: first node
				if (curr == frontPtr)
				{
					frontPtr = frontPtr->getNext();
					if (frontPtr == nullptr)
						backPtr == nullptr;
				}

				// second case: middle/last node
				else
				{
					prev->setNext(curr->getNext());
					if (curr == backPtr)
					{
						backPtr = prev;
					}
				}	
				delete curr;
				count--;
				return orderToCancell;
			}
			// if not, move to the next node to check id
			prev = curr;
			curr = curr->getNext();
		}
		return nullptr; // id not found 
	}
};