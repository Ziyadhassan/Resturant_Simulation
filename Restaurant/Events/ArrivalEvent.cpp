#include "ArrivalEvent.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int size, double money, int promtionlimit ,int vipPromotionlimit) :Event(eTime, oID)
{
	OrdType = oType;
	Size = size;
	OrdMoney = money;
	AutoPromotionlimit = promtionlimit; 
	VIP_PrmotionLimit = (OrdType == TYPE_VIP) ? vipPromotionlimit : 0;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID, OrdType, OrdMoney, Size, EventTime, AutoPromotionlimit , VIP_PrmotionLimit);

	pRest->OrderArrival(pOrd);

}
