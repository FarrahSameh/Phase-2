#pragma once
#include <iostream>
#include <string>
using namespace std;

#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "Cooking_Orders.h"
#include "RDY_OV.h"
#include "Pend_OVC.h"
#include "Fit_Tables.h"

#include "Order.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "Action.h"

class UI
{
public:
	UI() {}
	~UI() {}

	void PrintInterface(
		int currentTime,
		LinkedQueue<Action*>* pActionList,
		LinkedQueue<Order*>* pPendODG,
		LinkedQueue<Order*>* pPendODN,
		LinkedQueue<Order*>* pPendOT,
		LinkedQueue<Order*>* pPendOVN,
		Pend_OVC* pPendOVC,
		priQueue<Order*>* pPendOVG,
		LinkedQueue<Chef*>* pFreeCS,
		LinkedQueue<Chef*>* pFreeCN,
		Cooking_Orders* pCookOrds,
		LinkedQueue<Order*>* pRdyOD,
		LinkedQueue<Order*>* pRdyOT,
		RDY_OV* pRdyOV,
		priQueue<Scooter*>* pFreeScooter,
		priQueue<Scooter*>* pBackScooter,
		LinkedQueue<Scooter*>* pMaintScooter,
		Fit_Tables* pFreeTables,
		priQueue<Order*>* pInserv,
		LinkedQueue<Order*>* pCancelled,
		ArrayStack<Order*>* pFinished,
		/////
		bool isInteractive
		/////
	);
};