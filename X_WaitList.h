#pragma once
#include "EU_WaitList.h"
class Schedular;

class X_WaitList : public EU_WaitList
{
private :

public:
	bool RandomCancel(Schedular& hospitalsystem);
};