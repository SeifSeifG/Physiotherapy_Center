/**
 * @file U_treatment.h
 * @brief Represents an Ultrasound (U-type) treatment derived from the Treatment base class.
 *
 * Defines the U_treatment class which manages ultrasound therapy-specific behavior,
 * including patient queuing and resource assignment.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "Treatment.h"

class U_treatment : public Treatment
{
public:
    /**
     * @brief Default constructor.
     */
    U_treatment();

    /**
     * @brief Parameterized constructor.
     * @param d Duration of the treatment.
     */
    U_treatment(int d);

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
    ~U_treatment();
};


