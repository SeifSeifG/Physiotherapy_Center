#pragma once
#include "EU_WaitList.h"
class Schedular;
/**
 * @class X_WaitList
 * @brief Represents a waiting list for patients, allowing random cancellation based on probability.
 */
class X_WaitList : public EU_WaitList
{
public:
    /**
     * @brief Attempts to randomly cancel a patient from the waiting list based on the hospital system's cancellation probability.
     *
     * The function iterates through the waiting list, checking patients who have only one remaining treatment.
     * If a patient meets the cancellation criteria, they are marked as cancelled and their status is updated.
     *
     * @param hospitalsystem Reference to the Schedular instance to get cancellation probability and update patient status.
     * @return true if a patient was successfully cancelled, false otherwise.
     */
    bool RandomCancel(Schedular& hospitalsystem);
};

