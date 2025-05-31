#include "Output.h"
#include "Schedular.h"
#include "Patient.h"

Output::Output(string fileName) : _fileName(fileName) {}

void Output::GenerateOutputFile(const std::string& filename, const Schedular& mySchedular)
{
    std::ofstream outFile(filename);
    if (!outFile)
    {
        std::cerr << "Error: Could not open the output file!" << std::endl;
        return;
    }

    outFile << "PID  PType  PT  VT  FT  WT  TT  Cancel  Resc" << std::endl;

    // Extract finished patients from the scheduler
    ArrayStack<Patient*> finishedPatients = mySchedular.getFinishedPatients();
    Patient* p = nullptr;
    while (!finishedPatients.isEmpty())
    {
        finishedPatients.pop(p);
        outFile << p->getPID() << "    "
            << (p->getType() == NORMAL ? "N" : "R") << "    "
            << p->getAppointmentTime() << "   "
            << p->getArrivalTime() << "   "
            << p->getFinishTime() << "   "
            << p->getTotalWaitingTime() << "    "
            << p->getTotalTreatmentTime() << "    "
            << (p->isCancelled() ? "T" : "F") << "      "
            << (p->isRescheduled() ? "T" : "F") << std::endl;
    }

    // Write statistics
    outFile << "\nTotal number of timesteps = " << mySchedular.getTotalTimeSteps() << std::endl;
    outFile << "Total number of all, N, and R patients = " << mySchedular.getTotalPatients() << ", "
        << mySchedular.getTotal_N_Patient() << ", " << mySchedular.getTotal_R_Patient() << std::endl;
    outFile << "Average total waiting time for all, N, and R patients = " << mySchedular.get_AvgWait() << ", "
        << mySchedular.get_AvgWait_N() << ", " << mySchedular.get_AvgWait_R() << std::endl;
    outFile << "Average total treatment time for all, N, and R patients = " << mySchedular.get_AvgTimeTreatment() << ", "
        << mySchedular.get_AvgTimeTreatment_N() << ", " << mySchedular.get_AvgTimeTreatment_R() << std::endl;
    outFile << "Percentage of patients of an accepted cancellation (%) = " << mySchedular.get_Perc_Accepted_Cancel() * 100 << " %" << std::endl;
    outFile << "Percentage of patients of an accepted rescheduling (%) = " << mySchedular.get_Perc_Accepted_Resch() * 100 << " %" << std::endl;
    outFile << "Percentage of early patients (%) = " << mySchedular.get_Perc_Early_Patients() * 100 << " %" << std::endl;
    outFile << "Percentage of late patients (%) = " << mySchedular.get_Perc_Late_Patients() * 100 << " %" << std::endl;
    outFile << "Average late penalty = " << mySchedular.get_Avg_Late_Penalty() << " timestep(s)" << std::endl;
    outFile << "Percentage of Free Electro Device Failed (%) = " << mySchedular.get_Perc_FreeFail_E() * 100 << " %" << std::endl;
    outFile << "Percentage of Free UltraSonic Device Failed (%) = " << mySchedular.get_Perc_FreeFail_U() * 100 << " %" << std::endl;
    outFile << "Total number of Busy failures for Electro Devices = " << mySchedular.getTotal_BusyFail_E() << std::endl;
    outFile << "Total number of Busy failures for UltraSonic Devices = " << mySchedular.getTotal_BusyFail_U() << std::endl;

    outFile.close();
}
