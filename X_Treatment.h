#pragma once
#include "Treatment.h"
/**
 * @class E_treatment
 * @brief Represents an E-type (Electrotherapy) treatment.
 */
class X_treatment : public Treatment
{
public:
    /**
     * @brief Default constructor.
     */
    X_treatment();

    /**
     * @brief Parameterized constructor.
     * @param d Duration of the treatment.
     */
    X_treatment(int d);

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
    ~X_treatment();
};


