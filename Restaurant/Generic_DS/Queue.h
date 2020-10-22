#ifndef __QUEUE_H_
#define __QUEUE_H_

/*This code is an updated version from "Data Abstraction & Problem Solving with C++,WALLS AND MIRRORS ,SIXTH EDITION"*/

/*
This is a program that implements the queue abstract data type using a linked list.
The queue is implemented as a chain of linked nodes that has two pointers,
a frontPtr pointer for the front of the queue and a backPtr pointer for the back of the queue.
*/

/*

				The Node: item of type T and a "next" pointer
					-------------
					| item| next | --->
					-------------
General Queue case:

				 frontPtr																backPtr
					\											   						/
					 \											  					   /
					------------- 	  ------------- 	  ------------- 	  -------------
					| item| next |--->| item| next |--->  | item| next |--->  | item| next |---> NULL
					------------- 	  ------------- 	  ------------- 	  -------------

Empty Case:

				 frontptr	 backptr
						\	 /
						 \	/
					---- NULL ------


Single Node Case:
				 frontPtr	 backPtr
					\		/
					 \	   /
					--------
					|	|nxt -->NULL
					--------

*/

#include "Node.h"

template <typename T>
class Queue
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	Queue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool enqueue(Node<T>* node);
	bool dequeue(T& frntEntry);
	Node<T>* dequeue();
	bool peekFront(T& frntEntry)  const;
	bool peekRear(T& RearEntry)   const;
	Node<T>* GetHead();
	Node<T>* GetRear();
	void SetHead(Node<T>* head);
	void SetRear(Node<T>* rear);
	T* toArray(int& count);	//returns array of T (array if items)
	~Queue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: Queue()
The constructor of the Queue class.

*/

template <typename T>
Queue<T>::Queue()
{
	backPtr = NULL;
	frontPtr = NULL;

}
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool Queue<T>::isEmpty() const
{
	if (frontPtr == NULL)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty
	backPtr = newNodePtr; // New node is at back
	return true;
} // end enqueue


template<typename T>
inline bool Queue<T>::enqueue(Node<T>* node)
{
	if (isEmpty())
		frontPtr = node; // The queue is empty
	else
		backPtr->setNext(node); // The queue was not empty
	backPtr = node; // New node is at back
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool Queue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = NULL;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;


	return true;

}

template<typename T>
inline Node<T>* Queue<T>::dequeue()
{
	if (frontPtr)
	{
		Node<T>* ptr = frontPtr;
		frontPtr = frontPtr->getNext();
		ptr->setNext(NULL);
		return ptr;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if Queue is empty
*/
template <typename T>
bool Queue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
template<typename T>
inline bool Queue<T>::peekRear(T & RearEntry) const
{
	if (isEmpty())
		return false;

	RearEntry = backPtr->getItem();
	return true;
}
template<typename T>
inline Node<T>* Queue<T>::GetHead()
{
	return frontPtr;
}
template<typename T>
inline Node<T>* Queue<T>::GetRear()
{
	return backPtr;
}
template<typename T>
inline void Queue<T>::SetHead(Node<T>* head)
{
	frontPtr = head;
}
template<typename T>
inline void Queue<T>::SetRear(Node<T>* rear)
{
	backPtr = rear;
}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::~Queue()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if Queue is empty)
returns: The array of T. (NULL if Queue is empty)
*/

template <typename T>
T* Queue<T>::toArray(int& count)
{
	count = 0;

	if (!frontPtr)
		return NULL;
	//counting the no. of items in the Queue
	Node<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

#endif
