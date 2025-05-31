#include "Load.h"
#include <assert.h>
#include <iostream>
#include "Schedular.h"

Load::Load(string fileName) : _fileName(fileName)
{
    // Constructor
}

void Load::LoadDataFromFile(Schedular& hospitalSystem)
{
    ifstream l_inFile(_fileName + ".txt", ios::in);
    if (!l_inFile.is_open()) 
    {
        cout << "Error: Unable to open file " << _fileName << ".txt" << endl;
        return;
    }

    // Step 1: Read the number of resources
    int numEDevices = 0, numUDevices = 0, numXRooms = 0;
    if (!(l_inFile >> numEDevices >> numUDevices >> numXRooms)) 
    {
        cout << "Error: Failed to read device counts." << endl;
        return;
    }

    // Add E-therapy devices
    for (int i = 0; i < numEDevices; i++) {
        hospitalSystem.AddResource(new Edevice);
    }

    // Add U-therapy devices
    for (int i = 0; i < numUDevices; i++) {
        hospitalSystem.AddResource(new Udevice);
    }

    // Add X-therapy gym rooms
    for (int i = 0; i < numXRooms; i++) {
        int capacity;
        if (!(l_inFile >> capacity)) {
            cout << "Error: Failed to read gym room capacity." << endl;
            return;
        }
        hospitalSystem.AddResource(new GymRoom(capacity));
    }

    hospitalSystem.UpdateNumberResources();

    // Step 2: Read probabilities
    int Pcancel = 0, Presc = 0, pFF = 0, pBF = 0;
    if (!(l_inFile >> Pcancel >> Presc >> pFF >> pBF)) 
    {
        cout << "Error: Failed to read probabilities." << endl;
        return;
    }

    hospitalSystem.setpCancel(Pcancel);
    hospitalSystem.setpResch(Presc);
    hospitalSystem.setpFreeFail(pFF);
    hospitalSystem.setpBusyFail(pBF);

    // Step 3: Read the number of patients
    int numPatients = 0;
    if (!(l_inFile >> numPatients)) 
    {
        cout << "Error: Failed to read the number of patients." << endl;
        return;
    }

    // Step 4: Read patient details
    for (int i = 0; i < numPatients; i++) {
        char patientTypeChar;
        int arrivalTime, appointmentTime, numTreatments;    

        if (!(l_inFile >> patientTypeChar >> appointmentTime >> arrivalTime >> numTreatments)) 
        {
            cout << "Error: Failed to read patient data at entry " << i + 1 << endl;
            return;
        }

        PatientType patientType = (patientTypeChar == 'N') ? NORMAL : RECOVERING;

        // Create the patient object
        Patient* patient = new Patient(appointmentTime, arrivalTime, patientType);

        // Read treatments
        for (int j = 0; j < numTreatments; j++) 
        {
            char treatmentType;
            int duration;
            if (!(l_inFile >> treatmentType >> duration)) {
                cout << "Error: Failed to read treatment data for patient " << patient->getPID() + 1 << endl;
                return;
            }

            if (treatmentType == 'E')
                patient->addTreatment(new E_treatment(duration));
            else if (treatmentType == 'U')
                patient->addTreatment(new U_treatment(duration));
            else if (treatmentType == 'X')
                patient->addTreatment(new X_treatment(duration));
        }

        // Add patient to system
        hospitalSystem.AddToAllList(patient);
    }

    l_inFile.close();
}


bool Load::GenerateRandomPatient(Schedular& hospitalSystem)
{
    int _generate_patient = rand() % 100;

    if (_generate_patient >= 70)
    {
        //generate a random time after time t
        int _last_arrival_time = hospitalSystem.getLastArrivalTime();

        int _curr_time = hospitalSystem.getCurrentTime();

        int _rand_PT = 0;
        int _rand_VT = 0;

        //random appointment time
        if (_curr_time < _last_arrival_time)
        {
            _rand_PT = _last_arrival_time + rand() % NEW_PT_RANGE;

            _rand_VT = _last_arrival_time + rand() % NEW_VT_RANGE;
        }
        else
        {
            _rand_PT = _curr_time + rand() % NEW_PT_RANGE;
            if (_rand_PT == _curr_time)
                _rand_PT++;

            _rand_VT = _curr_time + rand() % NEW_VT_RANGE;
            if (_rand_VT == _curr_time)
                _rand_VT++;

            if (_rand_PT == _rand_VT)
                _rand_PT++;
        }

        //if the rand interger is an integer number 0, condition is false, Normal patient is generated
        //I casted the rand output into int because I need to check on aboslute zero or one
        PatientType _rand_type = ((int)(rand() % 2)) ? RECOVERING : NORMAL;

        Patient* myPatient = new Patient(_rand_PT, _rand_VT, _rand_type);

        assert(myPatient && "No more memory for extra patient");

        int _add_Etreatment = rand() % 100;
        int _add_Utreatment = rand() % 100;
        int _add_Xtreatment = rand() % 100;

        if (_add_Etreatment >= 0 && _add_Etreatment < MAX_PROBABILITY_E)
        {
            int _rand_treatment_time = rand() % NEW_TREATMENT_DURATION_RANGE;
            if (_rand_treatment_time != 0)
            {
                myPatient->addTreatment(new E_treatment(_rand_treatment_time));
            }
        }

        if (_add_Utreatment >= MAX_PROBABILITY_E && _add_Utreatment < MAX_PROBABILITY_U)
        {
            int _rand_treatment_time = rand() % NEW_TREATMENT_DURATION_RANGE;
            if (_rand_treatment_time != 0)
            {
                myPatient->addTreatment(new U_treatment(_rand_treatment_time));
            }
        }

        if (_add_Xtreatment >= MAX_PROBABILITY_U && _add_Xtreatment < MAX_PROBABILITY_X)
        {
            int _rand_treatment_time = rand() % NEW_TREATMENT_DURATION_RANGE;
            if (_rand_treatment_time != 0)
            {
                myPatient->addTreatment(new X_treatment(_rand_treatment_time));
            }
        }

        if (myPatient->getNextTreatment())
        {
            hospitalSystem.AddToAllList(myPatient);
            cout << "Random Genearation Successful, new Patient data :" << endl;
            cout << *myPatient << endl;
            return true;
        }
        else
        {
            //patient with no treatments
            return false;
        }
    }
    else
    {
        return false;
    }
}