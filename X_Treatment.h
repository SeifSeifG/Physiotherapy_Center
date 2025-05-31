#pragma once
#include "Treatment.h"

class X_treatment : public Treatment
{
public:
    X_treatment();
    X_treatment(int d);

    virtual bool CanAssign(Schedular& schedular) override;
    virtual bool MoveToWait(Schedular& scheduler, Patient* patient) override;

    virtual ~X_treatment();
    virtual void print(ostream& os) const override;

};