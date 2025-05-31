/**
 * @file Output.cpp
 * @brief Handles generation of formatted output files summarizing simulation results.
 *
 * The Output class is responsible for producing a detailed output file containing
 * patient information and overall simulation statistics. It formats patient data
 * such as appointment, arrival, treatment, waiting times, and cancellation/rescheduling status.
 * Additionally, it reports aggregate metrics like average waiting and treatment times,
 * percentages of cancellations, rescheduling, failures, and other system-wide statistics.
 *
 * This output supports clear, aligned tabular presentation to ease analysis of simulation results.
 *
 * @author Ibrahim Mohsen
 * @date March 2025
 */
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
