#include "Udevice.h"
static int ID = 300; // Initialize static member

Udevice::Udevice() : 
    Resource(_Udevice), 
    Working(true), 
    maintainanceTime(0), 
    MaintainanceCooldown(0),
    failedOnce(false)
{
    id = ++ID;
}

bool Udevice :: FreeFail(int pFF)
{
    int random = rand() % 100;
    if (random < pFF)
    {
        if (MaintainanceCooldown == 0)
        {
            Working = false;
            maintainanceTime = (rand() % MAINTAINANCE_DURATION) + 1;
            MaintainanceCooldown = MAINTAINANCE_COOLDOWN;
        }
        else
        {
            MaintainanceCooldown--;
        }
    }

    return Working;
}

bool Udevice::BusyFail()
{
    Working = false;
    maintainanceTime = (rand() % MAINTAINANCE_DURATION) + 1;
    MaintainanceCooldown = MAINTAINANCE_COOLDOWN;
    return true;
}

void Udevice :: Repair()
{
    Working = true;
}

int Udevice::getMaintainanceTime() const
{
    return maintainanceTime;
}

void Udevice::setMaintainanceTime(int time)
{
    maintainanceTime = time;
}

bool Udevice::isWorking() const
{
    return Working;
}

bool Udevice::FailedBefore()
{
    if (failedOnce == false)
    {
        failedOnce = true;
        return false;
    }
    else
    {
        return true;
    }
}


void Udevice::print(std::ostream& os) const 
{
    os << "Udevice id : " << id;
}

