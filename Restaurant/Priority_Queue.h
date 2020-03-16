#pragma once
#include"Generic_DS/Node.h"
#include"Rest/Order.h"

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

	T* PoP()
	{
		if (Head)
		{
			T ptr = Head->getItem();
			Head = Head->getNext();
			Count--;
			return &ptr;
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

	~Priority_Queue() {}
};

template<>
void Priority_Queue<Order>::push(const Order& Item)
{
	if (Head)
	{
		Node<Order>* ptr = new Node<Order>(Item);
		if (ptr->getItem().Get_Priority_Value() >= Head->getItem().Get_Priority_Value())
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
		Node<Order>* iteritor = Head ->getNext();
		while ( iteritor->getItem().Get_Priority_Value() > ptr->getItem().Get_Priority_Value() )
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
}