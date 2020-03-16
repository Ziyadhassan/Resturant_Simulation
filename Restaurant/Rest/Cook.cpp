#include "Cook.h"


Cook::Cook()
{
}

Cook::Cook(int id, ORD_TYPE tp, int sp, bool isbreak, bool isbusy, Order* ord)
{
	setID(id);
	setType(tp);
	setSpeed(sp);
	setINBREAK(isbreak);
	setISBUSY(isbusy);
	order = ord;
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

Order * Cook::GetOrder() const
{
	return order;
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


