/**
 * @file TreatmentList.h
 * @brief Declares the TreatmentList class for managing patients currently under treatment.
 *
 * This file contains the declaration of the TreatmentList class, which manages a queue of patients
 * undergoing treatment. It provides functionality to simulate random busy failures affecting patients
 * currently receiving treatments other than X therapy, adjusting their treatment durations accordingly.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "priQueue.h"
#include "Patient.h"

 /**
  * @class TreatmentList
  * @brief Manages a queue of patients undergoing treatments.
  *
  * TreatmentList provides mechanisms for managing patients currently receiving treatment.
  * It supports random busy failure simulation that may interrupt patient treatments and adjust durations.
  */
class TreatmentList : public priQueue <Patient*>
{
public:
    /**
     * @brief Simulates a random busy failure affecting one patient in treatment.
     *
     * The function uses a probability check to determine if a failure occurs.
     * If so, it randomly selects a patient currently under treatment (excluding those with only X therapy)
     * and interrupts their treatment by adjusting its remaining duration.
     *
     * @param pBusyFailure Probability (0-100) of failure occurring during busy state.
     * @param currentTime The current simulation time to calculate remaining treatment duration.
     * @return Pointer to the Patient whose treatment was interrupted, or nullptr if no failure occurred.
     */
    Patient* RandomBusyFailure(int pBusyFailure, int currentTime);

    // Other public member functions would be declared here

    // Note: This header assumes necessary queue functions like enqueue, dequeue, getCount, isEmpty are implemented in the class.
};