
#ifndef _NODE
#define _NODE
template < typename T>
class Node
{
private :
	T* item;         // Pointer to a data item
	Node<T>* next;   // Pointer to the next node
public :
	Node();
	Node(T* Item);	
	Node(T* Item, Node<T>* nextNodePtr);
	void setItem(T* Item);
	void setNext(Node<T>* nextNodePtr);
	T* get_ptr_to_item() const ;
	T getItem() const ;
	Node<T>* getNext() const ;
}; // end Node
#endif

template < typename T>
Node<T>::Node() 
{
	next = nullptr;
} 

template < typename T>
Node<T>::Node(T* Item)
{
	item = Item;
	next = nullptr;
} 

template < typename T>
Node<T>::Node(T* Item, Node<T>* nextNodePtr)
{
	item = Item;
	next = nextNodePtr;
}
template < typename T>
void Node<T>::setItem(T* Item)
{
	item = Item;
} 

template < typename T>
void Node<T>::setNext(Node<T>* nextNodePtr)
{
	next = nextNodePtr;
} 

template< typename T>
T* Node<T>::get_ptr_to_item() const
{
	return item;
}

template < typename T>
T Node<T>::getItem() const
{
	return (*item);
} 

template < typename T>
Node<T>* Node<T>::getNext() const
{
	return next;
} 