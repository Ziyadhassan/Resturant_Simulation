#pragma once

#include "..\Rest\Cook.h"
#include "..\Defs.h"

class Cook;

class Order
{

protected:
	int ID;             //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status; 	//waiting, in-service, done
	int Size;		    //The Order size ( Number of dishs )
	double totalMoney;  //Total order money
	Cook* CookMaker;    //The cook who makes the cook
	int ArrTime, ServTime, FinishTime, WaitTime;	//arrival, service start, finish , and waiting times
	double Priority_Value;
	int TimerAutoPromotion;
	int TimerVipPromotion;
	bool Vip_AutoPromotion;
	bool AutoPromoted;

public:
	Order();
	Order(int ID, ORD_TYPE r_Type, double money, int size, int arrTime, int PromotionLimit, int VIP_PromotionLimit);
	virtual ~Order();

	int GetID();
	ORD_STATUS getStatus() const;
	int getSize();
	Cook* Get_Cook() { return CookMaker; }
	double Get_Total_Money() { return totalMoney; }
	ORD_TYPE GetType() const;
	double Get_Priority_Value()
	{
		return Priority_Value;
	}
	int Get_Arrival_Time() { return ArrTime; }
	int Get_Service_Time() { return ServTime; }
	int Get_Waiting_Time() { return WaitTime; }
	int Get_Finish_Time() { return FinishTime; }
	int Get_Timer_AutoPrmotion();
	int Get_TImer_VipPromotion();
	bool Get_AutoPromoted();

	void setAutoPromotion(bool promotion);
	void setVipPromotion(bool promotion);
	void setStatus(ORD_STATUS s);
	void Set_Type(ORD_TYPE s)
	{
		type = s;
		if (type == TYPE_VIP || type == TYPE_URG)
		{
			Set_Priority_Value();
		}
	}
	void Set_CookMaker(Cook* cook);
	void Set_Money(double money) { totalMoney = (money >= 0) ? money : totalMoney; }
	void Set_Priority_Value()
	{
		double t, m, s;
		t = ArrTime;
		m = totalMoney;
		s = Size;
		if (type == TYPE_VIP)
			Priority_Value = 100000000 - 8 * t + m * 7 - s * 4.7;
		else if (type == TYPE_URG)
			Priority_Value = 1.79769e+308; // MAX number can be assign to double
	}
	void Set_Service_Time(int time) { ServTime = time; }
	void Set_Waiting_Time(int time) { WaitTime = time; }
	void Set_Finish_Time(int time) { FinishTime = time; }


};