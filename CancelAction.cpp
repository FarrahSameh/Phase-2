#include "CancelAction.h"
#include "Restaurant.h"

void CancelAction::Act(Restaurant* pRest)
{
    pRest->CancelOrder(orderID);
}