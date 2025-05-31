#include "X_WaitList.h"
#include "Schedular.h"
#include <assert.h>


bool X_WaitList::RandomCancel(Schedular& hospitalsystem)
{
	int pCancel = hospitalsystem.getpCancel();

	//before any thing, just check if we should cancel or not
	int probability = rand() % 100;

	if (probability >= pCancel)
		return false;

	X_WaitList temp;

	Patient* _to_cancel = nullptr;

	Patient* _cancel_found = nullptr;

	bool cancelled_successfully = false;

	while (!isEmpty())
	{
		//inspect the patient
		dequeue(_to_cancel);

		//get copy of the patient treatment list (is it a copy?)
		LinkedQueue <Treatment*> _this_patient_Tlist = _to_cancel->getRemainingTreatments();

		Treatment* _next_treatment = nullptr;

		//this first dequeue is sure to be X treatment (no need to process it)
		_this_patient_Tlist.dequeue(_next_treatment);

		assert(_next_treatment && "shallow copy");

		//check this patient has no more treatments after the X_treatment
		Treatment* _this_isnt_last_treatment = nullptr;

		if (_this_patient_Tlist.peek(_this_isnt_last_treatment))
		{
			temp.enqueue(_to_cancel);
			continue; //move on to another patient in the waiting list
		}
		else //this patient ’MAY cancel the treatment
		{

			//leave them dequeued from the waititng list
			_cancel_found = _to_cancel;
			break;
			//cancel the patient

		}
		temp.enqueue(_to_cancel);
	}

	if (_cancel_found)
	{
		_cancel_found->setCancelled(true);
		_cancel_found->setStatus(FINISHED);
		//_cancel_found->setFinishTime(currentTime);
		cancelled_successfully = hospitalsystem.IsPatientFinished(_cancel_found);
	}

	//independant whether the cancel is found or not
	while (!isEmpty())
	{
		//continue dequeueing after finding the patient to be cancelled
		//to preserve the waiting list order
		dequeue(_to_cancel);
		temp.enqueue(_to_cancel);
	}

	while (!temp.isEmpty())
	{
		//restore the order
		temp.dequeue(_to_cancel);
		enqueue(_to_cancel);
	}

	return cancelled_successfully;

}