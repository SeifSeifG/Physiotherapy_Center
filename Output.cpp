#include "Output.h"
#include "Schedular.h"
#include "Patient.h"
#include <iomanip>  // For std::setw, std::left to prevent skewd output file

Output::Output(string fileName) : _fileName(fileName) {}

void Output::GenerateOutputFile(const string& filename, const Schedular& mySchedular)
{
    ofstream outFile(filename);
    if (!outFile)
    {
        cerr << "Error: Could not open the output file!" << endl;
        return;
    }

    // Header with consistent column widths
    outFile << left << setw(6) << "PID"
        << setw(8) << "PType"
        << setw(6) << "PT"
        << setw(6) << "VT"
        << setw(6) << "FT"
        << setw(6) << "WT"
        << setw(6) << "TT"
        << setw(9) << "Cancel"
        << setw(6) << "Resc" << endl;

    ArrayStack<Patient*> finishedPatients = mySchedular.getFinishedPatients();
    Patient* p = nullptr;
    while (finishedPatients.pop(p))
    {
        outFile << left << setw(6) << p->getPID()
            << setw(8) << (p->getType() == NORMAL ? "N" : "R")
            << setw(6) << p->getAppointmentTime()
            << setw(6) << p->getArrivalTime()
            << setw(6) << p->getFinishTime()
            << setw(6) << p->getTotalWaitingTime()
            << setw(6) << p->getTotalTreatmentTime()
            << setw(9) << (p->isCancelled() ? "T" : "F")
            << setw(6) << (p->isRescheduled() ? "T" : "F") << endl;
    }

    // Statistics
    outFile << "\nTotal number of timesteps = " << mySchedular.getTotalTimeSteps() << endl;
    outFile << "Total number of all, N, and R patients = " << mySchedular.getTotalPatients() << ", "
        << mySchedular.getTotal_N_Patient() << ", " << mySchedular.getTotal_R_Patient() << endl;

    outFile << "Average total waiting time for all, N, and R patients = "
        << mySchedular.get_AvgWait() << ", "
        << mySchedular.get_AvgWait_N() << ", "
        << mySchedular.get_AvgWait_R() << endl;

    outFile << "Average total treatment time for all, N, and R patients = "
        << mySchedular.get_AvgTimeTreatment() << ", "
        << mySchedular.get_AvgTimeTreatment_N() << ", "
        << mySchedular.get_AvgTimeTreatment_R() << endl;

    outFile << "Percentage of patients of an accepted cancellation (%) = "
        << mySchedular.get_Perc_Accepted_Cancel() * 100 << " %" << endl;

    outFile << "Percentage of patients of an accepted rescheduling (%) = "
        << mySchedular.get_Perc_Accepted_Resch() * 100 << " %" << endl;

    outFile << "Percentage of early patients (%) = "
        << mySchedular.get_Perc_Early_Patients() * 100 << " %" << endl;

    outFile << "Percentage of late patients (%) = "
        << mySchedular.get_Perc_Late_Patients() * 100 << " %" << endl;

    outFile << "Average late penalty = "
        << mySchedular.get_Avg_Late_Penalty() << " timestep(s)" << endl;

    outFile << "Percentage of Free Electro Device Failed (%) = "
        << mySchedular.get_Perc_FreeFail_E() * 100 << " %" << endl;

    outFile << "Percentage of Free UltraSonic Device Failed (%) = "
        << mySchedular.get_Perc_FreeFail_U() * 100 << " %" << endl;

    outFile << "Total number of Busy failures for Electro Devices = "
        << mySchedular.getTotal_BusyFail_E() << endl;

    outFile << "Total number of Busy failures for UltraSonic Devices = "
        << mySchedular.getTotal_BusyFail_U() << endl;

    outFile.close();
}
