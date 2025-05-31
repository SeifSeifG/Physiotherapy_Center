#pragma once
#include "Treatment.h"

class U_treatment : public Treatment
{
public:
    U_treatment();
    U_treatment(int d);

    virtual bool CanAssign(Schedular& schedular) override;
    virtual bool MoveToWait(Schedular& scheduler, Patient* patient) override;

    virtual ~U_treatment();
    virtual void print(ostream& os) const override;

};
