#include <iostream>
#include "LinkedQueue.h"

using namespace std;


// Sample class to test LinkedQueue
class Patient {
public:
    int ID;
    string name;


    Patient(int id, string n) : ID(id), name(n) {}
    Patient() {}

    void display() const {
        cout << "Patient ID: " << ID << ", Name: " << name << endl;
    }
};


int main() {
    // Create a queue for Patient objects
    LinkedQueue<Patient> waitingQueue;

    // Test isEmpty on an empty queue
    cout << "Is queue empty? " << (waitingQueue.isEmpty() ? "Yes" : "No") << endl;

    // Enqueue patients
    Patient* p1 = new Patient(1, "John Doe");
    Patient* p2 = new Patient(2, "Jane Smith");
    Patient* p3 = new Patient(3, "Alice Johnson");

    cout << "\nEnqueuing patients...\n";
    waitingQueue.enqueue(p1);
    waitingQueue.enqueue(p2);
    waitingQueue.enqueue(p3);

    // Test isEmpty after enqueuing
    cout << "Is queue empty? " << (waitingQueue.isEmpty() ? "Yes" : "No") << endl;

    // Peek at the front patient
    Patient* frontPatient = new Patient;
    if (waitingQueue.peek(*frontPatient)) {
        cout << "\nPeeking front patient:\n";
        frontPatient->display();
    }
    else {
        cout << "\nPeek failed: Queue is empty.\n";
    }

    // Dequeue and process each patient
    cout << "\nDequeuing patients...\n";
    Patient* dequeuedPatient;
    while (waitingQueue.dequeue(dequeuedPatient)) {
        cout << "Dequeued: ";
        dequeuedPatient->display();
        delete dequeuedPatient; // Free allocated memory
    }

    // Test isEmpty after all dequeues
    cout << "Is queue empty? " << (waitingQueue.isEmpty() ? "Yes" : "No") << endl;

    // Testing the copy constructor
    cout << "\nTesting copy constructor...\n";
    waitingQueue.enqueue(new Patient(4, "Michael Brown"));
    waitingQueue.enqueue(new Patient(5, "Emily White"));

    LinkedQueue<Patient> copiedQueue(waitingQueue);

    cout << "Original queue front:\n";
    if (waitingQueue.peek(*frontPatient)) frontPatient->display();

    cout << "Copied queue front:\n";
    if (copiedQueue.peek(*frontPatient)) frontPatient->display();

    // Cleanup remaining patients in original queue
    while (waitingQueue.dequeue(dequeuedPatient)) delete dequeuedPatient;
    while (copiedQueue.dequeue(dequeuedPatient)) delete dequeuedPatient;

    return 0;
}
