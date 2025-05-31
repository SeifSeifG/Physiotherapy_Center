#include "U_Treatment.h"
#include "Schedular.h"  // Full definition of Schedular

U_treatment::U_treatment() : Treatment() { type = U_THERAPY; }

U_treatment::U_treatment(int d) : Treatment(d, U_THERAPY) { type = U_THERAPY; }

bool U_treatment::MoveToWait(Schedular& scheduler, Patient* patient)
{
    return scheduler.addToUWait(patient);
}

bool U_treatment::CanAssign(Schedular& schedular)
{
    return schedular.CanAssignU();
}

U_treatment::~U_treatment() { }

void U_treatment::print(ostream& os) const
{
    os << "U-Treatment with duration: " << duration << endl;
}
