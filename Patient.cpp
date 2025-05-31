#include "Patient.h"

// Static counter for assigning unique IDs
static int ID = 100;

//----------------------------------- CONSTRUCTORS -----------------------------------//

// Default constructor: Initializes an empty patient with default values
Patient::Patient()
    : PID(++ID), PT(-1), VT(-1),
    pType(DUMMY_PTYPE), status(IDLE), LastChangeTimeStamp(0),
    cancelled(false), rescheduled(false),
    totalWaitingTime(0), totalTreatmentTime(0), finishTime(-1) {}

// Constructor with patient type, appointment time, and arrival time
Patient::Patient(int pt, int vt, PatientType type) :
    LastChangeTimeStamp(0), cancelled(false), rescheduled(false), totalWaitingTime(0), totalTreatmentTime(0), finishTime(-1)
{
    PID = ++ID;
    pType = type;
    PT = pt;
    VT = vt;
    status = (vt < pt) ? EARLY : LATE;
}

// Constructor with specific patient ID
Patient::Patient(int id, int pt, int vt, PatientType type) :
    LastChangeTimeStamp(0), cancelled(false), rescheduled(false), totalWaitingTime(0), totalTreatmentTime(0), finishTime(-1)
{
    PID = id;
    pType = type;
    PT = pt;
    VT = vt;
    status = (vt < pt) ? EARLY : LATE;
}

// Constructor with predefined treatments queue
Patient::Patient(int id, int pt, int vt, PatientType type, LinkedQueue<Treatment*> req_treat):
    LastChangeTimeStamp(0), cancelled(false), rescheduled(false), totalWaitingTime(0), totalTreatmentTime(0), finishTime(-1)
{
    PID = id;
    pType = type;
    PT = pt;
    VT = vt;
    Required_Treatments = req_treat;
    status = (vt < pt) ? EARLY : LATE;
}

//----------------------------------- TREATMENT HANDLING -----------------------------------//

// Adds a treatment if not already added and maximum not reached
bool Patient::addTreatment(Treatment* newTreatment)
{
    if (!newTreatment || Required_Treatments.getCount() == 3)
        return false;

    if (E_treatment* e = dynamic_cast<E_treatment*>(newTreatment))
    {
        return addTreatmentHelper<E_treatment>(e);
    }
    else if (U_treatment* u = dynamic_cast<U_treatment*>(newTreatment))
    {
        return addTreatmentHelper<U_treatment>(u);
    }
    else if (X_treatment* x = dynamic_cast<X_treatment*>(newTreatment))
    {
        return addTreatmentHelper<X_treatment>(x);
    }

    return false;
}

//----------------------------------- SETTERS -----------------------------------//

void Patient::setStatus(PatientStatus newStatus)
{
    if (status != FINISHED)
        status = newStatus;
}

void Patient::setFinishTime(int time)
{
    finishTime = time;
}

void Patient::TrackLastChange(int time)
{
    LastChangeTimeStamp = time;
}

void Patient::setPT(int pt)
{
    PT = pt;
}

void Patient::setCancelled(bool is)
{
    cancelled = is;
}

//----------------------------------- TIME TRACKING -----------------------------------//

void Patient::addWaitingTime(int timesteps)
{
    totalWaitingTime += (timesteps > 0) ? timesteps : 0;
}

void Patient::addTreatmentTime(int timesteps)
{
    totalTreatmentTime += (timesteps > 0) ? timesteps : 0;
}

//----------------------------------- TREATMENT MANAGEMENT -----------------------------------//

Treatment* Patient::getNextTreatment() const
{
    Treatment* t = nullptr;
    Required_Treatments.peek(t);
    return t; // Returns nullptr if no treatment is available
}

bool Patient::completeCurrentTreatment(int CurrentTime)
{
    Treatment* treatment = nullptr;
    if (Required_Treatments.dequeue(treatment))
    {
        treatment->setFinishTime(CurrentTime);
        treatment->FreeResource();

        totalTreatmentTime += treatment->getDuration();

        Treatment* Nexttreatment = nullptr;
        Required_Treatments.peek(Nexttreatment);

        if (!Nexttreatment)
            status = FINISHED;
    }

    return treatment;
}

//----------------------------------- GETTERS -----------------------------------//

int Patient::getPID() const { return PID; }
PatientType Patient::getType() const { return pType; }
int Patient::getAppointmentTime() const { return PT; }
int Patient::getArrivalTime() const { return VT; }

LinkedQueue<Treatment*> Patient::getRemainingTreatments() const 
{ 
    return Required_Treatments; 
}

int Patient::getNumber_RemainingTreatment() const
{
    return Required_Treatments.getCount();
}

PatientStatus Patient::getStatus() const { return status; }
int Patient::getLastChange() const { return LastChangeTimeStamp; }
int Patient::getTotalWaitingTime() const { return totalWaitingTime; }
int Patient::getTotalTreatmentTime() const { return totalTreatmentTime; }
int Patient::getFinishTime() const { return finishTime; }
bool Patient::isCancelled() const { return cancelled; }
bool Patient::isRescheduled() const { return rescheduled; }
bool Patient::isFinished() const { return (status == FINISHED); }

//----------------------------------- SPECIAL CASES -----------------------------------//

void Patient::markCancelled()
{
    if (status == WAIT && !Required_Treatments.isEmpty())
    {
        cancelled = true;
        status = FINISHED;
    }
}

void Patient::markRescheduled(int newPT, int newVT)
{
    if (status == EARLY && newPT >= PT && newVT >= VT)
    {
        PT = newPT;
        VT = newVT;
        rescheduled = true;
    }
}

void Patient::ResortTreatments(TreatmentType Ttype)
{
    if (pType != RECOVERING)
        return;

    LinkedQueue<Treatment*> temp;
    Treatment* _priority_treatment = nullptr;

    while (Required_Treatments.peek(_priority_treatment))
    {
        TreatmentType tempType = _priority_treatment->getType();
        if (tempType == Ttype)
            break;

        Required_Treatments.dequeue(_priority_treatment);
        temp.enqueue(_priority_treatment);
    }

    while (!temp.isEmpty())
    {
        temp.dequeue(_priority_treatment);
        Required_Treatments.enqueue(_priority_treatment);
    }
}

//----------------------------------- DESTRUCTOR -----------------------------------//

Patient::~Patient() {}