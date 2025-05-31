#ifndef _NODE
#define _NODE
#include "Treatment.h"
template < typename T>
class Node
{
private:
	T item; // A data item
	Node<T>* next; // Pointer to next node
public:
	Node();
	Node(const T& r_Item);
	Node(const T& r_Item, Node<T>* nextNodePtr);
	void setItem(const T& r_Item);
	void setNext(Node<T>* nextNodePtr);
	T getItem() const;
	Node<T>* getNext() const;
}; // end Node
#endif

template < typename T>
Node<T>::Node()
{
	next = nullptr;
}

template < typename T>
Node<T>::Node(const T& r_Item)
{
	item = r_Item;
	next = nullptr;
}

template < typename T>
Node<T>::Node(const T& r_Item, Node<T>* nextNodePtr)
{
	item = r_Item;
	next = nextNodePtr;
}
template < typename T>
void Node<T>::setItem(const T& r_Item)
{
	item = r_Item;
}

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
}

template < typename T>
T Node<T>::getItem() const
{
	return item;
}

template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
}

template<>
class Node<Treatment*>
{
private:
	Treatment* item; // A data item
	Node<Treatment*>* next; // Pointer to next node
public:
	Node()
	{
		item = nullptr;
		next = nullptr;
	}

	Node(Treatment* r_Item)
	{
		item = r_Item;
		next = nullptr;
	}

	Node(Treatment* r_Item, Node<Treatment* >* nextNodePtr)
	{
		item = r_Item;
		next = nextNodePtr;
	}

	void setItem(Treatment* r_Item)
	{
		item = r_Item;
	}

	void setNext(Node<Treatment*>* nextNodePtr)
	{
		next = nextNodePtr;
	}


	Treatment* getItem() const
	{
		return item;
	}


	Node<Treatment* >* getNext() const
	{
		return next;
	}
};