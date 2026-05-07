#include "RequestAction.h"
#include "Restaurant.h"

void RequestAction::Act(Restaurant* pRest)
{
    pRest->AddOrderToPendingList(pOrder);
}