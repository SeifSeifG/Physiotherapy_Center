#include "X_Treatment.h"
#include "Schedular.h"  // Full definition of Schedular

X_treatment::X_treatment() : Treatment() { type = X_THERAPY; }

X_treatment::X_treatment(int d) : Treatment(d, X_THERAPY) { type = X_THERAPY; }

bool X_treatment::MoveToWait(Schedular& scheduler, Patient* patient)
{
    return scheduler.addToXWait(patient);
}

bool X_treatment::CanAssign(Schedular& schedular)
{
    return schedular.CanAssignX();
}

X_treatment::~X_treatment() { }

void X_treatment::print(ostream& os) const  
{
    os << "X-Treatment with duration: " << duration << endl;
}
