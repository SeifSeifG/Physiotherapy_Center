#pragma once
#include "priQueue.h"
#include "Patient.h"

class TreatmentList : public priQueue <Patient*>
{
private:


public:
	Patient* RandomBusyFailure(int pBusyFailure, int currentTime);
};