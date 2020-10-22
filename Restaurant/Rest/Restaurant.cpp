#include <cstdlib>
#include <time.h>
#include<fstream>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include"..\CancellationEvent.h"
#include"..\Promotion.h"


Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Interactive_Mode();
		break;
	case MODE_STEP:
		StepByStep();
		break;
	case MODE_SLNT:
		Silent();
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	pGUI->AddToDrawingList(Normal_Cook, Vegan_Cook, VIP_Cook);
	pGUI->AddToDrawingList(Service_Order);
	pGUI->AddToDrawingList(Waiting_orders, Waiting_orders_Vegan);
	pGUI->AddToDrawingList(order_done);
}

void Restaurant::Interactive_Mode()
{
	string Filename = Read_Input();
	int CurrentTimeStep = 1;
	string Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined;
	int InjerentCooks = 0;

	//as long as events queue is not empty yet
	while (!Exit())
	{
		//print current timestep
		string timestep = "TimeStep : ";
		timestep += to_string(CurrentTimeStep);
		pGUI->PrintMessage(timestep);
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		Free_Cooks(CurrentTimeStep);   // Free all the cooks at the Current 

		Draw_Assignation(Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		Assgin_Cook_Order(CurrentTimeStep, Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		InjerentCooks += Injuered(CurrentTimeStep, rand() / float(RAND_MAX));

		Draw_Status_Bar();

		Auto_Promotion_Event(CurrentTimeStep);

		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}
	OutputFile(Filename, InjerentCooks);
	pGUI->PrintMessage("Simulation has Finish, Output File Has been Out ^-^ ....");
	pGUI->waitForClick();
}

void Restaurant::StepByStep()
{
	string Filename = Read_Input();
	int CurrentTimeStep = 1;
	string Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined;
	int InjerentCooks = 0;

	//as long as events queue is not empty yet
	while (!Exit())
	{
		//print current timestep
		string timestep = "TimeStep : ";
		timestep += to_string(CurrentTimeStep);
		pGUI->PrintMessage(timestep);
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		Free_Cooks(CurrentTimeStep);   // Free all the cooks at the Current 

		Draw_Assignation(Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		Assgin_Cook_Order(CurrentTimeStep, Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		InjerentCooks += Injuered(CurrentTimeStep, rand() / float(RAND_MAX));

		Draw_Status_Bar();

		Auto_Promotion_Event(CurrentTimeStep);

		FillDrawingList();
		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}
	OutputFile(Filename, InjerentCooks);
	pGUI->PrintMessage("Simulation has Finish, Output File Has been Out ^-^ ....");
	pGUI->waitForClick();
}

void Restaurant::Silent()
{
	string Filename = Read_Input();

	int CurrentTimeStep = 1;
	string Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined;
	int InjerentCooks = 0;

	//as long as events queue  is not empty yet
	while (!Exit())
	{

		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step

		Free_Cooks(CurrentTimeStep);   // Free all the cooks at the Current 

		//Draw_Status_Bar(Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		Assgin_Cook_Order(CurrentTimeStep, Normal_Cooks_Assgined, Vegan_Cooks_Assgined, VIP_Cooks_Assgined);

		Auto_Promotion_Event(CurrentTimeStep);

		CurrentTimeStep++;	//advance timestep
	}
	OutputFile(Filename, InjerentCooks);
	pGUI->PrintMessage("Output File Has been Out ^-^ ....");
	pGUI->waitForClick();
}

// To assgin a order to a cook
void Restaurant::Assgin_Cook_Order(int timestep, string& NC, string& GC, string& VC)
{
	Node<Order>* ord;
	Node<Order>* temp;
	bool VIP = true;
	bool Vegan = true;
	bool Normal = true;

	NC = GC = VC = "";

	ord = Waiting_orders.Get_Head();

	while (ord && (VIP && Normal))
	{
		temp = ord->getNext();
		switch (ord->getItem().GetType())
		{
		case TYPE_NRM:
			Normal = Assgin_Normal_Order(ord, timestep, NC, VC);
			break;
		case TYPE_VIP:
			VIP = Assgin_VIP_Order(ord, timestep, NC, GC, VC);
			break;
		case TYPE_URG:
			VIP = Assgin_Urgent_Order(ord, timestep, NC, GC, VC);
		default:
			break;
		}
		if (VIP && Normal)
			ord = temp;
	}

	ord = Waiting_orders_Vegan.GetHead();
	while (ord && Vegan)
	{
		temp = ord->getNext();
		Vegan = Assgin_Vegan_Order(ord, timestep, GC);
		if (Vegan)
			ord = temp;
	}

	if (Vegan)
	{
		ord = Vegan_Order.GetHead();
		while (ord && Vegan)
		{
			temp = ord->getNext();
			Vegan = Assgin_Vegan_Order(ord, timestep, GC);
			if (Vegan)
				ord = temp;
		}
	}
	if (!Vegan)
	{
		// Add the rest of the of the orders to the waiting list
		ord = Vegan_Order.GetHead();
		while (ord)
		{
			temp = ord->getNext();	// as it may be removed from the list
			if (ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				ord->getItem().setStatus(WAIT);
				ord->getItem().Set_Waiting_Time(timestep);
				ord->getItem().Set_CookMaker(NULL); // Theres is not cook
				//Remove it from Normal list and add it to waiting list
				Waiting_orders_Vegan.enqueue(Vegan_Order.Remove(&ord->getItem()));
			}

			ord = temp;
		}
	}

	if (VIP)
	{
		ord = VIP_Order.GetHead();
		while (ord && VIP)
		{
			temp = ord->getNext();
			VIP = Assgin_VIP_Order(ord, timestep, NC, GC, VC);
			if (VIP)
				ord = temp;
		}

		if (VIP && Normal)
		{
			ord = Normal_Order.GetHead();
			while (ord && Normal)
			{
				temp = ord->getNext();
				Normal = Assgin_Normal_Order(ord, timestep, NC, VC);
				if (Normal)
					ord = temp;
			}
		}
	}

	// Add the rest of the of the orders to the waiting list
	if (!VIP)
	{
		ord = Normal_Order.GetHead();
		while (ord)
		{
			temp = ord->getNext(); // as it may be removed from the list
			if (ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				ord->getItem().setStatus(WAIT);
				ord->getItem().Set_Waiting_Time(timestep);
				ord->getItem().Set_CookMaker(NULL); // Theres is not cook
				Waiting_orders.push(Normal_Order.Remove(&ord->getItem())); //Remove it from Normal list and add it to waiting list
			}
			ord = temp;
		}


		ord = VIP_Order.GetHead();
		// Add the rest of the of the orders to the waiting list
		while (ord)
		{
			temp = ord->getNext(); // as it may be removed from the list
			if (ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				ord->getItem().setStatus(WAIT);
				ord->getItem().Set_Waiting_Time(timestep);
				ord->getItem().Set_CookMaker(NULL); // Theres is not cook
				Waiting_orders.push(VIP_Order.Remove(&ord->getItem())); //Remove it from VIP list and add it to waiting list
			}
			ord = temp;
		}


	}
	else if (!Normal)
	{
		ord = Normal_Order.GetHead();

		// Add the rest of the of the orders to the waiting list
		while (ord)
		{
			temp = ord->getNext(); // as it may be removed from the list
			if (ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				ord->getItem().setStatus(WAIT);
				ord->getItem().Set_Waiting_Time(timestep);
				ord->getItem().Set_CookMaker(NULL); // Theres is not cook
				Waiting_orders.push(Normal_Order.Remove(&ord->getItem())); //Remove it from Normal list and add it to waiting list
			}
			ord = temp;
		}
	}
}

bool Restaurant::Injuered(int timestep, float propabilty)
{
	if (propabilty <= InjuryPropabilty)
	{
		if (Service_Order.GetHead())
		{
			Node<Order>* node = Service_Order.GetHead();
			node->getItem().Get_Cook()->setInjuered(true);
			node->getItem().Get_Cook()->setTimer(timestep + (node->getItem().Get_Cook()->GetTimer() - timestep) * 2);
			node->getItem().Set_Service_Time(node->getItem().Get_Cook()->GetTimer() - node->getItem().Get_Arrival_Time() - node->getItem().Get_Waiting_Time());
			node->getItem().Get_Cook()->setTimerReset(node->getItem().Get_Cook()->GetTimer() + ResetDuration);
			return true;
		}
	}
	return false;
}

// To free all the cook the should be freed by the current time step 
void Restaurant::Free_Cooks(int timestep)
{
	Node<Cook>* cook;

	cook = Normal_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().GetTimer() == timestep)  // if the cook should finish in the Current timestep
		{
			//cook->getItem().setTimer(0);
			cook->getItem().GetOrder()->setStatus(DONE); // Change the status of the order to "DONE"
			// Revmove the order from the serving list add the order to the "Order_done" List
			Node<Order>* ord = Service_Order.Remove(cook->getItem().GetOrder());
			ord->getItem().Set_Finish_Time(ord->getItem().Get_Arrival_Time() + ord->getItem().Get_Service_Time() + ord->getItem().Get_Waiting_Time()); // Set Finish Time

			order_done.InsertEnd(ord);
			cook->getItem().setISBUSY(false);  // Make cook Free
			cook->getItem().setOrder(NULL);

			if (cook->getItem().Get_Num_To_Break() == cook->getItem().Get_Num_Of_Ord_Break())  // The Cook Should Take Break
			{
				cook->getItem().setINBREAK(true);
				cook->getItem().setNumToBreak(0);
				cook->getItem().setTimerBreak(timestep + cook->getItem().Get_Break_Duration());
			}

			if (ord->getItem().GetType() == TYPE_URG)
			{
				if (cook->getItem().GetTimerBreak() > timestep)
					cook->getItem().setINBREAK(true);
				if (cook->getItem().GetTimerReset() > timestep)
					cook->getItem().setInjuered(true);
			}

		}


		if (cook->getItem().GetTimerBreak() == timestep) // The Cook Should Return From The Break
		{
			cook->getItem().setINBREAK(false);
			cook->getItem().setNumToBreak(0);
			//cook->getItem().setTimerBreak(0);
		}

		if (cook->getItem().GetTimerReset() == timestep)
			cook->getItem().setInjuered(false);

		cook = cook->getNext();
	}

	cook = Vegan_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().GetTimer() == timestep)  // if the cook should finish in the Current timestep
		{
			//cook->getItem().setTimer(0);
			cook->getItem().GetOrder()->setStatus(DONE); // Change the status of the order to "DONE"

			// Revmove the order from the serving list and  add the order to the "Order_done" List
			Node<Order>* ord = Service_Order.Remove(cook->getItem().GetOrder());
			ord->getItem().Set_Finish_Time(ord->getItem().Get_Arrival_Time() + ord->getItem().Get_Service_Time() + ord->getItem().Get_Waiting_Time()); // Set Finish Time

			order_done.InsertEnd(ord);
			cook->getItem().setISBUSY(false);  // Make cook Free
			cook->getItem().setOrder(NULL);

			if (cook->getItem().Get_Num_To_Break() == cook->getItem().Get_Num_Of_Ord_Break())
			{
				cook->getItem().setINBREAK(true);
				cook->getItem().setNumToBreak(0);
				cook->getItem().setTimerBreak(timestep + cook->getItem().Get_Break_Duration());
			}

			if (ord->getItem().GetType() == TYPE_URG)
			{
				if (cook->getItem().GetTimerBreak() > timestep)
					cook->getItem().setINBREAK(true);
				if (cook->getItem().GetTimerReset() > timestep)
					cook->getItem().setInjuered(true);
			}
		}

		if (cook->getItem().GetTimerBreak() == timestep) // The Cook Should Return From The Break
		{
			cook->getItem().setINBREAK(false);
			cook->getItem().setNumToBreak(0);
			//cook->getItem().setTimerBreak(0);
		}

		if (cook->getItem().GetTimerReset() == timestep)
			cook->getItem().setInjuered(false);

		cook = cook->getNext();
	}

	cook = VIP_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().GetTimer() == timestep)  // if the cook should finish in the Current timestep
		{
			//cook->getItem().setTimer(0);
			cook->getItem().GetOrder()->setStatus(DONE); // Change the status of the order to "DONE"
			// Revmove the order from the serving list add the order to the "Order_done" List
			Node<Order>* ord = Service_Order.Remove(cook->getItem().GetOrder());
			ord->getItem().Set_Finish_Time(ord->getItem().Get_Arrival_Time() + ord->getItem().Get_Service_Time() + ord->getItem().Get_Waiting_Time()); // Set Finish Time

			order_done.InsertEnd(ord);
			cook->getItem().setISBUSY(false);  // Make cook Free
			cook->getItem().setOrder(NULL);

			if (cook->getItem().Get_Num_To_Break() == cook->getItem().Get_Num_Of_Ord_Break())
			{
				cook->getItem().setINBREAK(true);
				cook->getItem().setNumToBreak(0);
				cook->getItem().setTimerBreak(timestep + cook->getItem().Get_Break_Duration());
			}

			if (ord->getItem().GetType() == TYPE_URG)
			{
				if (cook->getItem().GetTimerBreak() > timestep)
					cook->getItem().setINBREAK(true);
				if (cook->getItem().GetTimerReset() > timestep)
					cook->getItem().setInjuered(true);
			}
		}

		if (cook->getItem().GetTimerBreak() == timestep) // The Cook Should Return From The Break
		{
			cook->getItem().setINBREAK(false);
			cook->getItem().setNumToBreak(0);
			//cook->getItem().setTimerBreak(0);
		}

		if (cook->getItem().GetTimerReset() == timestep)
			cook->getItem().setInjuered(false);

		cook = cook->getNext();
	}
}

bool Restaurant::Assgin_Normal_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& VCooks_Assgined)
{
	Node<Cook>* cook = Normal_Cook.GetHead();
	// Search For Normal Cook
	while (cook)
	{

		if (cook->getItem().IsFree())	// If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem());      // Assgin the order to the cook
			cook->getItem().setISBUSY(true);                // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders.PoP());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(Normal_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			NCooks_Assgined += " N" + to_string(cook->getItem().GetID()) + "(N" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}

	cook = VIP_Cook.GetHead();
	// Search For VIP Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders.PoP());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(Normal_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			VCooks_Assgined += " V" + to_string(cook->getItem().GetID()) + "(N" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}

	return false;
}

bool Restaurant::Assgin_Vegan_Order(Node<Order>* Ord, int timestep, string& GCooks_Assgined)
{
	Node<Cook>* cook = Vegan_Cook.GetHead();
	// Search For Vegan Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders_Vegan.dequeue());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(Vegan_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			GCooks_Assgined += " G" + to_string(cook->getItem().GetID()) + "(G" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}
	return false;
}

bool Restaurant::Assgin_VIP_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& GCooks_Assgined, string& VCooks_Assgined)
{
	Node<Cook>* cook = VIP_Cook.GetHead();
	// Search For VIP Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders.PoP());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(VIP_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			VCooks_Assgined += " V" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}

	cook = Normal_Cook.GetHead();
	// Search For Normal Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders.PoP());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(VIP_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			NCooks_Assgined += " N" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}


	cook = Vegan_Cook.GetHead();
	// Search For Vegan Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			if (Ord->getItem().getStatus() == WAIT)
			{
				//Remove it from Waiting list and Add it to the serving list
				Service_Order.InsertEnd(Waiting_orders.PoP());
				// The Current Time Step minus the timeStep were it was add to the waiting list
				Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());
			}
			else if (Ord->getItem().getStatus() == ORD_STATUS_CNT)
			{
				//Remove it from Normal list and Add it to the serving list
				Service_Order.InsertEnd(VIP_Order.Remove(&Ord->getItem()));
				Ord->getItem().Set_Waiting_Time(0);        // Didn't wait
			}

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			GCooks_Assgined += " G" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}

		cook = cook->getNext();
	}

	return false;
}

bool Restaurant::Assgin_Urgent_Order(Node<Order>* Ord, int timestep, string& NCooks_Assgined, string& GCooks_Assgined, string& VCooks_Assgined)
{
	Node<Cook>* cook = VIP_Cook.GetHead();
	Node<Cook>* cookBreak = NULL;
	Node<Cook>* cookReset = NULL;
	// Search For VIP Cook
	while (cook)
	{
		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			//Remove it from Waiting list and Add it to the serving list
			Service_Order.InsertEnd(Waiting_orders.PoP());
			// The Current Time Step minus the timeStep were it was add to the waiting list
			Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			VCooks_Assgined += " V" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}
		else if (cook->getItem().InBREAK() && !cook->getItem().IsInjuered() && cookBreak != NULL)
		{
			cookBreak = cook;
		}
		else if (!cook->getItem().IsBUSY() && cook->getItem().IsInjuered() && cookReset != NULL)
		{
			cookReset = cook;
		}
		cook = cook->getNext();
	}

	cook = Normal_Cook.GetHead();
	// Search For Normal Cook
	while (cook)
	{
		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			//Remove it from Waiting list and Add it to the serving list
			Service_Order.InsertEnd(Waiting_orders.PoP());
			// The Current Time Step minus the timeStep were it was add to the waiting list
			Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			NCooks_Assgined += " N" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}
		else if (cook->getItem().InBREAK() && !cook->getItem().IsInjuered() && cookBreak != NULL)
		{
			cookBreak = cook;
		}
		else if (!cook->getItem().IsBUSY() && cook->getItem().IsInjuered() && cookReset != NULL)
		{
			cookReset = cook;
		}

		cook = cook->getNext();
	}

	cook = Vegan_Cook.GetHead();
	// Search For Vegan Cook
	while (cook)
	{

		if (cook->getItem().IsFree()) // If cook is free
		{
			Ord->getItem().Set_CookMaker(&cook->getItem()); // Assgin the cook to the order
			cook->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
			cook->getItem().setISBUSY(true);           // Make cook Busy

			int x = ceil((Ord->getItem().getSize() / cook->getItem().GetSpeed()));
			x = (x > 0) ? x : 1;
			cook->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	

			// Increse The ammount of prepared orders of the cook by 1
			cook->getItem().setNumToBreak(cook->getItem().Get_Num_To_Break() + 1);

			//Remove it from Waiting list and Add it to the serving list
			Service_Order.InsertEnd(Waiting_orders.PoP());
			// The Current Time Step minus the timeStep were it was add to the waiting list
			Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());

			Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
			Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order
			GCooks_Assgined += " G" + to_string(cook->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
			return true;
		}
		else if (cook->getItem().InBREAK() && !cook->getItem().IsInjuered() && cookBreak != NULL)
		{
			cookBreak = cook;
		}
		else if (!cook->getItem().IsBUSY() && cook->getItem().IsInjuered() && cookReset != NULL)
		{
			cookReset = cook;
		}
		cook = cook->getNext();
	}

	if (cookBreak)
	{
		Ord->getItem().Set_CookMaker(&cookBreak->getItem()); // Assgin the cook to the order
		cookBreak->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
		cookBreak->getItem().setISBUSY(true);           // Make cook Busy
		cookBreak->getItem().setINBREAK(false);

		int x = ceil((Ord->getItem().getSize() / cookBreak->getItem().GetSpeed()));
		x = (x > 0) ? x : 1;
		cookBreak->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish
		cookBreak->getItem().setTimerBreak(cookBreak->getItem().GetTimerBreak() + x); // To make the cook complete the break after he finish 

		// Increse The ammount of prepared orders of the cook by 1
		cookBreak->getItem().setNumToBreak(cookBreak->getItem().Get_Num_To_Break() + 1);

		//Remove it from Waiting list and Add it to the serving list
		Service_Order.InsertEnd(Waiting_orders.PoP());
		// The Current Time Step minus the timeStep were it was add to the waiting list
		Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());

		Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
		Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order

		if (cookBreak->getItem().GetType() == TYPE_VIP)
			VCooks_Assgined += " V" + to_string(cookBreak->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		else if (cookBreak->getItem().GetType() == TYPE_NRM)
			NCooks_Assgined += " N" + to_string(cookBreak->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		else
			GCooks_Assgined += " G" + to_string(cookBreak->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		return true;
	}
	else if (cookReset)
	{
		Ord->getItem().Set_CookMaker(&cookReset->getItem()); // Assgin the cook to the order
		cookReset->getItem().setOrder(&Ord->getItem()); // Assgin the order to the cook
		cookReset->getItem().setISBUSY(true);           // Make cook Busy
		cookReset->getItem().setINBREAK(false);

		int x = ceil((Ord->getItem().getSize() / (float(cookReset->getItem().GetSpeed()) / 2)));
		cookReset->getItem().setTimer(timestep + x); // Set the timestep at which the cook should finish	
		cookReset->getItem().setTimerReset(cookReset->getItem().GetTimerReset() + x); // to make him complete his rest period after he finishs the order

		// Increse The ammount of prepared orders of the cook by 1
		cookReset->getItem().setNumToBreak(cookReset->getItem().Get_Num_To_Break() + 1);

		//Remove it from Waiting list and Add it to the serving list
		Service_Order.InsertEnd(Waiting_orders.PoP());
		// The Current Time Step minus the timeStep were it was add to the waiting list
		Ord->getItem().Set_Waiting_Time(timestep - Ord->getItem().Get_Waiting_Time());

		Ord->getItem().setStatus(SRV);             // Change the status of the order to "In-Servce"
		Ord->getItem().Set_Service_Time(x);		   // Assgin the service time to the order

		if (cookReset->getItem().GetType() == TYPE_VIP)
			VCooks_Assgined += " V" + to_string(cookReset->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		else if (cookReset->getItem().GetType() == TYPE_NRM)
			NCooks_Assgined += " N" + to_string(cookReset->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		else
			GCooks_Assgined += " G" + to_string(cookReset->getItem().GetID()) + "(V" + to_string(Ord->getItem().GetID()) + ")  ";
		return true;
	}
	return false;
}

void Restaurant::OrderArrival(Order* Ord)
{
	switch (Ord->GetType())
	{
	case TYPE_NRM:
		Normal_Order.InsertEnd(Ord);
		break;
	case TYPE_VGAN:
		Vegan_Order.InsertEnd(Ord);
		break;
	case TYPE_VIP:
		VIP_Order.InsertEnd(Ord);
		break;
	default:
		break;
	}
}

void Restaurant::OrderPromotion(int IDtoPromote, double ExtraMoney)
{
	bool indicator = false;
	if (Waiting_orders.Get_Head()) // Search in Waiting List
	{

		Node<Order>* ptr = Waiting_orders.Get_Head();
		while (ptr)
		{
			if (ptr->getItem().GetID() == IDtoPromote && ptr->getItem().GetType() == TYPE_NRM)
			{
				ptr->getItem().Set_Type(TYPE_VIP);
				ptr->getItem().Set_Money(ptr->getItem().Get_Total_Money() + ExtraMoney);
				indicator = true;
			}
			ptr = ptr->getNext();
		}
	}

	if (indicator) // you have to resort the Waiting list after an order gets a promotion
	{
		Priority_Queue<Order>* temp = new Priority_Queue<Order>;
		Node<Order>* ptr = Waiting_orders.PoP();
		int count = 0;
		while (ptr)
		{
			temp->push(ptr);
			count++;
			ptr = Waiting_orders.PoP();;
		}

		Waiting_orders.SetHead(temp->Get_Head());
		Waiting_orders.SetRear(temp->Get_Rear());
		Waiting_orders.SetCount(temp->GetCount());
		return;
	}


	if (Service_Order.GetHead())  // Search in Service List
	{
		Node<Order>* ptr = Service_Order.GetHead();
		while (ptr)
		{
			if (ptr->getItem().GetID() == IDtoPromote && ptr->getItem().GetType() == TYPE_NRM)
			{
				ptr->getItem().Set_Type(TYPE_VIP);
				ptr->getItem().Set_Money(ptr->getItem().Get_Total_Money() + ExtraMoney);
				return;
			}
			ptr = ptr->getNext();
		}
	}
}

void Restaurant::OrderCancellation(int IDtoCancell)
{
	Node<Order>* ptr = Waiting_orders.PoP();
	Priority_Queue<Order>* New_Waiting_List = new Priority_Queue<Order>;
	while (ptr)
	{
		if (ptr->getItem().GetID() == IDtoCancell)
		{
			ptr = Waiting_orders.PoP();
			continue;
		}
		else
		{
			New_Waiting_List->push(ptr);
			ptr = Waiting_orders.PoP();
		}
	}
	Waiting_orders.SetHead(New_Waiting_List->Get_Head());
	Waiting_orders.SetRear(New_Waiting_List->Get_Rear());
	Waiting_orders.SetCount(New_Waiting_List->GetCount());
}

void Restaurant::Auto_Promotion_Event(int timestep)
{
	int count = 0;
	Node<Order>* ord = Waiting_orders.Get_Head();
	while (ord)
	{
		if (ord->getItem().Get_Timer_AutoPrmotion() == timestep)
		{
			ord->getItem().Set_Type(TYPE_VIP);
			ord->getItem().setAutoPromotion(true);
			count++;
		}
		if (ord->getItem().Get_TImer_VipPromotion() == timestep)
		{
			ord->getItem().Set_Type(TYPE_URG);
			ord->getItem().setVipPromotion(true);
		}
		ord = ord->getNext();
	}

	if (count) // you have to resort the Waiting list after an order gets a promotion
	{
		Priority_Queue<Order>* temp = new Priority_Queue<Order>;
		Node<Order>* ptr = Waiting_orders.PoP();
		while (ptr)
		{
			temp->push(ptr);
			ptr = Waiting_orders.PoP();;
		}
		Waiting_orders.SetHead(temp->Get_Head());
		Waiting_orders.SetRear(temp->Get_Rear());
		Waiting_orders.SetCount(temp->GetCount());
	}
	return;
}

void Restaurant::Draw_Status_Bar()
{
	int Vegan_orders, Normal_orders, VIP_Orders, Normal_cooks, Vegan_cooks, VIP_cooks, Served_Now;
	Vegan_orders = Normal_orders = VIP_Orders = Normal_cooks = Vegan_cooks = VIP_cooks = Served_Now = 0;
	string n, g, v, w;
	Node<Order>* ord;
	Node<Cook>* cook;
	//waiting of types vip & normal
	ord = Waiting_orders.Get_Head();
	while (ord)
	{
		if (ord->getItem().GetType() == TYPE_NRM)
			Normal_orders++;
		else
			VIP_Orders++;
		ord = ord->getNext();
	}
	//waiting of type vegan
	ord = Waiting_orders_Vegan.GetHead();
	while (ord)
	{
		Vegan_orders++;
		ord = ord->getNext();
	}
	//served so far
	Served_Now = order_done.GetCount();
	//free normal
	cook = Normal_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().IsFree())
			Normal_cooks++;
		cook = cook->getNext();
	}
	//free vegan
	cook = Vegan_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().IsFree())
			Vegan_cooks++;
		cook = cook->getNext();
	}
	//free vip
	cook = VIP_Cook.GetHead();
	while (cook)
	{
		if (cook->getItem().IsFree())
			VIP_cooks++;
		cook = cook->getNext();
	}
	//filling basis info
	n = "Waiting Normal Orders  : " + to_string(Normal_orders) + "        Free Normal Cooks  : " + to_string(Normal_cooks) + "       Orders Assigned Past TS :";
	g = "Waiting  Vegan  Orders : " + to_string(Vegan_orders) + "        Free  Vegan   Cooks : " + to_string(Vegan_cooks) + "       Orders Assigned Past TS :";
	v = "Waiting    VIP     Orders  : " + to_string(VIP_Orders) + "        Free    VIP     Cooks  : " + to_string(VIP_cooks) + "       Orders Assigned Past TS :";
	switch (mode)
	{
	case MODE_INTR:
		w = "INTERACTIVE MODE                                                  ";
		break;
	case MODE_STEP:
		w = "STEP-BY-STEP                                                             ";
		break;
	case MODE_SLNT:
		w = "SILENT MODE                                                       "; //I KNOW ITS REDUNDANT
		break;
	};
	w += "                                     Orders Served Till Now : " + to_string(Served_Now);
	//printing
	pGUI->PrintMessageN(n, g, v, w);
	pGUI->PrintMessageNoRemove("                                                                                                                                                                                                            WELCOME TO OUR RESTAURANT");

}

void Restaurant::Draw_Assignation(string& n, string& g, string& v)
{
	string normal = "                                                                                                                                                    " + n;
	string vegan = "                                                                                                                                                    " + g;
	string vip = "                                                                                                                                                    " + v;
	pGUI->PrintMessageN(normal, vegan, vip, "");
}

void Restaurant::OutputFile(string filename, int Injuerdcooks)
{
	ofstream out("OUTPUT_" + filename);

	int Total_Normal_Orders = 0, Total_Vegan_Orders = 0, Total_VIP_Orders = 0, Total_Urgent_Orders = 0;
	int Total_Orders = order_done.GetCount(), Total_Auto_Promotion = 0;
	int Total_Waiting_Time = 0, Total_Service_Time = 0;
	double Avg_Waiting_Time = 0, Avg_Service_Time = 0, Total_Earning = 0;
	Node<Order>* ord = order_done.GetHead();
	out << "FT\tID\tAt\tWt\tST" << endl;

	while (ord) // For Orders
	{
		out << ord->getItem().Get_Finish_Time() << "\t" << ord->getItem().GetID() << "\t" << ord->getItem().Get_Arrival_Time() << "\t";
		out << ord->getItem().Get_Waiting_Time() << "\t" << ord->getItem().Get_Service_Time() << endl;

		Total_Waiting_Time += ord->getItem().Get_Waiting_Time();


		if (ord->getItem().Get_AutoPromoted() == 1)
			Total_Auto_Promotion++;

		if (ord->getItem().GetType() == TYPE_URG)
			Total_Urgent_Orders++;

		if (ord->getItem().GetType() == TYPE_NRM)
			Total_Normal_Orders++;

		else if (ord->getItem().GetType() == TYPE_VGAN)
			Total_Vegan_Orders++;
		else
			Total_VIP_Orders++;

		Total_Service_Time += ord->getItem().Get_Service_Time();
		Total_Earning += ord->getItem().Get_Total_Money();
		ord = ord->getNext();
	}
	if (Total_Orders)
	{
		Avg_Waiting_Time = Total_Waiting_Time / float(Total_Orders);
		Avg_Service_Time = Total_Service_Time / float(Total_Orders);
	}

	int Total_Normal_Cooks = Normal_Cook.GetCount();
	int Total_Vegan_Cooks = Vegan_Cook.GetCount();   // For Cooks
	int Total_VIP_Cooks = VIP_Cook.GetCount();
	int Total_Cooks = Total_Normal_Cooks + Total_Vegan_Cooks + Total_VIP_Cooks;


	out << "Orders: " << Total_Orders << " [ Norm:" << Total_Normal_Orders << ", Veg:" << Total_Vegan_Orders << ", VIP:" << Total_VIP_Orders << " ]" << endl;
	out << "Cooks:" << Total_Cooks << " [ Norm:" << Total_Normal_Cooks << ", Veg:" << Total_Vegan_Cooks << ", VIP:" << Total_VIP_Cooks << ", injured: " << Injuerdcooks << " ]" << endl;
	out << "Avg Wait = " << Avg_Waiting_Time << " , Avg Serv = " << Avg_Service_Time << endl;
	out << "Urgent orders: " << Total_Urgent_Orders << " , Auto-promoted: " << Total_Auto_Promotion / float(Total_Normal_Orders) * 100 << "%" << endl;
	out << "Total-Earnings : " << Total_Earning << " L.E.\n";
	out.close();
}

string Restaurant::Read_Input()
{
	ifstream Input;

	pGUI->PrintMessage("Enter File Name : ");
	string Filename = pGUI->GetString();
	int length = Filename.length();

	if (Filename[length - 1] == 't' && Filename[length - 2] == 'x' && Filename[length - 3] == 't' && Filename[length - 4] == '.')
		Filename = Filename;
	else
		Filename = Filename + ".txt";
	Input.open(Filename);

	while (!Input.is_open()) // If the file is not open succesfully
	{
		pGUI->PrintMessage("Couldn't Open the File (Invalid Name) , Please try again : ");
		Filename = pGUI->GetString();
		if (Filename[length - 1] == 't' && Filename[length - 2] == 'x' && Filename[length - 3] == 't' && Filename[length - 4] == '.')
			Filename = Filename;
		else
			Filename = Filename + ".txt";
		Input.open(Filename);
	}

	int Num_Normal_Cooks; // Number of Normal Cooks
	int Num_Vegan_Cooks;  // Numbers of Vegan Cooks
	int Num_VIP_Cooks;    // Numbers of VIP Cooks


	int Min_Normal_Speed;   // The Min Speed of Normal Cook
	int Max_Normal_Speed;   // The Max Speed of Normal Cook

	int Min_Vegan_Speed;    // The Min Speed of Vegan Cook
	int Max_Vegan_Speed;    // The Max Speed of Vegan Cook

	int Min_VIP_Speed;		// The Min Speed of VIP Cook
	int Max_VIP_Speed;		// The Max Speed of VIP Cook


	int NUM_OF_ORDER_TO_BREAK;  // The Number of order should the cook prepare to have break

	int Min_BreakNormal;    // The Min Break Duration Of Normal Cook
	int Max_BreakNormal;    // The Max Break Duration Of Normal Cook

	int Min_BreakVegan;     // The Min Break Duration Of Vegan Cook
	int Max_BreakVegan;     // The Max Break Duration Of Vegan Cook

	int Min_BreakVIP;       // The Min Break Duration Of VIP Cook
	int Max_BreakVIP;       // The Max Break Duration Of VIP Cook       

	int PromotionLimit;
	int VIP_PromotionLimit;
	int NumOfEvents;

	Input >> Num_Normal_Cooks >> Num_Vegan_Cooks >> Num_VIP_Cooks;
	Input >> Min_Normal_Speed >> Max_Normal_Speed;
	Input >> Min_Vegan_Speed >> Max_Vegan_Speed;
	Input >> Min_VIP_Speed >> Max_VIP_Speed;
	Input >> NUM_OF_ORDER_TO_BREAK;
	Input >> Min_BreakNormal >> Max_BreakNormal;
	Input >> Min_BreakVegan >> Max_BreakVegan;
	Input >> Min_BreakVIP >> Max_BreakVIP;
	Input >> InjuryPropabilty >> ResetDuration;
	Input >> PromotionLimit >> VIP_PromotionLimit;
	Input >> NumOfEvents;

	Normal_Cook.Setter_Linked(Num_Normal_Cooks, Cook(1, TYPE_NRM, 0, 0, NUM_OF_ORDER_TO_BREAK), Min_Normal_Speed, Max_Normal_Speed, Min_BreakNormal, Max_BreakNormal);
	Vegan_Cook.Setter_Linked(Num_Vegan_Cooks, Cook(1 + Num_Normal_Cooks, TYPE_VGAN, 0, 0, NUM_OF_ORDER_TO_BREAK), Min_Vegan_Speed, Max_Vegan_Speed, Min_BreakVegan, Max_BreakVegan);
	VIP_Cook.Setter_Linked(Num_VIP_Cooks, Cook(1 + Num_Normal_Cooks + Num_Vegan_Cooks, TYPE_VIP, 0, 0, NUM_OF_ORDER_TO_BREAK), Min_VIP_Speed, Max_VIP_Speed, Min_BreakVIP, Max_BreakVIP);



	Event* pEv;
	//The next line is reading all the events from the file and add them to the event queue
	while (NumOfEvents-- && !Input.eof())
	{
		char Event_type;
		int EvTime;
		int O_id;

		Input >> Event_type;
		switch (Event_type)
		{
		case 'R':
			char ordType;
			int size;
			double money;
			ORD_TYPE type;
			Input >> ordType >> EvTime >> O_id >> size >> money;
			switch (ordType)
			{
			case 'N':
				type = TYPE_NRM;
				break;
			case 'G':
				type = TYPE_VGAN;
				break;
			case 'V':
				type = TYPE_VIP;
				break;
			}
			pEv = new ArrivalEvent(EvTime, O_id, type, size, money, PromotionLimit, VIP_PromotionLimit);
			EventsQueue.enqueue(pEv);
			break;
		case 'X':
			Input >> EvTime >> O_id;
			pEv = new CancellationEvent(EvTime, O_id);
			EventsQueue.enqueue(pEv);
			break;
		case'P':
			double money2;
			Input >> EvTime >> O_id >> money2;
			pEv = new Promotion(EvTime, O_id, money2);
			EventsQueue.enqueue(pEv);
			break;
		}
	}

	Input.close();
	return Filename;
}

bool Restaurant::Exit()
{
	return EventsQueue.isEmpty() && Waiting_orders.GetCount() == 0 && Waiting_orders_Vegan.isEmpty() && Service_Order.GetCount() == 0;
}
