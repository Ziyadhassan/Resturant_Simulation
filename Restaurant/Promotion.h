#pragma once
#include "..\Restaurant\Events\Event.h"
#include"Rest/Restaurant.h"
class Promotion : public Event
{
	double ExtraMoney;
public:
	Promotion(int eTime, int oID, double money);
	virtual void Execute(Restaurant *pRest);
};

