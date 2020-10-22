#pragma once
#include"..\Generic_DS\Node.h"
#include"..\Rest\Cook.h"

template<class T>
class Priority_Queue
{
private:
	Node<T>* Head;
	Node<T>* Rear;
	int Count;
public:
	Priority_Queue()
	{
		Head = Rear = nullptr;
		Count = 0;
	}

	Priority_Queue(const  T& Item)
	{
		Node<T>* ptr = new  Node<T>(Item, nullptr);
		Head = ptr;
		Count = 1;
	}

	void SetCount(int count) { Count = (count >= 0) ? count : Count; }
	void SetHead(Node<T>* head) { Head = head; }
	void SetRear(Node<T>* rear) { Rear = rear; }
	int GetCount() { return Count; }
	Node<T>* Get_Head() { return Head; }
	Node<T>* Get_Rear() { return Rear; }



	Node<T>* PoP()
	{
		if (Head)
		{
			Node<T>* ptr = Head;
			Head = Head->getNext();
			Count--;
			return ptr;
		}
		return nullptr;
	}

	void push(const T& Item)
	{
		Node<T>* newNodePtr = new Node<T>(Item);
		// Insert the new node
		if (isEmpty())
			frontPtr = newNodePtr; // The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty
		backPtr = newNodePtr; // New node is at back
		return true;
		// end enqueue
	}


	void push(Node<T>* node)
	{
		if (isEmpty())
			frontPtr = node; // The queue is empty
		else
			backPtr->setNext(node); // The queue was not empty
		backPtr = node; // New node is at back
		return true;
	}

	~Priority_Queue() {}
};

template<>
void Priority_Queue<Order>::push(const Order& Item) //For the "Waiting list" of orders
{
	if (Head)
	{
		Node<Order>* ptr = new Node<Order>(Item);
		if (ptr->getItem().Get_Priority_Value() > Head->getItem().Get_Priority_Value())
		{
			ptr->setNext(Head);
			Head = ptr;
			Count++;
			return;
		}

		if (ptr->getItem().Get_Priority_Value() <= Rear->getItem().Get_Priority_Value())
		{
			Rear->setNext(ptr);
			Rear = ptr;
			Count++;
			return;
		}

		Node<Order>* prev = Head;
		Node<Order>* iteritor = Head->getNext();
		while (ptr->getItem().Get_Priority_Value() <= iteritor->getItem().Get_Priority_Value())
		{
			Head = iteritor;
			iteritor = iteritor->getNext();
		}

		prev->setNext(ptr);
		ptr->setNext(iteritor);

	}
	else
	{
		Node<Order>* ptr = new Node<Order>(Item);
		Rear = Head = ptr;
		Count = 1;
	}
};

template<>
void Priority_Queue<Order>::push(Node<Order>* node) //For the "Waiting list" of orders
{
	if (Head)
	{
		Node<Order>* ptr = Head;
		if (node->getItem().Get_Priority_Value() > ptr->getItem().Get_Priority_Value())
		{
			node->setNext(ptr);
			Head = node;
			Count++;
			return;
		}

		if (node->getItem().Get_Priority_Value() <= Rear->getItem().Get_Priority_Value())
		{
			Rear->setNext(node);
			Rear = node;
			node->setNext(nullptr);
			Count++;
			return;
		}

		Node<Order>* iteritor = Head->getNext();
		while (node->getItem().Get_Priority_Value() <= iteritor->getItem().Get_Priority_Value())
		{
			ptr = iteritor;
			iteritor = iteritor->getNext();
		}

		ptr->setNext(node);
		node->setNext(iteritor);
		Count++;
		return;

	}
	else
	{
		Rear = Head = node;
		Count = 1;
	}


};


template<>
Node<Order>* Priority_Queue<Order>::PoP()
{
	if (Head)
	{
		Node<Order>* ptr = Head;
		Head = Head->getNext();
		Count--;
		ptr->setNext(nullptr);
		return ptr;
	}
	return NULL;
};