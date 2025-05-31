/**
 * @file EarlyPlist.h
 * @brief Defines the EarlyPlist class, a priority queue implementation for managing Early Patients objects.
 *
 * This file contains the declaration of the EarlyPlist class, which extends a generic priority queue
 * to handle pointer to Patient objects. It provides functionality for enqueuing and dequeuing patients based on
 * priority, counting the number of patients, printing the queue contents, and selecting a random patient
 * for rescheduling.
 *
 * @author Ibrahim Mohsen
 * @date March 2025
 */
#ifndef EARLYPLIST_H
#define EARLYPLIST_H
/**
 * @include <priQueue.h> >>> Header file for the priority queue implementation.
 * @include <Patient.h>  >>> Header file for the Patient class.
 * @include <random>   >>> Header file for random number generation.
 * @include <iostream>    >>> Header file for input/output stream for print functionalities.
 * @brief Includes necessary headers for the EarlyPlist class.
 */
class Patient;
#include "priQueue.h"
#include <random>
#include <iostream>
using namespace std;
/**
 * @class EarlyPlist
 * @brief A class that represents a priority queue for managing Early Patients.
 *
 * This class extends the priQueue class to handle pointers to Patient objects. It provides methods for
 * enqueuing and dequeuing patients based on priority, counting the number of patients, printing the queue
 * contents, and selecting a random patient for rescheduling.
 */
class EarlyPlist : public priQueue<Patient *>
{
public:
    /**
     * @brief Default constructor for EarlyPlist.
     * Initializes the priority queue and sets the patient count to zero.
     */
    EarlyPlist();

    ~EarlyPlist();

    /**
     * @brief Selects a random patient from the queue for rescheduling.
     * 
     * @return Pointer to the randomly selected Patient object.
     * @details This method generates a random number between 0 and 100
     * if the number is less than pResch, we call getRandomReschedule();
     */
    Patient* RandomReschedule(int pResch, int lastArrivalTime);
};

#endif // EARLYPLIST_H
