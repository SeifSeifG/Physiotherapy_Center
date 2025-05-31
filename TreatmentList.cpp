#include "TreatmentList.h"
#include <vector>

Patient* TreatmentList::RandomBusyFailure(int pBusyFailure, int currentTime)
{
    // Return nullptr if no patients in treatment
    if (isEmpty()) { return nullptr; }

    // Generate random number to check for failure
    if (rand() % 100 >= pBusyFailure) { return nullptr; } // No failure this time

    // for holding original queue and restoring it later
    TreatmentList temp;

    // for dequeueing and enqueueing
    Patient* tempPatient = nullptr;
    int TreatmentFinishTime = 0;

    // for holding the data of the patient with generated index
    Patient* InterruptedPatient = nullptr;
    int foundFinishTime = 0;

    //to decide when to stop searching for a patient having E or U
    //indication we don't have a patient with matching characteristics
    int TotalPatients = getCount();
    bool* CheckedIndices = new bool[TotalPatients];
    int CheckedPatients = 0;

    //initialization for checked indices
    for (int i = 0; i < TotalPatients; i++) { CheckedIndices[i] = false; }

    //indicate when to stop searching if we find a patient with matching characteristics
    bool isPatientFound = false;

    while(CheckedPatients < TotalPatients)
    {
        //generate random index
        int RandomIndex = rand() % TotalPatients;

        //if this index is checked previouly, generate another
        if (CheckedIndices[RandomIndex])
            continue;

        //dequeue until you reach the required patient index
        for (int i = 0; i < RandomIndex; i++)
        {
            dequeue(tempPatient, TreatmentFinishTime);
            temp.enqueue(tempPatient, TreatmentFinishTime);
        }

        //get pointer to required patient index
        dequeue(InterruptedPatient, foundFinishTime);

        //empty *this before further processing
        while (dequeue(tempPatient, TreatmentFinishTime))
        {
            temp.enqueue(tempPatient, TreatmentFinishTime);
        }

        //restore the original queue
        while (temp.dequeue(tempPatient, TreatmentFinishTime))
        {
            enqueue(tempPatient, TreatmentFinishTime);
        }

        //if the patient choosen doesn't have either E or U treatments (has X treatment)
        if (InterruptedPatient->getNextTreatment()->getType() == X_THERAPY)
        {
            //increment number of checked patients
            CheckedPatients++;

            //mark the checked index as checked
            CheckedIndices[RandomIndex] = true;

            //put them back into the original queue
            enqueue(InterruptedPatient, foundFinishTime);

            //continue searching
            continue;
        }
        else
        {
            //for any other type other that X_THERAPY, we found our target
            isPatientFound = true;
            break;
        }

    }

    // clean up
    delete[] CheckedIndices;

    if (isPatientFound)
    {
        //calculate new duration
        int newDuration = (foundFinishTime - currentTime);

        //set new duration
        InterruptedPatient->getNextTreatment()->setDuration(newDuration);

        //flag indicates this treatment was interrupted
        InterruptedPatient->getNextTreatment()->setInterrupted();

        //return pointer to Interrupted Patient
        return InterruptedPatient;
    }
    else
    {
        return nullptr;
    }


}

