#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"
#include "..\Rest\Restaurant.h"



//class for the arrival event
class ArrivalEvent : public Event
{
	//info about the order ralted to arrival event
	int Size;				//order Size
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int AutoPromotionlimit;
	int VIP_PrmotionLimit;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int size, double money, int promtionlimit , int vipPromotionlimit);
	//Add more constructors if needed

	virtual void Execute(Restaurant *pRest);	//override execute function

};

#endif