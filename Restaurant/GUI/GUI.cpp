#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	DrawingItemsCount = 0;
	pWind = new window(WindWidth + 15, WindHeight, 0, 0);
	pWind->ChangeTitle("The Restautant");


	//Set color for each order type
	DrawingColors[TYPE_NRM] = RED;	//normal-order color
	DrawingColors[TYPE_VGAN] = LIMEGREEN;		//vegan-order color
	DrawingColors[TYPE_VIP] = GOLD;		//VIP-order color						
	DrawingColors[TYPE_URG] = CADETBLUE;		//URG-order color

	ClearStatusBar();
	ClearDrawingArea();
	DrawRestArea();

}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x, y;
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const
{
	string Label;
	char Key;
	while (1)
	{
		pWind->WaitKeyPress(Key);
		if (Key == 27)	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if (Key == 13)	//ENTER key is pressed
			return Label;
		if ((Key == 8) && (Label.size() >= 1))	//BackSpace is pressed
			Label.resize(Label.size() - 1);
		else
			Label += Key;

		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg) const	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar

	pWind->SetPen(BLACK);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.1), msg); // You may need to change these coordinates later 
}
void GUI::PrintStatusBar(string& Timestep, string& Normal, string& Vegan, string& VIP, string& Urgent)
{
	pWind->SetFont(18, BOLD, BY_NAME, "Arial"); // Constant for all

	pWind->SetPen(BLACK); // To Draw the Timestep
	pWind->DrawString(WindWidth / 2 - 34, WindHeight - (int)(StatusBarHeight / 1.1), Timestep);

	pWind->SetPen(DrawingColors[0]); // To Draw the Noraml Status
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.325), Normal);

	pWind->SetPen(DrawingColors[1]); // To Draw the Vegan Status
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.65), Vegan);

	pWind->SetPen(DrawingColors[2]); // To Draw the VIP Status
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 2.2), VIP);

	pWind->SetPen(DrawingColors[3]); // To Draw the Urgent Status
	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.1), Urgent);

	// To Draw the Lines in the Status Bar
	pWind->SetPen(CYAN, 3);
	pWind->DrawLine(210, 504, 210, 602);
	pWind->DrawLine(400, 504, 400, 602);
	pWind->DrawLine(4, 603, 1196, 603);
	pWind->DrawLine(710, 603, 760, 650);
	pWind->DrawLine(490, 603, 440, 650);

}

void GUI::PrintMessageN(string& Normal) const
{
	pWind->SetPen(DrawingColors[0]);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(600, WindHeight - (int)(StatusBarHeight / 1.325), Normal);
}

void GUI::PrintMessageG(string& Vegan) const
{
	pWind->SetPen(DrawingColors[1]);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(600, WindHeight - (int)(StatusBarHeight / 1.65), Vegan);
}

void GUI::PrintMessageV(string& VIP) const
{
	pWind->SetPen(DrawingColors[2]);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(600, WindHeight - (int)(StatusBarHeight / 2.2), VIP);
}

void GUI::PrintMessageInjury(string& Injuered) const
{
	pWind->SetPen(RED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(WindWidth - 170, WindHeight - (int)(StatusBarHeight / 1.1), Injuered);
}

void GUI::PrintMessageNoRemove(string& mode, string& served) const
{
	pWind->SetPen(VIOLET);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");

	pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 3.6), mode);
	pWind->DrawString(WindWidth / 2 - 100, WindHeight - (int)(StatusBarHeight / 3.6), served);
	pWind->DrawString(WindWidth - 300, WindHeight - (int)(StatusBarHeight / 3.6), "WELCOME TO OUR RESTURANT");

}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text)
{
	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(iX, iY, Text);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	pWind->SetPen(BLUE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(1, WindHeight - StatusBarHeight + 1, WindWidth, WindHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	pWind->SetPen(BLUE, 3);
	pWind->SetBrush(BLACK);
	pWind->DrawRectangle(1, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the brown square of the Rest
	pWind->SetPen(VIOLET);
	pWind->SetBrush(PURPLE);
	pWind->DrawRectangle(RestStartX, RestStartY, RestEndX, RestEndY);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(BLUE, 3);
	pWind->DrawLine(2, YHalfDrawingArea, WindWidth - 2, YHalfDrawingArea);
	pWind->DrawLine(WindWidth / 2, MenuBarHeight, WindWidth / 2, WindHeight - StatusBarHeight - 2);

	// 3- Drawing the 2 white crossed lines (inside the Rest)
	pWind->SetPen(WHITE);
	pWind->DrawLine(WindWidth / 2, YHalfDrawingArea - RestWidth / 2, WindWidth / 2, YHalfDrawingArea + RestWidth / 2);
	pWind->DrawLine(WindWidth / 2 - RestWidth / 2, YHalfDrawingArea, WindWidth / 2 + RestWidth / 2, YHalfDrawingArea);

	// 4- Drawing the 4 white squares inside the Rest (one for each region)
	//pWind->SetPen(WHITE);
	//pWind->SetBrush(WHITE);
	//pWind->DrawRectangle(RestStartX , RestStartY , RestStartX + 2*L/2, RestStartY + 2*L/2);
	//pWind->DrawRectangle(RestStartX + L/3, RestEndY - L/3, RestStartX + 2*L/3, RestEndY - 2*L/3);
	//pWind->DrawRectangle(RestEndX - 2*L/3, RestStartY + L/3, RestEndX - L/3, RestStartY + 2*L/3);
	//pWind->DrawRectangle(RestEndX - 2*L/3, RestEndY - L/3, RestEndX - L/3, RestEndY - 2*L/3);

	// 5- Writing regions labels
	pWind->SetPen(WHITE);
	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(RestStartX + (int)(0.1 * L), RestStartY + 5 * L / 12, "WAIT");

	pWind->DrawString(WindWidth / 2 + (int)(0.1 * L), RestStartY + 5 * L / 12, "COOK");

	pWind->DrawString(WindWidth / 2 + (int)(0.1 * L), YHalfDrawingArea + 5 * L / 12, "SRVG");

	pWind->DrawString(RestStartX + (int)(0.1 * L), YHalfDrawingArea + 5 * L / 12, "DONE");
	// 6- Drawing a line to seperate status bar

//	pWind->DrawLine(WindWidth / 2, MenuBarHeight, WindWidth / 2, WindHeight - StatusBarHeight);

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the passed item in its region
//region count in the numbers of items drawn so far in that item's region
void GUI::DrawSingleItem(const DrawingItem* pDitem, int RegionCount) const       // It is a private function
{

	if (RegionCount > MaxRegionOrderCount)
		return; //no more items can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if (RegionCount >= MaxHorizOrders)	//max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount - 1) % MaxHorizOrders + 1;
		YPos = (RegionCount - 1) / MaxHorizOrders + 1;
	}

	GUI_REGION Region = pDitem->region;

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance
	switch (Region)
	{
	case ORD_REG:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos; // YPos
		break;
	case COOK_REG:
		refX = (WindWidth / 2 + RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos; // YPos
		break;
	case SRV_REG:
		refX = (WindWidth / 2 + RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos; // YPos
		break;
	case DONE_REG:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos; // YPos
		break;
	default:
		break;
	}

	// Drawing the item
	pWind->SetPen(pDitem->clr);
	pWind->SetFont(20, BOLD, MODERN);
	pWind->DrawInteger(x, y, pDitem->ID);
}


//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw all items in DrawingList and ensure there is no overflow in the drawing*/
void GUI::DrawAllItems()
{

	//Prepare counter for each region
	int RegionsCounts[REG_CNT] = { 0 };	//initlaize all counters to zero

	DrawingItem* pDitem;
	for (int i = 0; i < DrawingItemsCount; i++)
	{
		pDitem = DrawingList[i];
		RegionsCounts[pDitem->region]++;
		DrawSingleItem(DrawingList[i], RegionsCounts[pDitem->region]);
	}

}

void GUI::UpdateInterface()
{
	ClearDrawingArea();
	DrawRestArea();
	DrawAllItems();
}

/*
	AddOrderForDrawing: Adds a new item related to the passed Order to the drawing list
*/
void GUI::AddToDrawingList(Order* pOrd)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pOrd->GetID();
	pDitem->clr = DrawingColors[pOrd->GetType()];
	ORD_STATUS order_status = pOrd->getStatus();
	GUI_REGION reg;

	switch (order_status)
	{
	case WAIT:
		reg = ORD_REG;	//region of waiting orders
		break;
	case SRV:
		reg = SRV_REG;	//region of waiting orders
		break;
	case DONE:
		reg = DONE_REG;	//region of waiting orders
		break;
	}

	pDitem->region = reg;

	DrawingList[DrawingItemsCount++] = pDitem;

}

void GUI::AddToDrawingList(Cook* pC)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pC->GetID();
	pDitem->clr = DrawingColors[pC->GetType()];
	pDitem->region = COOK_REG;

	DrawingList[DrawingItemsCount++] = pDitem;

}


// Adds all the cooks in the list to the drawing list (the free ones only )
void GUI::AddToDrawingList(Linked_List<Cook> L1, Linked_List<Cook> L2, Linked_List<Cook> L3)
{
	Node<Cook>* ptr1 = L1.GetHead();
	Node<Cook>* ptr2 = L2.GetHead();
	Node<Cook>* ptr3 = L3.GetHead();
	Cook cook;

	Queue<Cook> q1;
	Queue<Cook> q2;

	while (ptr1 && ptr2)
	{
		if (!ptr1->getItem().IsFree())
		{
			ptr1 = ptr1->getNext();
			continue;
		}
		if (!ptr2->getItem().IsFree())
		{
			ptr2 = ptr2->getNext();
			continue;
		}

		if (ptr1->getItem().GetAvilabilityTime() <= ptr2->getItem().GetAvilabilityTime())
		{
			cook = ptr1->getItem();
			q1.enqueue(cook);
			ptr1 = ptr1->getNext();
		}
		else
		{
			cook = ptr2->getItem();
			q1.enqueue(cook);
			ptr2 = ptr2->getNext();
		}
	}
	while (ptr1)
	{
		if (ptr1->getItem().IsFree())
		{
			cook = ptr1->getItem();
			q1.enqueue(cook);
		}
		ptr1 = ptr1->getNext();
	}

	while (ptr2)
	{
		if (ptr2->getItem().IsFree())
		{
			cook = ptr2->getItem();
			q1.enqueue(cook);
		}
		ptr2 = ptr2->getNext();
	}

	ptr1 = q1.dequeue();
	while (ptr1 && ptr3)
	{
		if (!ptr3->getItem().IsFree())
		{
			ptr3 = ptr3->getNext();
			continue;
		}

		if (ptr1->getItem().GetAvilabilityTime() <= ptr3->getItem().GetAvilabilityTime())
		{
			cook = ptr1->getItem();
			q2.enqueue(cook);
			ptr1 = q1.dequeue();
		}
		else
		{
			cook = ptr3->getItem();
			q2.enqueue(cook);
			ptr3 = ptr3->getNext();
		}
	}
	while (ptr1)
	{
		cook = ptr1->getItem();
		q2.enqueue(cook);
		ptr1 = q1.dequeue();

	}
	while (ptr3)
	{
		if (ptr3->getItem().IsFree())
		{
			cook = ptr3->getItem();
			q2.enqueue(cook);
		}
		ptr3 = ptr3->getNext();
	}

	ptr1 = q2.dequeue();
	while (ptr1)
	{
		AddToDrawingList(&ptr1->getItem());
		ptr1 = q2.dequeue();
	}
}

void GUI::AddToDrawingList(Linked_List<Order> LL) // Adds all the Ordes in the list to the drawing list
{
	Node<Order>* ptr = LL.GetHead();
	int count = LL.GetCount();
	Order** ord = new Order * [count];
	for (int i = 0; i < count; i++)
	{
		ord[i] = &ptr->getItem();
		ptr = ptr->getNext();
	}
	for (int i = count - 1; i >= 0; i--)
		AddToDrawingList(ord[i]);
	delete[] ord;
}

void GUI::AddToDrawingList(Queue<Order> LL)
{
	Node<Order>* ptr = LL.GetHead();
	Order ord;
	while (ptr)
	{
		ord = ptr->getItem();
		AddToDrawingList(&ord);
		ptr = ptr->getNext();
	}
}


// Adds all the Orders (Waiting) in the list to the drawing list
void GUI::AddToDrawingList(Priority_Queue<Order> LL, Queue<Order> L2)
{
	Node<Order>* ptr = LL.Get_Head();
	Node<Order>* p = L2.GetHead();
	Order ord;
	while (ptr && p)
	{
		if (ptr->getItem().Get_Arrival_Time() <= p->getItem().Get_Arrival_Time())
		{
			ord = ptr->getItem();
			AddToDrawingList(&ord);
			ptr = ptr->getNext();
		}
		else
		{
			ord = p->getItem();
			AddToDrawingList(&ord);
			p = p->getNext();
		}
	}
	while (ptr)
	{
		ord = ptr->getItem();
		AddToDrawingList(&ord);
		ptr = ptr->getNext();

	}
	while (p)
	{
		ord = p->getItem();
		AddToDrawingList(&ord);
		p = p->getNext();
	}

}
void GUI::ResetDrawingList()
{
	for (int i = 0; i < DrawingItemsCount; i++)
		delete DrawingList[i];

	DrawingItemsCount = 0;
}




PROG_MODE	GUI::getGUIMode() const
{
	PROG_MODE Mode;
	do
	{
		PrintMessage("Please select GUI mode: (1)Interactive, (2)StepByStep, (3)Silent... ");
		string S = GetString();
		Mode = (PROG_MODE)(atoi(S.c_str()) - 1);
	} while (Mode < 0 || Mode >= MODE_CNT);

	return Mode;
}
