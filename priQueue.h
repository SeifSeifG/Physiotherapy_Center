#pragma once
#include "priNode.h"
#include "QueueADT.h"
#include <climits>
#include <iostream>
using namespace std;

//This class impelements the priority queue as a sorted list (Linked List)
//The item with highest priority is at the front of the queue
template <typename T>
class priQueue
{
    priNode<T>* head;
    int count;
public:
    priQueue() : head(nullptr), count(0) {}

    ~priQueue() {
        T tmp;
        int p;
        while (dequeue(tmp,p));
    }

    //insert the new node in its correct position according to its priority
    void enqueue(const T& data, int priority) 
    {
        priNode<T>* newNode = new priNode<T>(data, priority);

        //initially empty queue

		// Seif: the priority must be smaller than the head priority to print the smallest PT first
        if (head == nullptr || priority < head->getPri()) 
        {
            newNode->setNext(head);
            head = newNode;
            count++;
            return;
        }
       

        priNode<T>* current = head;    
        //Seif : the >= sign here is not the original given, I changed it
        while (current->getNext() && priority >= current->getNext()->getPri()) 
        {
            current = current->getNext();
        }
        newNode->setNext( current->getNext());
        current->setNext( newNode);        
        count++;
    }

    //pri stores the priority of the dequeued item
    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        priNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        count--;
        return true;
    }

    bool peek(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem(pri);
        pri = head->getPri();
        return true;
    }

    bool isEmpty() const 
    {
        return (head == nullptr);
    }

    int getCount() const
    {
        return count;
    }

    // Add printQueue as a member function
    void printQueue() const 
    {
        if (isEmpty()) 
        {
            return;
        }

        priNode<T>* temp = head;
        while (temp) 
        {
            int pri = 0;
            std :: cout << *(temp->getItem(pri)) << "\n";  // Assuming operator<< is overloaded
            temp = temp->getNext();
        }
    }

    // Copy constructor
    priQueue(const priQueue<T>& other) : head(nullptr), count(0) 
    {
        priNode<T>* otherCurrent = other.head;
        priNode<T>* thisCurrent = nullptr;

        while (otherCurrent != nullptr) {
            // Extract data and priority from source node
            int pri;
            T data = otherCurrent->getItem(pri);

            // Create new node
            priNode<T>* newNode = new priNode<T>(data, pri);

            // Link nodes
            if (head == nullptr) {
                head = newNode;
            }
            else {
                thisCurrent->setNext(newNode);
            }

            thisCurrent = newNode;
            otherCurrent = otherCurrent->getNext();
            count++;
        }
    }
};
