#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "..\Generic_DS\Linked_List.h"
#include "..\Generic_DS\Priority_Queue.h"
#include <string>


#include "Order.h"

// it is the maestro of the project
class Restaurant
{
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	Linked_List<Cook> Normal_Cook;  // List of the Normal cooks
	Linked_List<Cook> Vegan_Cook;  // List of the Vegan cooks
	Linked_List<Cook> VIP_Cook;   // List of the VIP cooks

	PROG_MODE	mode;

	Linked_List<Order> Normal_Order;  // List of Normal orders in the current Timestep
	Linked_List<Order> Vegan_Order;   // List of Vegan orders in the current Timestep
	Linked_List<Order> VIP_Order;     // List of VIP orders in the current Timestep
	Linked_List<Order> Service_Order; // List of Orders in service in the current Timestep
	Linked_List<Order> order_done;	   		// List of Finished orders
	Priority_Queue<Order> Waiting_orders;   // List of Waiting orders for vip + normal
	Queue<Order> Waiting_orders_Vegan;   // List of Waiting orders for vegan

	float InjuryPropabilty;  // The Propabilty of a cook to be injuered
	int ResetDuration;	     // The Reset Period when a cook is injuered	 

public:

	Restaurant();
	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void FillDrawingList();

	// Modes 
	void Interactive_Mode();
	void StepByStep();
	void Silent();


	// The Main
	void Assgin_Cook_Order(int timestep, string&, string&, string&);
	bool Injuered(int timestep, float propabilty);
	void Free_Cooks(int timestep);
	void Auto_Promotion_Event(int timestep);
	void Draw_Status_Bar();
	void Draw_Assignation(string&, string&, string&);
	void OutputFile(string filename , int Injeredcooks);
	string Read_Input();
	bool Exit();


	// Assgin Order to a Cook
	bool Assgin_Normal_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& VCooks_Assgined);
	bool Assgin_Vegan_Order(Node<Order>* Ord, int timestep, string& GCooks_Assgined);
	bool Assgin_VIP_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& GCooks_Assgined, string& VCooks_Assgined);
	bool Assgin_Urgent_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& GCooks_Assgined, string& VCooks_Assgined);


	//Events Function
	void OrderArrival(Order* Ord);
	void OrderPromotion(int IDtoPromote, double ExtraMoney);
	void OrderCancellation(int IDtoCancell);
};

#endif