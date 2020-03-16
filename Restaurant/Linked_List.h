#ifndef __LINKED_LIST_H_
#define __LINKED_LIST_H_

#pragma once
#include"Generic_DS/Node.h"
#include"Rest/Cook.h"

template<class T>
class Linked_List
{
	Node<T>* Head;
	Node<T>* Next;
	int Count;
public:
	Linked_List()
	{
		Head = nullptr;
		Next = nullptr;
		Count = 0;
	}
	Linked_List(const T& Item) // It is UseLess ( I think we will not use it )
	{
		Node<T>* ptr = new Node<T>(item, nullptr);
		Head = ptr;
		Count = 1;
	}

	Linked_List(int Number_Of_Node, const Cook& cook) // Special Const for the cooks as it is mostly will be used for cooks
	{
		Node<T>* ptr;
		Node<T>* prev = nullptr;

		int id = cook.GetID();

		for (int i = 0; i < Number_Of_Node; i++)
		{
			ptr = new Node<Cook>(Cook(id++, cook.GetType(), cook.GetSpeed(), cook.InBREAK(), cook.IsBUSY()), prev);
			prev = ptr;
		}
		Head = ptr;
		Count = Number_Of_Node;
	}

	void Add_To_List(const T& Item)
	{
		Node<T>* ptr = new Node<t>(Item, Head);
		Head = ptr;
		Count++;
	}
	bool Remove() // Removes the first node
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

	bool IsEmpty()
	{
		if (Count)
		{
			return true;
		}
		return false;
	}

	int GetCount() const;
	Node<T>* GetHead() const;

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
