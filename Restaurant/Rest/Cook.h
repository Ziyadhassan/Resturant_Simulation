#pragma once
#include"Order.h"
#include "..\Defs.h"

class Order;

class Cook
{
	int ID;
	ORD_TYPE type;        // For each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		      // Dishes it can prepare in one clock tick (in one timestep)
	bool InBreak;         // Returns True if the cook is in a break
	bool IsBusy;          // Returns True if the cook is busy ( Having an order )
	bool Injuered;        // Returns True if the cook is injured
	int Timer;		      // The timestep at which the cook will finish the order
	int Timer_Break;      // The timestep at which the cook will Return from the Break
	int Timer_Reset;	  // The timestep at which the cook will Return from the Reset if he is injured
	int Break_Duration;   // The Break Duration of the cook
	int Num_Of_Ord_Break; // The Number of orders the cook should prepare to take a break
	int Num_To_Break;	  // The Current prepered Order to have break
	Order* order;

public:
	Cook();
	Cook(int id, ORD_TYPE tp, int sp , int breakduration , int numtobreak); // constructor takes all the data member needed
	
	int GetID() const;
	ORD_TYPE GetType() const;
	int GetSpeed() const;
	bool InBREAK() const;
	bool IsBUSY() const;
	bool IsFree() const;     // Checks the "IsBusy()" and "InBreak()" together
	bool IsInjuered() const; 

	Order* GetOrder() const;
	int GetTimer();
	int GetTimerBreak();        // Returns "Timer_Break" of the Cook
	int GetTimerReset();        // Returns "Timer_Reset" 
	int Get_Break_Duration();   // Returns The Break Duration Ofthe Cook
	int Get_Num_Of_Ord_Break(); // Returns The Number of orders the cook should prepare to take a break
	int Get_Num_To_Break();     // Returns "Num_To_Break" of the Cook
	
	void setID(int);
	void setType(ORD_TYPE) ;
	void setSpeed(int);
	void setINBREAK(bool);
	void setISBUSY(bool);
	void setOrder(Order* ord);
	void setTimer(int time);
	void setTimerBreak(int time);
	void setTimerReset(int time);
	void setNumToBreak(int x);
	void setInjuered(bool x);


	int GetAvilabilityTime();


	virtual ~Cook();
};
