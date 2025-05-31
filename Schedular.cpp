#include "Schedular.h"

//----------------------------------------------------------------------------------
// PRIVATE HELPER FUNCTIONS
//----------------------------------------------------------------------------------

/**
 * @brief Handles recovering patients and adjusts treatment order.
 * @param rec Pointer to the recovering patient.
 */
void Schedular::_handle_recovering_patient(Patient* rec)
{
	LinkedQueue<Treatment*> _this_patient_treatments = rec->getRemainingTreatments();

	//no need to handle anything, it is just 1 single treatment type
	if (_this_patient_treatments.getCount() == 1)
	{
		return;
	}

	Treatment* next = nullptr;

	int eWait = INT_MAX;
	int uWait = INT_MAX;
	int xWait = INT_MAX;

	//1) get the latemcy in each treatment type
	while (_this_patient_treatments.dequeue(next))
	{
		TreatmentType type = next->getType();

		switch (type)
		{
		case E_THERAPY:
			if (next->CanAssign(*this)) 
				eWait = 0; 
			else
				eWait = Ewaiting.CalcTreatmentLatency(E_THERAPY, currentTime, inTreatment);
			break;
		case U_THERAPY:
			if (next->CanAssign(*this))
				uWait = 0;
			else
				uWait = Uwaiting.CalcTreatmentLatency(U_THERAPY, currentTime, inTreatment);
			break;
		case X_THERAPY:
			if (next->CanAssign(*this))
				xWait = 0;
			else
				xWait = Xwaiting.CalcTreatmentLatency(X_THERAPY, currentTime, inTreatment);
			break;
		}
	}

	//2) compare them
	bool least_is_E = ((eWait <= uWait) && (eWait <= xWait));

	bool least_is_U = ((uWait <= eWait) && (uWait <= xWait));

	bool least_is_X = ((xWait <= uWait) && (xWait <= eWait));

	//3) sort the queue of the patient accordingly
	if		(least_is_E)
	{
		rec->ResortTreatments(E_THERAPY);
	}
	else if (least_is_U)
	{
		rec->ResortTreatments(U_THERAPY);
	}
	else  //(least_is_X)
	{
		rec->ResortTreatments(X_THERAPY);
	}
}

/**
 * @brief Manages rescheduling logic.
 * @return True if a patient was rescheduled, false otherwise.
 */
bool Schedular::_handle_rescheduling_patient()
{
	//presc is passed to compare it against a random generated number
	Patient* isResc = EARLY_patients.RandomReschedule(pResc, lastArrivalTime);

	if (!isResc)
		return false;

	AddToAllList(isResc);
	return true;
}

ResType Schedular::_handle_busy_failure()
{
	Patient* InterruptedPatient = inTreatment.RandomBusyFailure(pBusyFailure, currentTime);

	if (InterruptedPatient)
	{
		// 1) process Patient
		AddToInterrupted(InterruptedPatient, InterruptedPatient->getAppointmentTime());

		// 2) process the failed resource
		Resource* AssginedRes = InterruptedPatient->getNextTreatment()->getAssignedResource();
		Edevice* edev = dynamic_cast <Edevice*> (AssginedRes);
		Udevice* udev = dynamic_cast <Udevice*> (AssginedRes);
		if (edev)
		{
			edev->BusyFail();
			MaintenanceE.enqueue(edev, currentTime + edev->getMaintainanceTime());
			return _Edevice;
		}
		else if (udev)
		{
			udev->BusyFail();
			MaintenanceU.enqueue(udev,currentTime + udev->getMaintainanceTime());
			return _Udevice;
		}
	}

	return _dummydevice;
}

/**
 * @brief Calculates final statistics after simulation ends.
 */
void Schedular::CalcPatientStatistics()
{
	//prevent dividing by zero exception
	if (TotalPatients == 0)
		return;

	ArrayStack<Patient*> copy = Finished_patients;

	Patient* myPatient = nullptr;

	int TotalWait = 0;
	int TotalWait_N = 0;
	int TotalWait_R = 0;

	int TotalTreatment = 0;
	int TotalTreatment_N = 0;
	int TotalTreatment_R = 0;

	int TotalCancelled = 0;
	int TotalReschduled = 0;

	int EarlyPatient = 0;

	while (copy.pop(myPatient))
	{
		int _wait_time = myPatient->getTotalWaitingTime();
		int _treatment_time = myPatient->getTotalTreatmentTime();

		TotalWait += _wait_time;
		TotalTreatment += _treatment_time;

		if (myPatient->getType() == NORMAL)
		{
			TotalWait_N += _wait_time;
			TotalTreatment_N += _treatment_time;
		}
		else
		{
			TotalWait_R += _wait_time;
			TotalTreatment_R += _treatment_time;
		}

		if (myPatient->isCancelled())
			TotalCancelled++;

		if (myPatient->isRescheduled())
			TotalReschduled++;

	}
	TotalTimeSteps;

	AvgWaitTime_all = (double)TotalWait / TotalTimeSteps;
	AvgWaitTime_N = (double)TotalWait_N / TotalTimeSteps;
	AvgWaitTime_R = (double)TotalWait_R / TotalTimeSteps;

	AvgTreatmentTime_all = (double)TotalTreatment / TotalTimeSteps;
	AvgTreatmentTime_N = (double)TotalTreatment_N / TotalTimeSteps;
	AvgTreatmentTime_R = (double)TotalTreatment_R / TotalTimeSteps;

	perc_accepted_cancell = (double)TotalCancelled / TotalPatients;
	perc_accepted_resch = (double)TotalReschduled / TotalPatients;

	perc_early_patients = (double)TotalEarly / TotalPatients;
	perc_late_patients = (double)TotalLate / TotalPatients;

	if (NumEdevices)
		perc_freefail_E = (double)TotalFreeFail_E / NumEdevices;
	if (NumUdevices)
		perc_freefail_U = (double)TotalFreeFail_U / NumUdevices;

	//if there is no late patients, this line throughs divide by zero exception
	if (TotalLate)
		AvgLatePenalty = TotalLatePenalty / TotalLate;
}

void Schedular::AddToEarlyList(Patient* patient, int arr_time, int app_time)
{
	//set patient status
	patient->setStatus(EARLY);

	//track when the enqueuing in the EARLY List occurs
	patient->TrackLastChange(arr_time);

	//enqueue them in EarlyPlist according to their priority i.e. app_time
	//which is the time step they shall be dequeued from Early list
	EARLY_patients.enqueue(patient, app_time);

	//in order not to count recovering patients twice
	if (patient->isRescheduled()) { TotalEarly--; }
	TotalEarly++;

}

void Schedular::AddToLateList(Patient* patient, int arr_time, int app_time)
{
	//calculate penalty
	int penalty = (arr_time - app_time) / 2;

	TotalLatePenalty += penalty;
	//set patient status
	patient->setStatus(LATE);

	//track when the enqueuing in the LATE List occurs
	patient->TrackLastChange(arr_time);

	//calculate the time step they shall be dequeued from the LATE list
	int pri = penalty + arr_time;

	LATE_patients.enqueue(patient, pri);

	//in order not to count recovering patients twice
	if (patient->isRescheduled()) { TotalEarly--; }
	TotalLate++;
}

void Schedular :: AddToInterrupted(Patient* InterruptedPatient, int app_time)
{
	//track patient status for later checking
	InterruptedPatient->setStatus(INTERRUPTED);

	//track inserting time in the Interrupted list for waiting time calculation
	InterruptedPatient->TrackLastChange(currentTime);

	//enqueue in the interruptedPatients list
	interruptedPatients.enqueue(InterruptedPatient, app_time);
}

/**
 * @brief Skips waiting and checks for direct treatment eligibility.
 * @param required_treatment Pointer to the treatment.
 * @return Pointer to an available resource, or nullptr if not available.
 */
Resource* Schedular::ReadyResource(Treatment* required_treatment) 
{
	Resource* tempRes = nullptr;
	Patient* WaitingPatient = nullptr;

	Resource* isReady = nullptr;
	TreatmentType type = required_treatment->getType();
	bool WasInterrupted = required_treatment->isInterrupted();

	//this boolean ensures that 1 device only may FreeFail per function call
	bool FailureOccured = false;

	switch (type)
	{
	case E_THERAPY:
		if (!Ewaiting.peek(WaitingPatient))
		{
			while (E_Devices.peek(tempRes))
			{
				//to gain access to free failure functionality, we need to dynamic cast
				Edevice* edev = dynamic_cast<Edevice*>(tempRes);
				if (!WasInterrupted && edev && !FailureOccured && !edev->FreeFail(pFreeFailure))
				{
					E_Devices.dequeue(tempRes);
					MaintenanceE.enqueue(edev, (currentTime + edev->getMaintainanceTime()));
					FailureOccured = true;
					if (!(edev->FailedBefore()))
						TotalFreeFail_E++;
				}
				else
				{
					isReady = tempRes;
					break;
				}
			}
		}
		break;
	case U_THERAPY:
		if (!Uwaiting.peek(WaitingPatient))
		{
			while (U_Devices.peek(tempRes))
			{
				//to gain access to free failure functionality, we need to dynamic cast
				Udevice* udev = dynamic_cast<Udevice*>(tempRes);
				if (!WasInterrupted && udev && !FailureOccured && !udev->FreeFail(pFreeFailure) )
				{
					U_Devices.dequeue(tempRes);
					MaintenanceU.enqueue(udev, (currentTime + udev->getMaintainanceTime()));
					FailureOccured = true;
					if (!(udev->FailedBefore()))
						TotalFreeFail_U++;
				}
				else
				{
					isReady = tempRes;
					break;
				}
			}
		}
		break;
	case X_THERAPY:
		if (!Xwaiting.peek(WaitingPatient))
		{
			X_Devices.peek(isReady);
		}
		break;
	}

	return isReady;
}

//----------------------------------------------------------------------------------
// PUBLIC MEMBER FUNCTION
//----------------------------------------------------------------------------------

Schedular::Schedular() :
	NumEdevices(0),
	NumUdevices(0),
	NumXdevices(0),
	currentTime(0),
	lastArrivalTime(0),
	pCancel(0),
	pResc(0),
	pFreeFailure(0),
	pBusyFailure(0),
	TotalTimeSteps(0),
	TotalPatients(0),
	N_patients(0),
	R_patients(0),
	TotalEarly(0),
	TotalLate(0),
	TotalFreeFail_E(0),
	TotalFreeFail_U(0),
	TotalBusyFail_E(0),
	TotalBusyFail_U(0),
	TotalLatePenalty(0),
	AvgWaitTime_all(0),
	AvgWaitTime_N(0),
	AvgWaitTime_R(0),
	AvgTreatmentTime_all(0),
	AvgTreatmentTime_N(0),
	AvgTreatmentTime_R(0),
	perc_accepted_cancell(0),
	perc_accepted_resch(0),
	perc_early_patients(0),
	perc_late_patients(0),
	AvgLatePenalty(0),
	perc_freefail_E(0),
	perc_freefail_U(0),
	perc_Busyfail_E(0),
	perc_Busyfail_U(0)
{}


void Schedular::setCurrentTime(int currtime)
{
	currentTime = (currtime > 0) ? currtime : 0;
}

void Schedular::IncrementTime()
{
	currentTime++;
}

void Schedular::setpCancel(int p_cancel)
{
	pCancel = (p_cancel <= 100 && p_cancel >= 0) ? p_cancel : 0;
}

void Schedular::setpResch(int p_resch)
{
	pResc = (p_resch <= 100 && p_resch >= 0) ? p_resch : 0;
}

void Schedular :: setpFreeFail(int pFF)
{
	pFreeFailure = (pFF <= 100 && pFF >= 0) ? pFF : 0;
}

void Schedular:: setpBusyFail(int pBF)
{
	pBusyFailure = (pBF <= 100 && pBF >= 0) ? pBF : 0;
}

void Schedular::AddToAllList(Patient* myPatient)
{
	if (!myPatient)
		return;

	lastArrivalTime = myPatient->getArrivalTime();

	history.enqueue(myPatient);

	if (!myPatient->isRescheduled())
	{
		ALL_patients.enqueue(myPatient);
		if (myPatient->getType() == NORMAL)
		{
			N_patients++;
		}
		else
		{
			R_patients++;
		}

		TotalPatients++;
	}
	else //insert them in all list sorted by arrival time  (handling rescheduled patients)
	{
		LinkedQueue<Patient*> temp;
		Patient* tempPatient = nullptr;
		bool inserted = false;

		// Move patients from ALL_patients to temp, inserting myPatient at the right place
		while (ALL_patients.dequeue(tempPatient))
		{
			if (!inserted && myPatient->getArrivalTime() < tempPatient->getArrivalTime())
			{
				temp.enqueue(myPatient); // Insert before curr
				inserted = true;
			}

			temp.enqueue(tempPatient);
		}

		// If myPatient has the latest arrival time, insert it at the end
		if (!inserted)
			temp.enqueue(myPatient);

		// Copy back to AllPatients
		while (temp.dequeue(tempPatient))
		{
			ALL_patients.enqueue(tempPatient);
		}
	}
}

void Schedular::AddResource(Resource* resource)
{
	if (!resource)
		return;

	ResType type = resource->getType();

	switch (type)
	{
	case _Edevice:
		E_Devices.enqueue(resource);
		break;
	case _Udevice:
		U_Devices.enqueue(resource);
		break;
	case _Xdevice:
		X_Devices.enqueue(resource);
		break;
	}
}

void Schedular::UpdateNumberResources()
{
	NumEdevices = E_Devices.getCount();
	NumUdevices = U_Devices.getCount();
	NumXdevices = X_Devices.getCount();

}

void Schedular::CheckAllPatientList()
{
	Patient* myPatient = nullptr;

	while (ALL_patients.peek(myPatient))
	{
		int arr_time = myPatient->getArrivalTime();
		int app_time = myPatient->getAppointmentTime();

		if (currentTime >= arr_time)
		{
			ALL_patients.dequeue(myPatient);
			if (arr_time <= app_time) // Early
			{
				AddToEarlyList(myPatient, arr_time, app_time);
			}
			else // Late
			{
				AddToLateList(myPatient, arr_time, app_time);
			}
		}
		else
		{
			//patient hasn't arrived yet, enough checking on the all Patients list
			return;
		}
	}
}

void  Schedular::CheckInterrupted()
{
	Patient* myPatient = nullptr;
	int app_time = 0;

	while (interruptedPatients.peek(myPatient, app_time))
	{
		Treatment* treatment = myPatient->getNextTreatment();
		Resource* available = ReadyResource(treatment);

		if (available)
		{
			interruptedPatients.dequeue(myPatient, app_time);
			AddToTreatmentList(myPatient, available);
		}
		else
		{
			break;
		}
	}
}

void  Schedular::CheckMaintainanceList()
{
	Resource* tempRes = nullptr;

	int maintainanceFinishTime = 0;
	while (MaintenanceE.peek(tempRes, maintainanceFinishTime))
	{
		if (maintainanceFinishTime == currentTime)
		{
			MaintenanceE.dequeue(tempRes, maintainanceFinishTime);
			AddResource(tempRes);
		}
		else
		{
			break;
		}
	}

	while (MaintenanceU.peek(tempRes, maintainanceFinishTime))
	{
		if (maintainanceFinishTime == currentTime)
		{
			MaintenanceU.dequeue(tempRes, maintainanceFinishTime);
			AddResource(tempRes);
		}
		else
		{
			break;
		}
	}
}

void Schedular::EarlyToWait()
{
	Patient* myPatient = nullptr;
	int TreatmentStartTimeStep;

	while (EARLY_patients.peek(myPatient, TreatmentStartTimeStep))
	{
		//check on current time step before processing patient
		//we peek first if to check on time, to avoid the case the time is not the next patient time step
		//in which case we dequeued them early (wrong)

		if (currentTime >= TreatmentStartTimeStep)
		{
			EARLY_patients.dequeue(myPatient, TreatmentStartTimeStep);

			//handle the recovering patient
			if (myPatient->getType() == RECOVERING)
			{
				_handle_recovering_patient(myPatient);
			}

			//calculate waiting time in Early list
			myPatient->addWaitingTime(currentTime - myPatient->getLastChange());

			//track when the patient leaves the early list
			myPatient->TrackLastChange(currentTime);

			//get pointer to next treatment
			Treatment* myTreatment = myPatient->getNextTreatment();

			//get pointer to available resource/dequeue it from its devices list
			Resource* AvailableResource = ReadyResource(myTreatment);

			//a chance to skip the waiting list and go directly to the treatment list
			if (AvailableResource)
			{
				/*
				SPECIAL CASE : 2 patients, 1 in early , 1 or more in late list
				they leave the lists at the same time
				to ensure consistant logic, the late patient always has the priority to be transfered
				the late patient always has the priority to be transfered to the treatment list
				leave the patient in Early list to go to the waitlist
				*/
				Patient* isThereLatePatient;
				int isTimeNow;
				if (LATE_patients.peek(isThereLatePatient, isTimeNow)
					&& isTimeNow == currentTime
					&& isThereLatePatient->getNextTreatment()->getType() == myTreatment->getType())
				{
					AddToEarlyList(myPatient, myPatient->getArrivalTime(), myPatient->getAppointmentTime());
					break;
				}
				else
				{
					AddToTreatmentList(myPatient, AvailableResource);
					continue;
				}
				//if the resource is not nullptr (readily available resource)

			}
			else
			{
				//which Waiting list is determind by which treatment this function is being called on
				//POLYMORPHISM BABY!!!
				myTreatment->MoveToWait(*this, myPatient);
			}
		}
		else //reschedule handling (if a patient wants to reschdule)
		{
			//if a reschedling occurs
			if (_handle_rescheduling_patient())
			{
				continue;
			}
			else
			{
				//if we enter the else, it is not time yet for dequeueing from wait list, 
				return;
			}
		}
	}
}

void Schedular::LateToWait()
{
	Patient* myPatient = nullptr;
	int TreatmentStartTimeStep;

	while (LATE_patients.peek(myPatient, TreatmentStartTimeStep))
	{
		if (currentTime >= TreatmentStartTimeStep)
		{
			LATE_patients.dequeue(myPatient, TreatmentStartTimeStep);

			//handle the recovering patient
			if (myPatient->getType() == RECOVERING)
			{
				_handle_recovering_patient(myPatient);
			}

			//calculate waiting time in Late list
			myPatient->addWaitingTime(currentTime - myPatient->getLastChange());

			//track when the patient leaves the alte list
			myPatient->TrackLastChange(currentTime);

			//get pointer to next treatment
			Treatment* myTreatment = myPatient->getNextTreatment();

			//get pointer to available resource/dequeue it from its devices list
			Resource* AvailableResource = ReadyResource(myTreatment);

			//a chance to skip the waiting list and go directly to the treatment list
			if (AvailableResource)
			{
				AddToTreatmentList(myPatient, AvailableResource);
				continue;
			}
			else
			{
				myTreatment->MoveToWait(*this, myPatient);
			}
		}
		else {   return;   }
	}
}

//*******************************************************//
//next 3 functions are called by Treatment.MoveToWait(..)
bool Schedular::addToEWait(Patient* myPatient)
{
	PatientStatus status = myPatient->getStatus();
	int priority;

	switch (status)
	{
	case EARLY:
		Ewaiting.enqueue(myPatient);
		break;
	case LATE:
		priority = myPatient->getAppointmentTime() + ((myPatient->getArrivalTime() - myPatient->getAppointmentTime()) / 2);
		Ewaiting.insertSorted(myPatient, priority);
		break;
	case SERV:
		priority = myPatient->getAppointmentTime();
		Ewaiting.insertSorted(myPatient, priority);
		break;
	}

	myPatient->setStatus(WAIT);
	return true;
}

bool Schedular::addToUWait(Patient* myPatient)
{
	PatientStatus status = myPatient->getStatus();
	int priority;

	switch (status)
	{
	case EARLY:
		Uwaiting.enqueue(myPatient);
		break;
	case LATE:
		priority = myPatient->getAppointmentTime() + ((myPatient->getArrivalTime() - myPatient->getAppointmentTime()) / 2);
		Uwaiting.insertSorted(myPatient, priority);
		break;
	case SERV:
		priority = myPatient->getAppointmentTime();
		Uwaiting.insertSorted(myPatient, priority);
		break;
	}

	myPatient->setStatus(WAIT);
	return true;
}

bool Schedular::addToXWait(Patient* myPatient)
{
	PatientStatus status = myPatient->getStatus();
	int priority;

	switch (status)
	{
	case EARLY:
		Xwaiting.enqueue(myPatient);
		break;
	case LATE:
		priority = myPatient->getAppointmentTime() + ((myPatient->getArrivalTime() - myPatient->getAppointmentTime()) / 2);
		Xwaiting.insertSorted(myPatient, priority);
		break;
	case SERV:
		priority = myPatient->getAppointmentTime();
		Xwaiting.insertSorted(myPatient, priority);
		break;
	}

	myPatient->setStatus(WAIT);
	return true;
}
//*******************************************************//

//the next 3 functions are called by Treatment :: CanAssign()
bool Schedular::CanAssignE() const  {  return !(E_Devices.isEmpty());  }
bool Schedular::CanAssignU() const  {  return !(U_Devices.isEmpty());  }
bool Schedular::CanAssignX() const  {  return !(X_Devices.isEmpty());  }

//*******************************************************//


void Schedular::WaitToTreatment()
{
	Patient* myPatient = nullptr;
	Resource* ReadyResource = nullptr;

	//for E-treatment patients
	while (Ewaiting.peek(myPatient))
	{
		//check if there were no devices in the list
		if (!E_Devices.peek(ReadyResource))
			break;

		//if we reach this line, the resource is available
		Ewaiting.dequeue(myPatient);

		//add the waiting time in the waitinglist to total wait time of the patient
		myPatient->addWaitingTime(currentTime - myPatient->getLastChange());

		//keep track of last change time stamp
		myPatient->TrackLastChange(currentTime);

		//call to add to proper treatment lists
		AddToTreatmentList(myPatient, ReadyResource);
	}

	//for E-treatment patients
	while (Uwaiting.peek(myPatient))
	{
		//check if there were no devices in the list
		if (!U_Devices.peek(ReadyResource))
			break;

		//if we reach this line, the resource is available
		Uwaiting.dequeue(myPatient);

		//add the waiting time in the waitinglist to total wait time of the patient
		myPatient->addWaitingTime(currentTime - myPatient->getLastChange());

		//keep track of last change time stamp
		myPatient->TrackLastChange(currentTime);

		//call to add to proper treatment lists
		AddToTreatmentList(myPatient, ReadyResource);
	}

	//for X-treatment patients
	while (Xwaiting.peek(myPatient))
	{
		//check if there were no devices in the list
		if (!X_Devices.peek(ReadyResource))
			break;

		if (Xwaiting.RandomCancel(*this))
			continue;

		//if we reach this line, the resource is available
		Xwaiting.dequeue(myPatient);

		//add the waiting time in the waitinglist to total wait time
		myPatient->addWaitingTime(currentTime - myPatient->getLastChange());

		//keep track of last change time stamp
		myPatient->TrackLastChange(currentTime);

		//call to add to proper treatment lists
		AddToTreatmentList(myPatient, ReadyResource);
	}
}

void Schedular::AddToTreatmentList(Patient* myPatient, Resource* myResource)
{
	//get pointer to treatment to be healed
	Treatment* treatment = myPatient->getNextTreatment();

	// 1) process treatment
	// 
	// 1.1) store the pointer of the resource in the treatment
	treatment->assignResource(myResource);

	// 1.2) track assignment time data member
	treatment->setAssignmentTime(currentTime);

	// 2) process resource
	// 
	//dequeue from available devices of the schedular
	if (!myResource->isAvailable())
	{
		ResType type = myResource->getType();

		switch (type)
		{
		case _Edevice:
			E_Devices.dequeue(myResource);
			break;
		case _Udevice:
			U_Devices.dequeue(myResource);
			break;
		case _Xdevice:
			X_Devices.dequeue(myResource);
			break;
		}
	}

	// 3) process patient
	// 
	// 3.1) track patient status data member
	myPatient->setStatus(SERV);

	// 3.2) enqueue in inTreatment List
	int TreatmentDuration = treatment->getDuration();

	inTreatment.enqueue(myPatient, currentTime + TreatmentDuration);
}

void Schedular::MoveToNextTreatment() 
{
	Patient* myPatient = nullptr;
	int Treatment_finishTime;
	Resource* FreeResource = nullptr;

	while (inTreatment.peek(myPatient, Treatment_finishTime))
	{
		//process the patient and its assigned resource
		if (currentTime >= Treatment_finishTime)
		{
			// 1) process resource
			FreeResource = myPatient->getNextTreatment()->getAssignedResource();

			//prepare a pointer with gymRoom functionality
			GymRoom* device = dynamic_cast<GymRoom*>(FreeResource);

			//set its status
			FreeResource->releasePatient();

			ResType type = FreeResource->getType();

			switch (type)
			{
			case _Edevice:
				E_Devices.enqueue(FreeResource);
				break;
			case _Udevice:
				U_Devices.enqueue(FreeResource);
				break;
			case _Xdevice:
				if (device->getAvailableSlots() == 1) 
				//this indicates the room was previously 0 and now only has 1 available slot
				//which indicates it was previously dequeued from the X_Devices
				{
					X_Devices.enqueue(FreeResource);
				}
				break;
			}

			// 2) process current treatment
			myPatient->completeCurrentTreatment(currentTime);

			// 3) process patient
			inTreatment.dequeue(myPatient, Treatment_finishTime);

			// 3.1) SPECIAL CASE : if the patient has no more treatments
			if (IsPatientFinished(myPatient)) continue;

			//handle the recovering patients
			if (myPatient->getType() == RECOVERING)
			{
				_handle_recovering_patient(myPatient);
			}

			// 3.2) get next treatment for the patient
			Treatment* next = myPatient->getNextTreatment();

			next->MoveToWait(*this, myPatient);
		}
		else
		{
			ResType type = _handle_busy_failure();

			switch (type)
			{
			case _Edevice:
				TotalBusyFail_E++;
				break;
			case _Udevice:
				TotalBusyFail_U++;
				break;
			}

			return;
		}
	}
}


bool Schedular::IsPatientFinished(Patient* myPatient) 
{
	if (myPatient->getStatus() == FINISHED)
	{
		Finished_patients.push(myPatient);
		TotalTimeSteps = currentTime;
		myPatient->setFinishTime(currentTime);
		return true;
	}

	return false;
}

int Schedular::getCurrentTime() const { return currentTime; }

int Schedular::getLastArrivalTime() const { return lastArrivalTime; }

int Schedular::getEarlyPatientsCount() const { return EARLY_patients.getCount(); }

int Schedular::getLatePatientsCount() const { return LATE_patients.getCount(); }

int Schedular::getEWaitCount() const { return Ewaiting.getCount(); }

int Schedular::getUWaitCount() const { return Uwaiting.getCount(); }

int Schedular::getXWaitCount() const { return Xwaiting.getCount(); }

int Schedular::getInTreatmentCount() const { return inTreatment.getCount(); }

int Schedular::getInterruptedCount() const { return interruptedPatients.getCount(); }

bool Schedular::isThereMaintainance() const { return (MaintenanceE.getCount() || MaintenanceU.getCount()); }

void Schedular::printResources() const
{
	cout << "\n=========== Resource Counts ===========\n";
	cout << "E-Devices: " << E_Devices.getCount() << endl;
	cout << "U-Devices: " << U_Devices.getCount() << endl;
	cout << "X - Rooms: " << X_Devices.getCount() << endl;
	cout << "=======================================\n";
}

void Schedular::printEarlyPatients() const {  EARLY_patients.printQueue();  }

void Schedular::printLatePatients() const { LATE_patients.printQueue(); }

void Schedular::printEWait() const { Ewaiting.printQueue(); }

void Schedular::printUWait() const { Uwaiting.printQueue(); }

void Schedular::printXWait() const { Xwaiting.printQueue(); }

void Schedular::printInTreatment() const { inTreatment.printQueue(); }

void Schedular::printInterruptedList() const { interruptedPatients.printQueue(); }

void Schedular::printMaintainance() const 
{
	if (!MaintenanceE.isEmpty())
	{
		MaintenanceE.printQueue();	
	}
	if (!MaintenanceU.isEmpty())
	{
		MaintenanceU.printQueue(); 
	}
}

int Schedular::getpCancel() const { return pCancel; }

int Schedular::getpResc() const { return pResc; }

int Schedular::getTotalPatients() const { return TotalPatients; }

int Schedular::getTotalResources() const 
{
	return E_Devices.getCount() + U_Devices.getCount() + X_Devices.getCount(); 
}

int Schedular::getTotalTimeSteps() const { return TotalTimeSteps; }

int Schedular::getTotal_N_Patient() const { return N_patients; }

int Schedular::getTotal_R_Patient() const { return R_patients; }

int Schedular::getTotal_BusyFail_E() const { return TotalBusyFail_E;  }

int Schedular::getTotal_BusyFail_U() const { return TotalBusyFail_U;  }

double Schedular::get_AvgWait() const { return AvgWaitTime_all; }

double Schedular::get_AvgWait_N() const { return AvgWaitTime_N; }

double Schedular::get_AvgWait_R() const { return AvgWaitTime_R; }

double Schedular::get_AvgTimeTreatment() const { return AvgTreatmentTime_all; }

double Schedular::get_AvgTimeTreatment_N() const { return AvgTreatmentTime_N; }

double Schedular::get_AvgTimeTreatment_R() const { return AvgTreatmentTime_R; }

double Schedular::get_Perc_Accepted_Cancel() const { return perc_accepted_cancell; }

double Schedular::get_Perc_Accepted_Resch() const { return perc_accepted_resch; }

double Schedular::get_Perc_Early_Patients() const { return perc_early_patients; }

double Schedular::get_Perc_Late_Patients() const { return perc_late_patients; }

double Schedular::get_Avg_Late_Penalty() const { return AvgLatePenalty; }

double Schedular::get_Perc_FreeFail_E() const { return perc_freefail_E; }

double Schedular::get_Perc_FreeFail_U() const { return perc_freefail_U; }

bool Schedular::SimulationEnded() 
{ 
	if (TotalPatients == Finished_patients.getCount() && MaintenanceE.isEmpty() && MaintenanceU.isEmpty())
	{
		CalcPatientStatistics();
		return true;
	}
	return false;
}

LinkedQueue<Patient*> Schedular::getHistory() const { return history;  }

priQueue <Resource*>  Schedular::getE_MaintainanceList() const { return MaintenanceE; }
priQueue <Resource*>  Schedular::getU_MaintainanceList() const { return MaintenanceU; }

ArrayStack<Patient*> Schedular::getFinishedPatients() const { return Finished_patients; }

LinkedQueue<Patient*> Schedular::getAllPatientList() const { return ALL_patients; }

Schedular::~Schedular() {}
