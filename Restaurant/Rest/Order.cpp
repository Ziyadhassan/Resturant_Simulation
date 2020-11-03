#include "Order.h"

Order::Order()
{
}

Order::Order(int id, ORD_TYPE r_Type, double money, int size, int arrTime, int PromotionLimit, int VIP_PromotionLimit)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = ORD_STATUS_CNT;
	totalMoney = money;
	Size = size;
	ArrTime = arrTime;
	if (type == TYPE_NRM)
	{
		TimerAutoPromotion = ArrTime + PromotionLimit;
		TimerVipPromotion = -1;
	}
	else if (type == TYPE_VIP)
	{
		TimerAutoPromotion = -1;
		TimerVipPromotion = ArrTime + VIP_PromotionLimit;
	}
	else
	{
		TimerAutoPromotion = -1;
		Vip_AutoPromotion = -1;
	}
		
	Vip_AutoPromotion = false;
	AutoPromoted = false;

	if (type == TYPE_VIP)
	{
		Set_Priority_Value();
	}
	else
	{
		Priority_Value = 0;
	}
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


int Order::Get_Timer_AutoPrmotion()
{
	return TimerAutoPromotion;
}

int Order::Get_TImer_VipPromotion()
{
	return TimerVipPromotion;
}

bool Order::Get_AutoPromoted()
{
	return AutoPromoted;
}

void Order::setAutoPromotion(bool promotion)
{
	AutoPromoted = promotion;
}

void Order::setVipPromotion(bool promotion)
{
	Vip_AutoPromotion = promotion;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

void Order::Set_CookMaker(Cook* cook)
{
	CookMaker = cook;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

int Order::getSize()
{
	return Size;
}

