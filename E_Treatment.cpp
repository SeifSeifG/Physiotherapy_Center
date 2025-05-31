#include "E_Treatment.h"
#include "Schedular.h"  // Full definition of Schedular now available

E_treatment::E_treatment() : Treatment() { type = U_THERAPY; }

E_treatment::E_treatment(int d) : Treatment(d, E_THERAPY) { type = E_THERAPY;}

bool E_treatment::MoveToWait(Schedular& scheduler, Patient* patient) 
{
	return scheduler.addToEWait(patient);
}

bool E_treatment::CanAssign(Schedular& schedular)
{
	return schedular.CanAssignE();
}


E_treatment::~E_treatment() {}

void E_treatment::print(ostream& os) const
{
    os << "E-Treatment with duration: " << duration << endl;
}
