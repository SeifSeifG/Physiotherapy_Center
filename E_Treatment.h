/**
 * @file E_treatment.h
 * @brief Represents an Electrotherapy (E-type) treatment derived from the Treatment base class.
 *
 * Defines the E_treatment class which handles electrotherapy-specific logic,
 * including patient waitlist management and resource assignment.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "Treatment.h"
class Schedular;

class E_treatment : public Treatment
{
public:
    /**
     * @brief Default constructor.
     */
    E_treatment();

    /**
     * @brief Parameterized constructor.
     * @param d Duration of the treatment.
     */
    E_treatment(int d);

    /**
     * @brief Moves the patient to the E wait list in the scheduler.
     * @param scheduler Reference to the scheduler.
     * @param patient Pointer to the patient.
     * @return True if successfully added to wait list.
     */
    bool MoveToWait(Schedular& scheduler, Patient* patient) override;

    /**
     * @brief Checks if there is an available E-type resource.
     * @param scheduler Reference to the scheduler.
     * @return True if an E device is available.
     */
    bool CanAssign(Schedular& scheduler) override;

    /**
     * @brief Prints treatment information to output stream.
     * @param os Output stream.
     */
    void print(ostream& os) const override;

    /**
     * @brief Destructor.
     */
    ~E_treatment();
};

