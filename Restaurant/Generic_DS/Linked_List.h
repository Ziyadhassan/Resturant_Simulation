#ifndef __LINKED_LIST_H_
#define __LINKED_LIST_H_

#pragma once
#include"..\Generic_DS\Node.h"
#include"..\Rest\Cook.h"

template<class T>
class Linked_List
{
	Node<T>* Head;
	Node<T>* Rear;
	int Count;
public:
	Linked_List()
	{
		Head = nullptr;
		Rear = nullptr;
		Count = 0;
	}
	Linked_List(const T& Item)
	{
		Node<T>* ptr = new Node<T>(item, nullptr);
		Head = Rear = ptr;
		Count = 1;
	}

	void Setter_Linked(int Number_Of_Node, Cook& cook, int MinSpeed, int MaxSpeed, int MinBreak, int MaxBreak) // Special Func for the cooks as it is mostly will be used while
	{	
		// constructing the linkedList of each type of cook
		Node<T>* ptr = NULL;
		Node<T>* prev = nullptr;

		int id = cook.GetID() + Number_Of_Node - 1;

		for (int i = 0; i < Number_Of_Node; i++)
		{
			ptr = new Node<Cook>(Cook(id--, cook.GetType(), rand() % (MaxSpeed - MinSpeed + 1) + MinSpeed, rand() % (MaxBreak - MinBreak + 1) + MinBreak, cook.Get_Num_Of_Ord_Break()), prev);
			prev = ptr;
			if (i == 0)
				Rear = ptr;
		}
		Head = ptr;
		Count = Number_Of_Node;
	}

	void Add_To_List(T* Item) // Add at the Front 
	{
		Node<T>* ptr = new Node<T>(*Item, Head);
		Head = ptr;
		Count++;
	}
	void Add_To_List(Node<T>* node)  // Add at the Front 
	{
		if (node)
		{
			node->setNext(Head);
			Head = node;
			Count++;
		}

	}

	void InsertEnd(T* Item)  // Add at the end of the list
	{
		Node<T>* ptr = new Node<T>(*Item, nullptr);
		if (Head)
		{
			Rear->setNext(ptr);
			Rear = ptr;
			Count++;
		}
		else
		{
			Head = Rear = ptr;
			Head->setNext(NULL);
			Rear->setNext(NULL);
			Count = 1;
		}

	}
	void InsertEnd(Node<T>* node)  // Add at the end of the list
	{
		if (node)
		{
			if (Head)
			{
				Rear->setNext(node);
				Rear = node;
				Count++;
			}
			else
			{
				Head = Rear = node;
				Count++;
			}

		}
	}


	bool Delete() // Delete the first node
	{
		if (Head)
		{
			Node<T>* ptr = Head;
			Head = Head->getNext();
			ptr->setNext(nullptr);
			delete ptr;
			Count--;
			return true;
		}
		else
		{
			return false; // List is empty
		}
	}
	bool Delete(T* ToBeRemoved) // Deletes the Sent Item
	{
		if (Head)
		{
			Node<T>* ptr = Head;
			Node<T>* ptr2 = Head->getNext();


			if (*ptr == ToBeRemoved)
			{
				this->Delete();
				return true;
			}


			while (ptr2) // assume we will not delete the head as there is a function the do this
			{
				if (*ptr2 == ToBeRemoved)
				{
					ptr->setNext(ptr2->getNext());
					Count--;
					ptr2->setNext(nullptr);
					delete ptr2;
					ptr2 = nullptr;
					return true;
				}
				ptr = ptr2;
				ptr2 = ptr2->getNext();
			}
			return false;
		}
		else
		{
			return false; // List is empty
		}
	}

	Node<T>* Remove(T* ToBeRemoved) // Removes the Sent Item but doesn't delete it
	{
		if (Head)
		{
			Node<T>* ptr = Head;
			Node<T>* ptr2 = Head->getNext();
			T* item;
			T* item2;
			if (ptr)
			{
				item = &Head->getItem();
			}
			if (ptr2)
			{
				item2 = &Head->getNext()->getItem();
			}

			if (item == ToBeRemoved)
			{
				if (!ptr2)
				{
					SetHead(nullptr);
					SetRear(nullptr);
					SetCount(0);
				}
				else
				{
					SetHead(ptr2);
					Count--;
				}
				ptr->setNext(nullptr);
				return ptr;
			}

			while (ptr2) // assume we will not delete the head as there is a function the do this
			{
				if (item2 == ToBeRemoved)
				{
					if (!ptr2->getNext())
					{
						SetRear(ptr);
					}
					ptr->setNext(ptr2->getNext());
					Count--;
					ptr2->setNext(nullptr);
					return ptr2;
				}
				ptr = ptr2;
				item = &ptr->getItem();
				ptr2 = ptr2->getNext();
				item2 = &ptr2->getItem();
			}
			return false;
		}
		else
		{
			return false; // List is empty
		}
	}

	bool IsEmpty()
	{
		if (Count)
		{
			return true;
		}
		return false;
	}

	Linked_List<T> Reverse() // it sends a copy of the list in reverse order
	{
		if (Head)
		{
			Linked_List<T> temp;
			Node<T>* iterator = Head;
			while (iterator)
			{
				Node<T>* ptr = new Node<T>(iterator->getItem());
				temp.Add_To_List(ptr);
				iterator = iterator->getNext();
			}
			return temp;
		}
	}


	int GetCount() const;
	Node<T>* GetHead() const;
	Node<T>* GetRear() const;

	void SetCount(int count);
	void SetHead(Node<T>* head);
	void SetRear(Node<T>* rear);


	virtual ~Linked_List() {}
};
#endif

template<class T>
inline int Linked_List<T>::GetCount() const
{
	return Count;
}

template<class T>
inline Node<T>* Linked_List<T>::GetHead() const
{
	return Head;
}

template<class T>
inline Node<T>* Linked_List<T>::GetRear() const
{
	return Rear;
}

template<class T>
inline void Linked_List<T>::SetCount(int count)
{
	Count = (count >= 0) ? count : 0;
}

template<class T>
inline void Linked_List<T>::SetHead(Node<T>* head)
{
	Head = head;
}

template<class T>
inline void Linked_List<T>::SetRear(Node<T>* rear)
{
	Rear = rear;
}
