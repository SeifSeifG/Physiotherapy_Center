/**
 * @file Load.h
 * @brief Handles loading simulation data from input files and generating random patients.
 *
 * The Load class is responsible for reading resource configurations, patient data,
 * and simulation parameters from input files. It also supports creating randomized
 * patients with treatments and scheduling them within the hospital system.
 *
 * @author Ibrahim Mohsen
 * @date March 2025
 */
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

/**
 * @class Load
 * @brief Responsible for reading simulation data from input files and generating random patients.
 */
class Load
{
private:
    string _fileName; ///< Name of the input file (without extension)

public:
    /**
     * @brief Constructs a Load object with a given input file name.
     * @param fileName Name of the file (excluding the .txt extension)
     */
    Load(string fileName);

    /**
     * @brief Loads data from the specified file into the Schedular system.
     *
     * This includes reading:
     * - Resource counts and capacities (E, U devices, X rooms)
     * - Probability values (cancellation, rescheduling, free failure, busy failure)
     * - Patient details including type, times, and treatment needs
     *
     * @param hospitalSystem Reference to the Schedular object to populate
     */
    void LoadDataFromFile(Schedular& hospitalSystem);

    /**
     * @brief Generates a random patient and adds them to the scheduler system.
     *
     * This includes:
     * - Random appointment and arrival times
     * - Random patient type (NORMAL or RECOVERING)
     * - Randomly assigned treatments (E, U, X)
     *
     * @param hospitalSystem Reference to the Schedular system
     * @return true if a patient was successfully generated and added; false otherwise
     */
    bool GenerateRandomPatient(Schedular& hospitalSystem);
};

#endif