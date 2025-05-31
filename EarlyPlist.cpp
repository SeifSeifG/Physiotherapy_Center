#include "EarlyPlist.h"
#include "Patient.h"
#define RANDOM_ARGUMENT time(0)
#define NEW_PT_RANGE 12
#define NEW_VT_RANGE 10


EarlyPlist ::EarlyPlist() : priQueue<Patient *>() {}

EarlyPlist ::~EarlyPlist()
{
    // parent destructor will delete all nodes
}

Patient *EarlyPlist ::RandomReschedule(int presc, int lastArrivalTime)
{
    if ((rand() % 100) < presc)
    {
        priQueue<Patient*> tempQueue;

        Patient* tempPatient = nullptr;

        Patient* foundPatient = nullptr;

        int tempPriority;

        int randomIndex = 0;

        int CurrCount = getCount();
        if (CurrCount != 0)
        {
            randomIndex = rand() % CurrCount; // Generate a random index
        }//else we choose the first patient to reschedule
        else
        {
            return foundPatient; //nullptr
        }

        //randomIndex = 1;
        // Dequeue elements until we reach the random index
        // Create a temporary queue to preserve the original queue
        for (int i = 0; i <= randomIndex; i++)
        {
            this->dequeue(tempPatient, tempPriority);

            // Enqueue the patient into the temporary queue
            if (i < randomIndex)
            {
                tempQueue.enqueue(tempPatient, tempPriority);
            }
            else
            {
                //prevent multiple rescheduling (just for peace in mind)
                if (tempPatient->isRescheduled())
                {
                    //if this random patient has rescheduled berfore, prevent more resheduling
                    tempQueue.enqueue(tempPatient, tempPriority);
                    //get out of the loop
                    break;
                }
                else
                {
                    //store their address for further processing
                    foundPatient = tempPatient;

                    //get out of the loop
                    break;
                }
            }
        }

        //continue deuque in the temp to preserve order
        while (!isEmpty())
        {
            this->dequeue(tempPatient, tempPriority);
            tempQueue.enqueue(tempPatient, tempPriority);
        }

        // Restore the original queue
        while (tempQueue.dequeue(tempPatient, tempPriority))
        {
            this->enqueue(tempPatient, tempPriority);
        }

        if (foundPatient)
        {
            //get their old App. time and Arrival time
            int old_PT = foundPatient->getAppointmentTime();
            int old_VT = foundPatient->getArrivalTime();

            //generate new appointment data,
            //Task : how to insert them in all patients list by arrival time?

            int new_PT = lastArrivalTime + (rand() % NEW_PT_RANGE);
            int new_VT = lastArrivalTime + (rand() % NEW_VT_RANGE);

            //reschdule to the same PT? doesnt seem right
            new_PT = (old_PT == new_PT) ? (new_PT + 1) : new_PT;

            //Same
            new_VT = (old_VT == new_VT) ? (new_VT + 1) : new_VT;

            foundPatient->markRescheduled(new_PT, new_VT);
        }

        return foundPatient;
    }
    else
    {
        return nullptr;
    }
}