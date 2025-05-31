#ifndef LOAD_H
#define LOAD_H
#include <fstream>
#include <string>
using namespace std;
#include "LinkedQueue.h"

//some macros for generating a random patient
#define NEW_PT_RANGE 5
#define NEW_VT_RANGE 5
#define NEW_TREATMENT_DURATION_RANGE 5
#define MAX_PROBABILITY_E 33
#define MAX_PROBABILITY_U 66
#define MAX_PROBABILITY_X 100

class Patient;
class E_device;
class U_device;
class Gym_Room;
class Schedular;
class Load
{
    string _fileName;

public:
    Load(string fileName);
    void LoadDataFromFile(Schedular& hospitalSystem);
    bool GenerateRandomPatient(Schedular& hospitalSystem);
};

#endif