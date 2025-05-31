#pragma once
#include "Resource.h"

class Udevice : public Resource
{
private:
       
    // for device failure functionality
    int maintainanceTime;
    bool Working;
    int MaintainanceCooldown;
    bool failedOnce;

public:
    Udevice();

    bool FreeFail(int pFF);

    bool BusyFail();

    void Repair();

    void setMaintainanceTime(int time);

    bool isWorking() const;

    bool FailedBefore();

    int getMaintainanceTime() const;

    void print(std::ostream& os) const override;
};
