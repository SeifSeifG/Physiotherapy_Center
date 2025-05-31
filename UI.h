/**
 * @file UI.h
 * @brief Declares the UI class and related utility functions for displaying simulation data.
 *
 * This header defines the `UI` class, which provides a set of static display methods
 * used for visualizing the internal state of the scheduling simulation at various stages.
 * These include displaying all patients, current system status at each time step,
 * final patient outcomes, full patient history, and resource utilization statistics.
 *
 * Additionally, it declares overloaded stream insertion operators for convenient printing
 * of `Patient`, `Resource`, and `Treatment` objects in a human-readable format.
 *
 * These tools collectively aid in debugging, simulation tracking, and reporting.
 *
 * @authors Abdelrahman Samir & Ibrahim Mohsen
 * @date May 2025
 */
#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "Schedular.h"

/**
 * @class UI
 * @brief Provides static methods for displaying various aspects of the scheduler system.
 */
class UI
{
public:
    /**
     * @brief Displays a summary of all patients initially loaded into the scheduler.
     * @param mySchedular Reference to the scheduler system.
     */
    static void DisplayAllPatients(const Schedular& mySchedular);

    /**
     * @brief Displays the current state of the system at each time step.
     * Shows patients in all queues and current resource usage.
     * @param mySchedular Reference to the scheduler system.
     */
    static void DisplaySystemStatus(const Schedular& mySchedular);

    /**
     * @brief Displays the final report of finished patients including statistics.
     * @param mySchedular Reference to the scheduler system.
     */
    static void DisplayFinalPatient(const Schedular& mySchedular);

    /**
     * @brief Displays the entire history of patients, including their status and remaining treatments.
     * @param mySchedular Reference to the scheduler system.
     */
    static void DisplayHistory(const Schedular& mySchedular);

    /**
     * @brief Displays a summary of available and maintained resources.
     * @param mySchedular Reference to the scheduler system.
     */
    static void DisplayResources(const Schedular& mySchedular);
};

/**
 * @brief Overloaded output stream operator for printing Patient objects.
 * @param os Output stream.
 * @param myPatient Reference to the patient.
 * @return Reference to the output stream.
 */
ostream& operator<<(ostream& os, Patient& myPatient);

/**
 * @brief Overloaded output stream operator for printing Resource objects.
 * @param os Output stream.
 * @param r Reference to the resource.
 * @return Reference to the output stream.
 */
ostream& operator<<(ostream& os, const Resource& r);

/**
 * @brief Overloaded output stream operator for printing Treatment objects.
 * @param os Output stream.
 * @param t Reference to the treatment.
 * @return Reference to the output stream.
 */
ostream& operator<<(ostream& os, const Treatment& t);
