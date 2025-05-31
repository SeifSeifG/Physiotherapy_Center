#include "EU_WaitList.h"
#include "Patient.h"
#include "stdint.h"
#include <assert.h>
//for adding late patients to their waiting list
//according to PT+penalty as their PT
void EU_WaitList :: insertSorted(Patient* pPatient, int priority)
{
	EU_WaitList temp_list;
	Patient* temp_patient;

	//insert the pPatient in its order in temp Q
	while (!this->isEmpty())
	{
		//look at first patient in the queue original
		this->peek(temp_patient);

		//if their PT is greater than our priority
		if (temp_patient->getAppointmentTime() > priority)
		{
			//enqueue the new patient in the temp queue and exit
			temp_list.enqueue(pPatient);
			break;
		}

		this->dequeue(temp_patient);
		temp_list.enqueue(temp_patient);
	}
	//special case: if pri > all the PT in the original waiting list
	//we kept dequeueing until original list became empty
	if (this->isEmpty())
	{
		temp_list.enqueue(pPatient);
	}

	//maintain order, by transfering all the remaining patients
	while (!this->isEmpty())
	{
		this->dequeue(temp_patient);
		temp_list.enqueue(temp_patient);
	}

	//restore the original Queue
	while (!temp_list.isEmpty())
	{
		temp_list.dequeue(temp_patient);
		this->enqueue(temp_patient);
	}

}

//the arguments to this functions are
//1) TreatmentType : the type of treatment we are concerned with
//2) current time step : to calculate total latency
//3) inTreatmentList : traversed for a specific type of treatment that we are concerned with its patients
//4) waiting_list : traversed to calculate the waiting list time relative to current time
int EU_WaitList:: CalcTreatmentLatency(TreatmentType type, int currentTime, priQueue<Patient*> inTreatmentList)
{
	EU_WaitList waiting_list = *this;

	//get total treatment time
	Patient* _deqeueud_Patient = nullptr;
	Treatment* _their_treatment = nullptr;
	int _treatment_finish = 0;

	int TotalWaitTime = 0;

	//finish the patients in the wait list
	while (!waiting_list.isEmpty())
	{
		/*
		I want to dequeue until I find a patient with the same treatment type
		get how much shall I wait for them to finish
		then enqueue the waiting patient to the treatment list with their priority
		repeat the process until the waiting list is empty
		*/
		bool _caluclated_once = false;
		//only one of these if conditions will work per call
		while (!_caluclated_once && !inTreatmentList.isEmpty())
		{
			inTreatmentList.dequeue(_deqeueud_Patient, _treatment_finish);

			//else, check on the treatment type of the dequeued patient
			TreatmentType tempType = _deqeueud_Patient->getNextTreatment()->getType();

			if (tempType == type)
			{
				//if they match the type, we are waiting for them to finish
				TotalWaitTime += (_treatment_finish - currentTime);
				currentTime = _treatment_finish;
				_caluclated_once = true;
			}
		}

		waiting_list.dequeue(_deqeueud_Patient);
		_treatment_finish = _deqeueud_Patient->getNextTreatment()->getDuration();

		int _their_finish_time = currentTime + _treatment_finish;

		inTreatmentList.enqueue(_deqeueud_Patient, _their_finish_time);
	}

	//we shall reach this line when the waiting list is empty
	bool _free_device_found = false;

	//keep looking for the first patient with the same treatment type
	//in which case TotalWaitTime will increment
	while (!inTreatmentList.isEmpty() && !_free_device_found)
	{
		//first patient to leave the is the only one holding us back
		inTreatmentList.dequeue(_deqeueud_Patient, _treatment_finish);

		//else, check on the treatment type of the dequeued patient
		TreatmentType tempType = _deqeueud_Patient->getNextTreatment()->getType();

		//repeat the algorithm from above
		if (tempType == type)
		{
			//if they match the type, we are waiting for them to finish
			TotalWaitTime += (_treatment_finish - currentTime);
			_free_device_found = true;
		}
	}

	return TotalWaitTime;
}