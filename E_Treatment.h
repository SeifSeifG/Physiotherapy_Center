#pragma once
#include "Treatment.h"

class E_treatment : public Treatment
{
public:
    E_treatment();
    E_treatment(int d);

    virtual bool CanAssign(Schedular& schedular) override;
    virtual bool MoveToWait(Schedular& scheduler, Patient* patient) override;

    virtual ~E_treatment();
    virtual void print(ostream& os) const override;

};
