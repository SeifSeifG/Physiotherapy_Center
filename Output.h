#pragma once
#ifndef OUTPUT
#define OUTPUT

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "LinkedQueue.h"

class Patient;
class E_device;
class U_device;
class Gym_Room;
class Schedular;

class Output
{
	string _fileName;
public:
    Output(string fileName);
    void GenerateOutputFile(const std::string& filename, const Schedular& mySchedular);
};


#endif // !OUTPUT
