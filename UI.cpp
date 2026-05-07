#include "UI.h"

void UI::PrintInterface(
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
)
{
	if (!isInteractive)
	{
		if (currentTime % 10 == 0)
		{
			cout << "Timestep: " << currentTime << " - Simulation running..." << endl;
		}
		return;
	}

	cout << "\nCurrent Timestep: " << currentTime << endl;
	cout << "=============== Actions List ================" << endl;
	pActionList->print(10);

	cout << "\n-------------- Pending Orders IDs ----------------------" << endl;
	pPendODG->print();
	pPendODN->print();
	pPendOT->print();
	pPendOVC->print();
	pPendOVN->print();
	pPendOVG->print();

	cout << "\n-------------- Available chefs IDs -----------------------" << endl;
	pFreeCS->print();
	pFreeCN->print();

	cout << "\n-------------- Cooking orders [Orders ID, chef ID] ----------------------" << endl;
	pCookOrds->print();

	cout << "\n-------------- Ready Orders IDs ----------------------" << endl;
	pRdyOD->print();
	pRdyOT->print();
	pRdyOV->print();

	cout << "\n-------------- Available scooters IDs -----------------------" << endl;
	pFreeScooter->print();

	cout << "\n-------------- Available tables [ID, capacity, free seats] -----------------------" << endl;
	pFreeTables->print();

	cout << "\n-------------- In-Service orders [order ID, scooter/Table ID] ------------------------" << endl;
	pInserv->print();

	cout << "\n-------------- In-maintenance scooters IDs ----------------------" << endl;
	pMaintScooter->print();

	cout << "\n-------------- Scooters Back to Restaurant IDs ----------------------" << endl;
	pBackScooter->print();

	cout << "\n-------------- Cancelled Orders IDs ----------------------" << endl;		pCancelled->print();

	cout << "\n-------------- Finished orders IDs ----------------------------" << endl;
	pFinished->print();

	cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
	cin.get();
}