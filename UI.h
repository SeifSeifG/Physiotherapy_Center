#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "Schedular.h"

class UI {
public:
    //displays all the patients at the start of the program
    //to track the input file
    static void DisplayAllPatients(const Schedular& mySchedular);

    //print the instantaneous state of the center
    static void DisplaySystemStatus(const Schedular& mySchedular);

    //prints the patients states at the end of the simulation
    static void displayFinalPatient(const Schedular& mySchedular);

    //prints all patients that entered the schedular class
    static void displayHistory(const Schedular& mySchedular);

    //prints all the resources in the schedular
    static void DisplayResources(const Schedular& mySchedular);


    //********* OVERLOADED ostream operators ************//
    // 
    //ostream& operator << (ostream& os, Patient& myPatient);
    //ostream& operator<<(ostream& os, const Resource& r);
    //ostream& operator<<(ostream& os, const Treatment& t)

};

#endif // UI_H