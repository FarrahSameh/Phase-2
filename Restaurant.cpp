#include "Restaurant.h"
#include "RequestAction.h"
#include "CancelAction.h"

#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

Restaurant::Restaurant() : currentTime(0)
{
	pUI = new UI();
}

void Restaurant::AddOrderToPendingList(Order* pOrd)
{
	if (!pOrd) return;

	switch (pOrd->getTYPE())
	{
	case ODG: PEND_ODG_LIST.enqueue(pOrd); break;
	case ODN: PEND_ODN_LIST.enqueue(pOrd); break;
	case OT:  PEND_OT_LIST.enqueue(pOrd);  break;
	case OVC: PEND_OVC_LIST.enqueue(pOrd); break;
	case OVN: PEND_OVN_LIST.enqueue(pOrd); break;
	case OVG: PEND_OVG_LIST.enqueue(pOrd, (int)pOrd->getPriorityWeight()); break;
	}
}

void Restaurant::AssignOrderToChef(Order* pOrd)
{
	if (!pOrd) return;

	Chef* pChef = nullptr;
	ORD_TYP type = pOrd->getTYPE();

	switch (type)
	{
	case ODG:
	case OVG:
		Free_CS_LIST.dequeue(pChef);
		break;

	case ODN:
	case OVC:
		if (!Free_CN_LIST.dequeue(pChef))
			Free_CS_LIST.dequeue(pChef);
		break;

	case OT:
	case OVN:
		Free_CN_LIST.dequeue(pChef);
		break;
	}

	if (!pChef) return;

	pChef->AssignOrder(pOrd, currentTime);
	pOrd->setChef(pChef);
	pOrd->setTA(currentTime);
	pOrd->setSTATUS(COOKING);
}

Order* Restaurant::CancelOrder(int id)
{
	Order* pCancelled = nullptr;

	pCancelled = PEND_OVC_LIST.CancelOrder(id);

	if (!pCancelled)
	{
		pCancelled = Cook_Ords_LIST.CancelOrder(id);

		if (pCancelled)
		{
			if (pCancelled->getChef())
			{
				pCancelled->getChef()->setStatus(Free);
				pCancelled->getChef()->setCurrentOrder(nullptr);
			}
			pCancelled->setChef(nullptr);
		}
	}

	if (!pCancelled)
	{
		pCancelled = RDY_OV_LIST.CancelOrder(id);
	}

	if (pCancelled)
	{
		pCancelled->setSTATUS(CANCELLED);
		Cancelled_Orders_LIST.enqueue(pCancelled);
	}
	return pCancelled;
}

template <typename T>
bool Restaurant::TakeRandomFromLinkedQueue(LinkedQueue<T*>& q, T*& chosen)
{
	chosen = nullptr;

	if (q.isEmpty())
		return false;

	LinkedQueue<T*> temp;
	T* item = nullptr;
	int n = 0;

	while (q.dequeue(item))
	{
		temp.enqueue(item);
		n++;
	}

	int randomIndex = rand() % n;

	for (int i = 0; i < n; i++)
	{
		temp.dequeue(item);

		if (i == randomIndex)
			chosen = item;
		else
			q.enqueue(item);
	}

	return true;
}

template <typename T>
bool Restaurant::TakeRandomFromPriQueue(priQueue<T*>& q, T*& chosen, int& chosenPri)
{
	chosen = nullptr;
	chosenPri = -1;

	if (q.isEmpty())
		return false;

	priQueue<T*> temp;
	T* item = nullptr;
	int pri = 0;
	int n = 0;

	while (q.dequeue(item, pri))
	{
		temp.enqueue(item, pri);
		n++;
	}

	int randomIndex = rand() % n;

	for (int i = 0; i < n; i++)
	{
		temp.dequeue(item, pri);

		if (i == randomIndex)
		{
			chosen = item;
			chosenPri = pri;
		}
		else
		{
			q.enqueue(item, pri);
		}
	}

	return true;
}

void Restaurant::GenerateRandomOrders()
{
	srand(time(0));
	int totalOrders = 500;

	for (int i = 1; i <= totalOrders; i++)
	{
		int typeRng = rand() % 6;
		ORD_TYP type;

		switch (typeRng)
		{
		case 0: type = ODG; break;
		case 1: type = ODN; break;
		case 2: type = OT;  break;
		case 3: type = OVC; break;
		case 4: type = OVN; break;
		case 5: type = OVG; break;
		}

		int size = rand() % 10 + 1;
		double price = (rand() % 191 + 10);
		int tq = rand() % 100 + 1;

		int seats = 0;
		int duration = 0;
		bool canShare = false;
		int distance = 0;

		if (type == ODG || type == ODN)
		{
			seats = rand() % 8 + 1;
			duration = rand() % 20 + 5;
			canShare = (rand() % 2 == 0);
		}
		else if (type == OVC || type == OVN || type == OVG)
		{
			distance = rand() % 4901 + 100;
		}

		Order* pOrd = new Order(i, size, price, type, tq, canShare, seats, duration, distance);
		AddOrderToPendingList(pOrd);
	}
}

void Restaurant::LoadFromFile(const string& filename)
{
	string currentFilename = filename;
	ifstream fin;
	
	while (true)
	{
		fin.open(currentFilename);
		if (fin.is_open())
		{
			break;
		}
		
		cout << "Error: Could not open file " << currentFilename << endl;
		cout << "Please enter a valid file name: ";
		cin >> currentFilename;
		fin.clear();
	}

	int cnCount, csCount;
	fin >> cnCount >> csCount;

	int cnSpeed, csSpeed;
	fin >> cnSpeed >> csSpeed;

	for (int i = 1; i <= cnCount; i++)
	{
		Chef* pChef = new Chef(i, cnSpeed, CN);
		Free_CN_LIST.enqueue(pChef);
		/////
		ALL_CHEFS_LIST.enqueue(pChef);
		/////
	}
	for (int i = cnCount + 1; i <= cnCount + csCount; i++)
	{
		Chef* pChef = new Chef(i, csSpeed, CS);
		Free_CS_LIST.enqueue(pChef);
		/////
		ALL_CHEFS_LIST.enqueue(pChef);
		/////
	}

	int scooterCount, scooterSpeed;
	fin >> scooterCount >> scooterSpeed;

	int mainOrds, mainDur;
	fin >> mainOrds >> mainDur;

	for (int i = 1; i <= scooterCount; i++)
	{
		Scooter* pScoot = new Scooter(i, scooterSpeed, mainDur, mainOrds);
		Free_Scooter_LIST.enqueue(pScoot, 0);
	}

	int totalTables;
	fin >> totalTables;

	int tablesRead = 0;
	int tableID = 1;
	while (tablesRead < totalTables)
	{
		int count, capacity;
		fin >> count >> capacity;
		for (int i = 0; i < count; i++)
		{
			Table* pTable = new Table(tableID++, capacity);
			Free_Tables_LIST.enqueue(pTable, capacity);
		}
		tablesRead += count;
	}

	int actionCount;
	fin >> actionCount;

	for (int i = 0; i < actionCount; i++)
	{
		char actionType;
		fin >> actionType;

		if (actionType == 'Q')
		{
			string typeStr;
			int tq, id, size;
			double price;
			fin >> typeStr >> tq >> id >> size >> price;

			ORD_TYP type;
			if (typeStr == "ODG")      type = ODG;
			else if (typeStr == "ODN") type = ODN;
			else if (typeStr == "OT")  type = OT;
			else if (typeStr == "OVC") type = OVC;
			else if (typeStr == "OVN") type = OVN;
			else                       type = OVG;

			int seats = 0;
			int duration = 0;
			bool canShare = false;
			int distance = 0;

			if (type == ODG || type == ODN)
			{
				char shareChar;
				fin >> seats >> duration >> shareChar;
				canShare = (shareChar == 'Y' || shareChar == 'y');
			}
			else if (type == OVC || type == OVN || type == OVG)
			{
				fin >> distance;
			}

			Order* pOrd = new Order(id, size, price, type, tq,
				canShare, seats, duration, distance);
			RequestAction* pAct = new RequestAction(tq, pOrd);
			ACTION_LIST.enqueue(pAct);
		}
		else if (actionType == 'X')
		{
			int tCancel, id;
			fin >> tCancel >> id;
			CancelAction* pAct = new CancelAction(tCancel, id);
			ACTION_LIST.enqueue(pAct);
		}
	}

	fin.close();
}

void Restaurant::Simulate()
{
	/////
	cout << "\n Select simulation mode: " << endl;
	cout << "1. Interactive" << endl;
	cout << "2. Silent" << endl;
	int modeChoice;
	cin >> modeChoice;
	cin.ignore();
	bool isInteractive = (modeChoice == 1);

	currentTime = 0;

	Action* pAct = nullptr;

	while (true)
	{
		currentTime++;
		while (!ACTION_LIST.isEmpty())
		{
			ACTION_LIST.peek(pAct);

			if (pAct && pAct->getTimestamp() == currentTime)
			{
				ACTION_LIST.dequeue(pAct);
				pAct->Act(this);
				delete pAct;
			}
			else
				break;
		}
		UpdateCookingOrders();

		Order* pOrd = nullptr;
		int pri = 0;

		while (!Free_CS_LIST.isEmpty() || !Free_CN_LIST.isEmpty())
		{
			bool assigned = false;

			if (PEND_ODG_LIST.dequeue(pOrd))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}
			else if (PEND_OVG_LIST.dequeue(pOrd, pri))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}
			else if (PEND_ODN_LIST.dequeue(pOrd))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}
			else if (PEND_OVC_LIST.dequeue(pOrd))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}
			else if (PEND_OT_LIST.dequeue(pOrd))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}
			else if (PEND_OVN_LIST.dequeue(pOrd))
			{
				AssignOrderToChef(pOrd);
				assigned = true;
			}

			if (!assigned)
				break;

			if (pOrd && pOrd->getSTATUS() == COOKING)
			{
				Cook_Ords_LIST.enqueue(pOrd, pOrd->getChef()->getFinishTime());
			}
		}
		AssignReadyDineInOrdersToTables();
		AssignReadyDeliveryOrdersToScooters();
		CheckFinishedDineInOrders();
		UpdateInServiceDeliveryOrders();
		UpdateScooters();

		pUI->PrintInterface(
			currentTime,
			&ACTION_LIST,
			&PEND_ODG_LIST,
			&PEND_ODN_LIST,
			&PEND_OT_LIST,
			&PEND_OVN_LIST,
			&PEND_OVC_LIST,
			&PEND_OVG_LIST,
			&Free_CS_LIST,
			&Free_CN_LIST,
			&Cook_Ords_LIST,
			&RDY_OD_LIST,
			&RDY_OT_LIST,
			&RDY_OV_LIST,
			&Free_Scooter_LIST,
			&Back_Scooter_LIST,
			&Maint_Scooter_LIST,
			&Free_Tables_LIST,
			&Inserv_Orders_LIST,
			&Cancelled_Orders_LIST,
			&Finished_Orders_Stack,
			isInteractive
		);

		if (ACTION_LIST.isEmpty() &&
			PEND_ODG_LIST.isEmpty() &&
			PEND_ODN_LIST.isEmpty() &&
			PEND_OT_LIST.isEmpty() &&
			PEND_OVC_LIST.isEmpty() &&
			PEND_OVN_LIST.isEmpty() &&
			PEND_OVG_LIST.isEmpty() &&
			Cook_Ords_LIST.isEmpty() &&
			RDY_OD_LIST.isEmpty() &&
			RDY_OT_LIST.isEmpty() &&
			RDY_OV_LIST.isEmpty() &&
			Inserv_Orders_LIST.isEmpty())
		{
			break;
		}
	}

	GenerateOutputFile("output.txt");
	cout << "\n Simulatin Completed! Output written to output.txt" << endl;
	/////
}

/////////////////////////Mostafa////////////////////////
void Restaurant::CheckFinishedDineInOrders()
{
	priQueue<Order*> tempList;

	Order* pOrd = nullptr;
	int priority = 0;

	while (Inserv_Orders_LIST.dequeue(pOrd, priority))
	{
		bool finished = false;

		if (pOrd != nullptr && (pOrd->getTYPE() == ODG || pOrd->getTYPE() == ODN))
		{
			if (pOrd->getTS() != -1 &&
				currentTime - pOrd->getTS() >= pOrd->getDURATION())
			{
				finished = true;
			}
		}

		if (finished)
		{
			Table* pTable = pOrd->getTable();

			if (pTable != nullptr)
			{
				pTable->ReleaseSeats(pOrd->getSEATS());

				if (pTable->getFreeSeats() == pTable->getCapacity())
				{
					Free_Tables_LIST.enqueue(pTable, pTable->getFreeSeats());
				}
				else
				{
					Busy_Sharable_LIST.enqueue(pTable, pTable->getFreeSeats());
				}
			}

			pOrd->setTF(currentTime);
			pOrd->setSTATUS(FINISHED);
			Finished_Orders_Stack.push(pOrd);
		}
		else
		{
			tempList.enqueue(pOrd, priority);
		}
	}

	while (tempList.dequeue(pOrd, priority))
	{
		Inserv_Orders_LIST.enqueue(pOrd, priority);
	}
}

void Restaurant::AssignReadyDineInOrdersToTables()
{
	LinkedQueue<Order*> tempList;

	Order* pOrd = nullptr;

	while (RDY_OD_LIST.dequeue(pOrd))
	{
		if (pOrd == nullptr)
			continue;

		Table* pTable = nullptr;
		int requiredSeats = pOrd->getSEATS();

		// If the order can share, try busy sharable tables first
		if (pOrd->getCanShare())
		{
			pTable = Busy_Sharable_LIST.getBest(requiredSeats);

			if (pTable == nullptr)
				pTable = Free_Tables_LIST.getBest(requiredSeats);
		}
		else
		{
			// If cannot share, use only a free table
			pTable = Free_Tables_LIST.getBest(requiredSeats);
		}

		// If table found
		if (pTable != nullptr)
		{
			pTable->OccupySeats(requiredSeats);

			pOrd->setTable(pTable);
			pOrd->setTS(currentTime);
			pOrd->setSTATUS(INSERVICE);

			Inserv_Orders_LIST.enqueue(pOrd, currentTime + pOrd->getDURATION());

			// Put table in the correct table list
			if (pTable->getFreeSeats() == 0)
			{
				Busy_No_Share_LIST.enqueue(pTable, pTable->getFreeSeats());
			}
			else if (pOrd->getCanShare())
			{
				Busy_Sharable_LIST.enqueue(pTable, pTable->getFreeSeats());
			}
			else
			{
				Busy_No_Share_LIST.enqueue(pTable, pTable->getFreeSeats());
			}
		}
		else
		{
			// No table available, keep order ready
			tempList.enqueue(pOrd);
		}
	}

	// Return unassigned orders back to ready list
	while (tempList.dequeue(pOrd))
	{
		RDY_OD_LIST.enqueue(pOrd);
	}
}

void Restaurant::AssignReadyDeliveryOrdersToScooters()
{
	Order* pOrd = nullptr;
	Scooter* pScoot = nullptr;
	int scooterPriority = 0;

	while (!Free_Scooter_LIST.isEmpty())
	{
		pOrd = nullptr;

		// Serve OVC first, then OVN, then OVG
		if (!RDY_OV_LIST.DequeueByType(OVC, pOrd))
		{
			if (!RDY_OV_LIST.DequeueByType(OVN, pOrd))
			{
				RDY_OV_LIST.DequeueByType(OVG, pOrd);
			}
		}

		// No ready delivery orders
		if (pOrd == nullptr)
			break;

		// Take scooter with shortest distance so far
		if (!Free_Scooter_LIST.dequeue(pScoot, scooterPriority))
		{
			RDY_OV_LIST.enqueue(pOrd);
			break;
		}

		pOrd->setScooter(pScoot);
		pScoot->startDelivery(pOrd, currentTime);

		int tripTime = (pOrd->getDISTANCE() + pScoot->getSpeed() - 1) / pScoot->getSpeed();
		if (tripTime == 0)
			tripTime = 1;

		int finishTime = currentTime + tripTime;

		Inserv_Orders_LIST.enqueue(pOrd, -finishTime);
	}
}
////////////////////////////////////////////////////////

/////
void Restaurant::UpdateCookingOrders()
{
	LinkedQueue<Chef*> tempChefList;
	Chef* pChef = nullptr;

	while (ALL_CHEFS_LIST.dequeue(pChef))
	{
		if (pChef)
		{
			Order* finishedOrder = pChef->Update(currentTime);
			
			if (finishedOrder)
			{
				finishedOrder->setTR(currentTime);
				finishedOrder->setSTATUS(READY);

				ORD_TYP type = finishedOrder->getTYPE();

				if (type == ODG || type == ODN)
				{
					RDY_OD_LIST.enqueue(finishedOrder);
				}
				else if (type == OT)
				{
					RDY_OT_LIST.enqueue(finishedOrder);
				}
				else
				{
					RDY_OV_LIST.enqueue(finishedOrder);
				}

				if (pChef->getType() == CS)
				{
					Free_CS_LIST.enqueue(pChef);
				}
				else
				{
					Free_CN_LIST.enqueue(pChef);
				}
			}
			tempChefList.enqueue(pChef);
		}
	}
	while (tempChefList.dequeue(pChef))
	{
		ALL_CHEFS_LIST.enqueue(pChef);
	}
}

void Restaurant::UpdateInServiceDeliveryOrders()
{
	priQueue<Order*> tempList;
	Order* pOrd = nullptr;
	int priority = 0;

	while (Inserv_Orders_LIST.dequeue(pOrd, priority))
	{
		if (pOrd)
		{
			bool finished = false;

			if (pOrd->getTYPE() == OVC || pOrd->getTYPE() == OVG || pOrd->getTYPE() == OVN)
			{
				if (currentTime >= -priority)
				{
					finished = true;
				}
			}
			
			if (finished)
			{
				pOrd->setTF(currentTime);
				pOrd->setSTATUS(FINISHED);

				// Release scooter only when delivery is complete
				Scooter* pScoot = pOrd->getScooter();
				if (pScoot)
				{
					pScoot->setStatus(SBack);

					int returnTrip = (pOrd->getDISTANCE() + pScoot->getSpeed() - 1) / pScoot->getSpeed();
					if (returnTrip == 0)
					{
						returnTrip = 1;
					}

					int backTime = currentTime + returnTrip;

					Back_Scooter_LIST.enqueue(pScoot, -backTime);
				}
				
				// Move finished order to stack
				Finished_Orders_Stack.push(pOrd);
			}
			else
			{
				// Order not finished yet, keep it in service
				tempList.enqueue(pOrd, priority);
			}
		}
	}

	while (tempList.dequeue(pOrd, priority))
	{
		Inserv_Orders_LIST.enqueue(pOrd, priority);
	}
}

void Restaurant::UpdateScooters()
{
	priQueue<Scooter*> tempBackList;
	Scooter* pScoot = nullptr;
	int priority = 0;

	// Scan Back_Scooter_LIST: check if return trip is complete
	while (Back_Scooter_LIST.dequeue(pScoot, priority))
	{
		if (pScoot)
		{
			if (currentTime >= -priority)  // Return trip complete
			{
				if (pScoot->getDistSoFar() >= pScoot->getSpeed() * 100)
				{
					pScoot->setStatus(SInMain);
					Maint_Scooter_LIST.enqueue(pScoot);
				}
				else
				{
					pScoot->setStatus(SAvailable);
					Free_Scooter_LIST.enqueue(pScoot, pScoot->getDistSoFar());
				}
				// Scooter moved to another list, don't re-enqueue
			}
			else
			{
				// Return trip not complete yet, keep in Back list
				tempBackList.enqueue(pScoot, priority);
			}
		}
	}
	
	// Re-enqueue only scooters still returning
	while (tempBackList.dequeue(pScoot, priority))
	{
		Back_Scooter_LIST.enqueue(pScoot, priority);
	}

	LinkedQueue<Scooter*> tempMainList;
	
	// Scan Maint_Scooter_LIST: decrement maintenance timer
	while (Maint_Scooter_LIST.dequeue(pScoot))
	{
		if (pScoot)
		{
			pScoot->update(currentTime);  // Decrements maintime

			if (pScoot->getStatus() == SAvailable)  // Maintenance complete
			{
				pScoot->setDistSoFar(0);
				Free_Scooter_LIST.enqueue(pScoot, 0);
				// Scooter moved to Free list, don't re-enqueue
			}
			else
			{
				// Still in maintenance, keep in Maint list
				tempMainList.enqueue(pScoot);
			}
		}
	}
	
	// Re-enqueue only scooters still in maintenance
	while (tempMainList.dequeue(pScoot))
	{
		Maint_Scooter_LIST.enqueue(pScoot);
	}
}

void Restaurant::GenerateOutputFile(const string& filename)
{
	ofstream fout(filename);
	if (!fout.is_open())
	{
		cout << "Error: Could not create output file!" << endl;
		return;
	}

	const int MAX_ORDERS = 1000;
	Order* ordersArray[MAX_ORDERS];
	int orderCount = 0;

	Order* pOrd = nullptr;
	while (Finished_Orders_Stack.pop(pOrd))
	{
		if (pOrd && orderCount < MAX_ORDERS)
		{
			ordersArray[orderCount] = pOrd;
			orderCount++;
		}
	}

	//sorting
	for (int i = 0; i < orderCount - 1; i++)
	{
		int maxIndex = i;
		for (int j = i + 1; j < orderCount; j++)
		{
			if (ordersArray[j]->getTF() > ordersArray[maxIndex]->getTF())
			{
				maxIndex = j;
			}
		}
		if (maxIndex != i)
		{
			Order* temp = ordersArray[i];
			ordersArray[i] = ordersArray[maxIndex];
			ordersArray[maxIndex] = temp;
		}
	}

	fout << "TF\tID\tTQ\tTA\tTR\tTS\tTi\tTc\tTw\tTserv" << endl;

	double totalTi = 0, totalTc = 0, totalTw = 0, totalTserv = 0;
	int orderTypeCounts[6] = { 0 };

	for (int i = 0; i < orderCount; i++)
	{
		Order* order = ordersArray[i];

		int Ti = order->getIdleTime();
		int Tc = order->getCookPeriod();
		int Tw = order->getWaitTime();
		int Tserv = order->getServiceDuration();

		fout << order->getTF() << "\t"
			<< order->getID() << "\t"
			<< order->getTQ() << "\t"
			<< order->getTA() << "\t"
			<< order->getTR() << "\t"
			<< order->getTS() << "\t"
			<< Ti << "\t"
			<< Tc << "\t"
			<< Tw << "\t"
			<< Tserv << endl;

		totalTi += Ti;
		totalTc += Tc;
		totalTw += Tw;
		totalTserv += Tserv;


		orderTypeCounts[order->getTYPE()]++;
	}

	for (int i = orderCount - 1; i >= 0; i--)
	{
		Finished_Orders_Stack.push(ordersArray[i]);
	}

	int finishedCount = orderCount;
	int cancelledCount = Cancelled_Orders_LIST.getCount();
	int totalOrders = finishedCount + cancelledCount;

	LinkedQueue<Order*> tempCancelled;
	while (Cancelled_Orders_LIST.dequeue(pOrd))
	{
		if (pOrd)
		{
			orderTypeCounts[pOrd->getTYPE()]++;
			tempCancelled.enqueue(pOrd);
		}
	}
	while (tempCancelled.dequeue(pOrd))
		Cancelled_Orders_LIST.enqueue(pOrd);

	
	int csCount = 0, cnCount = 0;
	LinkedQueue<Chef*> tempChefs;
	Chef* pChef = nullptr;

	while (ALL_CHEFS_LIST.dequeue(pChef))
	{
		if (pChef)
		{
			if (pChef->getType() == CS)
				csCount++;
			else
				cnCount++;
			tempChefs.enqueue(pChef);
		}
	}
	while (tempChefs.dequeue(pChef))
		ALL_CHEFS_LIST.enqueue(pChef);

	int totalChefs = csCount + cnCount;

	int totalScooters = Free_Scooter_LIST.getCount() +
		Back_Scooter_LIST.getCount() +
		Maint_Scooter_LIST.getCount();

	int overwaitCount = 0;
	for (int i = 0; i < orderCount; i++)
	{
		if (ordersArray[i]->getWaitTime() > ordersArray[i]->getServiceDuration())
			overwaitCount++;
	}

	fout << "========== STATISTICS ==========" << endl;

	fout << "\nTotal number of orders: " << totalOrders << endl;
	fout << "ODG: " << orderTypeCounts[ODG] << endl;
	fout << "ODN: " << orderTypeCounts[ODN] << endl;
	fout << "OT: " << orderTypeCounts[OT] << endl;
	fout << "OVC: " << orderTypeCounts[OVC] << endl;
	fout << "OVN: " << orderTypeCounts[OVN] << endl;
	fout << "OVG: " << orderTypeCounts[OVG] << endl;

	fout << "\nTotal number of chefs: " << totalChefs << endl;
	fout << "CS: " << csCount << endl;
	fout << "CN: " << cnCount << endl;

	fout << "\nTotal number of scooters: " << totalScooters << endl;

	double finishedPercent = 0, cancelledPercent = 0;
	if (totalOrders > 0)
	{
		finishedPercent = (finishedCount * 100.0) / totalOrders;
		cancelledPercent = (cancelledCount * 100.0) / totalOrders;
	}

	fout << "\nPercentage of finished orders: " << finishedPercent << "%" << endl;
	fout << "Percentage of cancelled orders: " << cancelledPercent << "%" << endl;

	double overwaitPercent = 0;
	if (finishedCount > 0)
		overwaitPercent = (overwaitCount * 100.0) / finishedCount;

	fout << "\nPercentage of overwait orders: " << overwaitPercent << "%" << endl;

	double avgTi = 0, avgTc = 0, avgTw = 0, avgTserv = 0;
	if (finishedCount > 0)
	{
		avgTi = totalTi / finishedCount;
		avgTc = totalTc / finishedCount;
		avgTw = totalTw / finishedCount;
		avgTserv = totalTserv / finishedCount;
	}

	fout << "\nAverage Ti: " << avgTi << endl;
	fout << "Average Tc: " << avgTc << endl;
	fout << "Average Tw: " << avgTw << endl;
	fout << "Average Tserv: " << avgTserv << endl;

	int totalScooterTime = totalScooters * currentTime;
	int scooterBusyTime = 0;

	for (int i = 0; i < orderCount; i++)
	{
		Order* order = ordersArray[i];
		if (order->getTYPE() == OVC || order->getTYPE() == OVN || order->getTYPE() == OVG)
		{
			int deliveryTime = order->getTF() - order->getTS();
			scooterBusyTime += deliveryTime * 2;
		}
	}

	double scooterUtil = 0;
	if (totalScooterTime > 0)
		scooterUtil = (scooterBusyTime * 100.0) / totalScooterTime;

	fout << "\nScooter utilization: " << scooterUtil << "%" << endl;

	int totalChefTime = totalChefs * currentTime;
	int chefBusyTime = 0;

	for (int i = 0; i < orderCount; i++)
	{
		chefBusyTime += (ordersArray[i]->getTR() - ordersArray[i]->getTA());
	}

	double chefUtil = 0;
	if (totalChefTime > 0)
		chefUtil = (chefBusyTime * 100.0) / totalChefTime;

	fout << "\nChef utilization: " << chefUtil << "%" << endl;

	fout << "\n========================================" << endl;

	fout.close();
}
/////


Restaurant::~Restaurant()
{
	Order* pOrd;

	while (Cancelled_Orders_LIST.dequeue(pOrd)) { delete pOrd; }
	while (Finished_Orders_Stack.pop(pOrd)) { delete pOrd; }

	Chef* pChef;

	while (Free_CS_LIST.dequeue(pChef)) { delete pChef; }
	while (Free_CN_LIST.dequeue(pChef)) { delete pChef; }

	Table* pTbl;
	int p;

	while (Free_Tables_LIST.dequeue(pTbl, p)) { delete pTbl; }
	while (Busy_Sharable_LIST.dequeue(pTbl, p)) { delete pTbl; }
	while (Busy_No_Share_LIST.dequeue(pTbl, p)) { delete pTbl; }

	Scooter* pScoot;
	while (Free_Scooter_LIST.dequeue(pScoot, p)) { delete pScoot; }
	while (Back_Scooter_LIST.dequeue(pScoot, p)) { delete pScoot; }
	while (Maint_Scooter_LIST.dequeue(pScoot)) { delete pScoot; }

	Action* pAct;

	while (ACTION_LIST.dequeue(pAct)) { delete pAct; }

	delete pUI;
}