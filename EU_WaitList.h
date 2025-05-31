/**
 * @file EU_WaitList.h
 * @brief Declares the EU_WaitList class for managing a waiting list of patients in the EU category.
 *
 * This file contains the declaration of the EU_WaitList class, which manages a queue of patients waiting
 * for treatments in the Emergency/Urgent category. It provides methods to insert patients in a sorted
 * manner based on priority and to calculate total treatment latency for a specific treatment type.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
class Patient;
/**
 * @class EU_WaitList
 * @brief Manages a sorted waiting list of patients requiring Emergency/Urgent treatments.
 *
 * This class provides functionalities to insert patients into the waiting list based on their priority,
 * and to calculate total latency for treatments of a specified type.
 */
class EU_WaitList : public LinkedQueue<Patient*>
{
public:
    /**
     * @brief Inserts a patient into the waiting list sorted by their priority.
     *
     * The patient is inserted such that the list remains sorted by the appointment time (priority).
     *
     * @param pPatient Pointer to the Patient object to insert.
     * @param priority The priority value used to sort the patient in the list.
     */
    void insertSorted(Patient* pPatient, int priority);

    /**
     * @brief Calculates the total latency for patients waiting and receiving a specific treatment type.
     *
     * This method computes the total waiting time for treatments of a given type by traversing both the
     * waiting list and the in-treatment queue, considering current simulation time.
     *
     * *this : traversed to calculate the waiting list time relative to current time
     *
     * @param type The TreatmentType : used to search the intreatment list
     * @param currentTime : to calculate total latency
     * @param inTreatmentList : traversed for a specific type of treatment
     * @return Total latency time (wait + treatment) for the specified treatment type.
     */
    int CalcTreatmentLatency(TreatmentType type, int currentTime, priQueue<Patient*> inTreatmentList);

    // Additional member functions and variables inherited from priQueue<Patient*>.
};





