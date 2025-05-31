#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
class Patient;
class EU_WaitList : public LinkedQueue<Patient*>
{
private:

public:
	//for adding late patients to their waiting list
	//according to PT+penalty as their PT
	void insertSorted(Patient* pPatient, int priority);

	//the arguments to this functions are
	//1) required_treatment : dequeeud from the patients treatment list, used to search the intreatment list
	//2) current time step : to calculate total latency
	//3) inTreatmentList : traversed for a specific type of treatment
	//4) waiting_list : traversed to calculate the waiting list time relative to current time
	int CalcTreatmentLatency(TreatmentType type, int currentTime, priQueue<Patient*> inTreatmentList);

};