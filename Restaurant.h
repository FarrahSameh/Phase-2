#pragma once

#include <string>
using namespace std;

#include "LinkedQueue.h"
#include "StackADT.h"
#include "ArrayStack.h"
#include "priQueue.h"

#include "Pend_OVC.h"
#include "Cooking_Orders.h"
#include "RDY_OV.h"
#include "Fit_Tables.h"

#include "Order.h"
#include "Chef.h"
#include "Table.h"
#include "Scooter.h"
#include "Action.h"
#include "UI.h"

enum SIM_MODE { INTERACTIVE, SILENT};

class Restaurant
{
private:

	LinkedQueue<Action*> ACTION_LIST;
	
	//pending Orders
	LinkedQueue<Order*> PEND_ODG_LIST;
	LinkedQueue<Order*> PEND_ODN_LIST;
	LinkedQueue<Order*> PEND_OT_LIST;
	LinkedQueue<Order*> PEND_OVN_LIST;
	Pend_OVC PEND_OVC_LIST;
	priQueue<Order*> PEND_OVG_LIST;

	//free chefs
	LinkedQueue<Chef*> Free_CS_LIST;
	LinkedQueue<Chef*> Free_CN_LIST;

	//cancelled orders
	LinkedQueue<Order*> Cancelled_Orders_LIST;

	//finished orders
	ArrayStack<Order*> Finished_Orders_Stack;

	//cooking orders
	Cooking_Orders Cook_Ords_LIST;

	//ready orders
	LinkedQueue<Order*> RDY_OT_LIST;
	RDY_OV RDY_OV_LIST;
	LinkedQueue<Order*> RDY_OD_LIST;

	//inservice orders
	priQueue<Order*> Inserv_Orders_LIST;

	//Scooters
	priQueue<Scooter*> Free_Scooter_LIST;
	priQueue<Scooter*> Back_Scooter_LIST;
	LinkedQueue<Scooter*> Maint_Scooter_LIST;

	//tables
	Fit_Tables Free_Tables_LIST;
	Fit_Tables Busy_Sharable_LIST;
	Fit_Tables Busy_No_Share_LIST;

	int currentTime;
	UI* pUI;  //pointer to UI class for printing

	/////
	LinkedQueue<Chef*> ALL_CHEFS_LIST;  // Tracking all chefs
	/////

public:
	Restaurant();

	void AssignOrderToChef(Order* pOrd);

	void GenerateRandomOrders();

	void AddOrderToPendingList(Order* pOrd);  //called by RequestAction
	Order* CancelOrder(int id);    //called by CancelAction
	                               //order cancellation for OV orders only
	
	template <typename T>
	bool TakeRandomFromLinkedQueue(LinkedQueue<T*>& q, T*& chosen);

	template <typename T>
	bool TakeRandomFromPriQueue(priQueue<T*>& q, T*& chosen, int& chosenPri);
	
	void LoadFromFile(const string& filename);
	
	void Simulate();
	
	/////////////////Mostafa/////////////////////////
	void CheckFinishedDineInOrders();
	void AssignReadyDineInOrdersToTables();
	void AssignReadyDeliveryOrdersToScooters();
	////////////////////////////////////////////////
	
	/////
	void UpdateCookingOrders();
	void UpdateInServiceDeliveryOrders();
	void UpdateScooters();
	void GenerateOutputFile(const string& filename);
	/////
	
	
	
	
	~Restaurant();
	//the first change
};

