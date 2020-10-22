#include "Cook.h"


Cook::Cook()
{
}

Cook::Cook(int id, ORD_TYPE tp, int sp,int breakduration, int numtobreak)
{
	setID(id);
	setType(tp);
	setSpeed(sp);
	setINBREAK(false);
	setISBUSY(false);
	setInjuered(false);
	order = nullptr;
	Timer = 0;
	Timer_Break = 0;
	Break_Duration = breakduration;
	Num_Of_Ord_Break = numtobreak;
	Num_To_Break = 0;

}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

int Cook::GetSpeed() const
{
	return speed;
}

bool Cook::InBREAK() const
{
	return InBreak;
}

bool Cook::IsBUSY() const
{
	return IsBusy;
}

bool Cook::IsFree() const
{
	return !( InBreak || IsBusy || Injuered);
}

bool Cook::IsInjuered() const
{
	return Injuered;
}

Order* Cook::GetOrder() const
{
	return order;
}

int Cook::GetTimer()
{
	return Timer;
}

int Cook::GetTimerBreak()
{
	return Timer_Break;
}

int Cook::GetTimerReset()
{
	return Timer_Reset;
}

int Cook::Get_Break_Duration()
{
	return Break_Duration;
}

int Cook::Get_Num_Of_Ord_Break()
{
	return Num_Of_Ord_Break;
}

int Cook::Get_Num_To_Break()
{
	return Num_To_Break;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int sp)
{
	speed = (sp > 0) ? sp : 1;
}

void Cook::setINBREAK(bool ib)
{
	InBreak = ib;
}

void Cook::setISBUSY(bool sb)
{
	IsBusy = sb;
}

void Cook::setOrder(Order * ord)
{
	order = ord;
}

void Cook::setTimer(int time)
{
	Timer = time;
}

void Cook::setTimerBreak(int time)
{
	Timer_Break = time;
}

void Cook::setTimerReset(int time)
{
	Timer_Reset = time;
}

void Cook::setNumToBreak(int x)
{
	Num_To_Break = x;
}

void Cook::setInjuered(bool x)
{
	Injuered = x;
}

int Cook::GetAvilabilityTime()
{
	if (Timer > Timer_Break && Timer > Timer_Reset)
		return Timer;
	else if (Timer_Break > Timer && Timer_Break > Timer_Reset)
		return Timer_Break;
	else
		return Timer_Reset;
}