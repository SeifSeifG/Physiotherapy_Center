#include "UI.h"


void UI::DisplayAllPatients(const Schedular& mySchedular)
{
	cout << "=========== ALL Patients Lists ===========\n";
	LinkedQueue <Patient*> all_patients = mySchedular.getAllPatientList();
	Patient* myPatient = nullptr;

	while (!all_patients.isEmpty())
	{
		all_patients.dequeue(myPatient);

		cout << *myPatient;
	}

	cout << endl;
}

void UI::DisplaySystemStatus(const Schedular& mySchedular)
{
	std::cout << "\n===== System Status at Time Step " << mySchedular.getCurrentTime() << " =====\n";
	std::cout << "Total Patients Processed: " << mySchedular.getTotalPatients() << "\n";
	std::cout << "Total Resources in Use: " << mySchedular.getTotalResources() << "\n";

	std::cout << "\n=== Patients Lists ===\n";

	std::cout << "Early Patients (" << mySchedular.getEarlyPatientsCount() << "):\n";
	mySchedular.printEarlyPatients();

	std::cout << "Late Patients (" << mySchedular.getLatePatientsCount() << "):\n";
	mySchedular.printLatePatients();

	std::cout << "E-Treatment Waiting List (" << mySchedular.getEWaitCount() << "):\n";
	mySchedular.printEWait();

	std::cout << "U-Treatment Waiting List (" << mySchedular.getUWaitCount() << "):\n";
	mySchedular.printUWait();

	std::cout << "X-Treatment Waiting List (" << mySchedular.getXWaitCount() << "):\n";
	mySchedular.printXWait();

	std::cout << "In Treatment (" << mySchedular.getInTreatmentCount() << "):\n";
	mySchedular.printInTreatment();

	//std::cout << "Interrupted Patients (" << mySchedular.getInterruptedCount() << "):\n";
	//mySchedular.printInterruptedList();

	if (mySchedular.isThereMaintainance())
	{
		std::cout << "Devices in Maintainance : \n";
		mySchedular.printMaintainance();
	}



}

void UI::displayFinalPatient(const Schedular& mySchedular)
{
	ArrayStack<Patient* >copy = mySchedular.getFinishedPatients();

	Patient* myPatient = nullptr;
	while (copy.pop(myPatient))
	{
		// ====================== Final Status Report ======================
		std::cout << "\n======= Treatment Completion Report =======\n";
		int treatment_number = myPatient->getNumber_RemainingTreatment();

		cout << "Patient " << myPatient->getPID()
			<< " | Finish: " << myPatient->getFinishTime()
			<< " | Wait: " << myPatient->getTotalWaitingTime()
			<< " | Treatment: " << myPatient->getTotalTreatmentTime()
			<< " | Status: " << (myPatient->isFinished() ? "COMPLETE" : "IN PROGRESS")
			<< " | Cancelled: " << (myPatient->isCancelled() ? "T" : "F")
			<< " | Rescheduled: " << (myPatient->isRescheduled() ? "T" : "F");

	}
}

void UI::displayHistory(const Schedular& mySchedular)
{
	LinkedQueue<Patient*> history = mySchedular.getHistory(); // or getHistory(), depending on implementation
	Patient* p = nullptr;

	cout << "---------------- Patient History ----------------" << endl;
	cout << "PID | Type | Status   | PT  | VT  | FT  | Cancel | Resched" << endl;
	cout << "-------------------------------------------------" << endl;

	while (!history.isEmpty())
	{
		history.dequeue(p);
		if (!p) continue;

		string typeStr = (p->getType() == NORMAL) ? "N" : "R";

		string statusStr;
		switch (p->getStatus())
		{
		case EARLY:    statusStr = "EARLY"; break;
		case LATE:     statusStr = "LATE"; break;
		case WAIT:     statusStr = "WAIT"; break;
		case SERV:     statusStr = "SERVING"; break;
		case FINISHED: statusStr = "FINISHED"; break;
		case IDLE:     statusStr = "IDLE"; break;
		default:       statusStr = "UNKNOWN"; break;
		}

		cout << p->getPID() << "   | "
			<< typeStr << " | "
			<< statusStr << " | "
			<< p->getAppointmentTime() << "  | "
			<< p->getArrivalTime() << "  | "
			<< p->getFinishTime() << "  | "
			<< (p->isCancelled() ? "T" : "F") << "      | "
			<< (p->isRescheduled() ? "T" : "F") << endl;
		if (p->getStatus() != FINISHED)
		{
			p->getRemainingTreatments().printQueue();
		}
	}

	cout << "-------------------------------------------------" << endl;
}


void UI::DisplayResources(const Schedular& mySchedular)
{
	mySchedular.printResources();
}

//for Patient
ostream& operator<<(ostream& os, Patient& myPatient)
{
	int treatment_number = myPatient.Required_Treatments.getCount();

	os << "[Patient " << myPatient.PID
		<< ", PT: " << myPatient.PT
		<< ", VT: " << myPatient.VT
		<< ", Type: " << (myPatient.pType == NORMAL ? "NORMAL" : "RECOVERING")
		<< ", Treatments: " << treatment_number
		<< ", Cancelled: " << (myPatient.cancelled ? "T" : "F")
		<< ", Rescheduled: " << (myPatient.rescheduled ? "T" : "F")
		<< "]\n";

	return os;
}

//for a resource
ostream& operator<<(ostream& os, const Resource& r)
{
	//function print is a virtual function, determines which resource type this is
	r.print(os);
	return os;
}

//for treatment
ostream& operator<<(ostream& os, const Treatment& t)
{
	t.print(os);
	return os;
}

