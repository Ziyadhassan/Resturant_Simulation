#include "Promotion.h"



Promotion::Promotion(int eTime, int oID, double money) :Event(eTime, oID)
{
	ExtraMoney = money;
}

void Promotion::Execute(Restaurant * pRest)
{
	pRest->OrderPromotion(OrderID, ExtraMoney);
}