#pragma once
#include"Order.h"
#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	bool InBreak;   // Returns True if the cook is in a break
	bool IsBusy;    // Returns True if the cook is busy ( Having an order )
	Order* order;

public:
	Cook();
	Cook(int id, ORD_TYPE tp, int sp, bool isbreak = false, bool isbusy = false , Order* ord = nullptr); // constructor takes all the data member
	
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed() const;
	bool InBREAK() const;
	bool IsBUSY() const;
	Order* GetOrder() const;

	
	void setID(int);
	void setType(ORD_TYPE) ;
	void setSpeed(int);
	void setINBREAK(bool);
	void setISBUSY(bool);
	void setOrder(Order* ord);

	virtual ~Cook();
};
