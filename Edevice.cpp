#include "Edevice.h"
static int ID = 200; // Initialize static member

Edevice::Edevice() : 
    Resource(_Edevice), 
    Working(true), 
    maintainanceTime(0), 
    MaintainanceCooldown(0),
    failedOnce(false)
{
    id = ++ID;
}

bool Edevice::FreeFail(int pFF)
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

bool Edevice::BusyFail()
{
    Working = false;
    maintainanceTime = (rand() % MAINTAINANCE_DURATION) + 1;
    MaintainanceCooldown = MAINTAINANCE_COOLDOWN;
    return true;
}

void Edevice::Repair()
{
    Working = true;
}


int Edevice::getMaintainanceTime() const
{
    return maintainanceTime;
}

bool Edevice::isWorking() const
{
    return Working;
}

bool Edevice :: FailedBefore()
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

void Edevice::setMaintainanceTime(int time)
{
    maintainanceTime = time;
}

void Edevice::print(std::ostream& os) const
{
    os << "Edevice id : " << id;
}
