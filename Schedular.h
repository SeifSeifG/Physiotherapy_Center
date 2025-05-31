#pragma once

#define RANDOM_ARGUMENT ((unsigned int)time(0))

// Lists
#include "LinkedQueue.h"
#include "TreatmentList.h"
#include "ArrayStack.h"
#include "EarlyPlist.h"
#include "EU_WaitList.h"
#include "X_WaitList.h"

// Resources
#include "Resource.h"
#include "Edevice.h"
#include "Udevice.h"
#include "GymRoom.h"

// Treatments
#include "Treatment.h"
#include "E_Treatment.h"
#include "U_Treatment.h"
#include "X_Treatment.h"

// Patient
#include "Patient.h"

/**
 * @class Schedular
 * @brief Main scheduling system responsible for managing patient flow and treatment assignment.
 */
class Schedular
{
private:
    // Main lists in the schedular
    LinkedQueue<Patient*> history;
    LinkedQueue<Patient*> ALL_patients;  ///< Queue of all patients
    LinkedQueue<Resource*> E_Devices;    ///< Queue of E devices
    int NumEdevices;
    LinkedQueue<Resource*> U_Devices;    ///< Queue of U devices
    int NumUdevices;
    LinkedQueue<Resource*> X_Devices;      ///< Queue of X rooms
    int NumXdevices;
    priQueue <Resource*> MaintenanceE;
    priQueue <Resource*> MaintenanceU;

    // Management lists
    EarlyPlist EARLY_patients;           ///< Priority list for early patients
    priQueue<Patient*> LATE_patients;    ///< Priority queue for late patients

    // Waiting lists
    EU_WaitList Ewaiting;          ///< Waiting list for E treatments
    EU_WaitList Uwaiting;          ///< Waiting list for U treatments
    X_WaitList Xwaiting;           ///< Waiting list for X treatments

    //Interrupted Patients List


    // In-treatment list
    TreatmentList inTreatment; ///< Queue for patients currently in treatment
    priQueue<Patient*> interruptedPatients;

    // Finished patients list
    ArrayStack<Patient*> Finished_patients; ///< Stack of finished patients

    // For time tracking
    int currentTime;     ///< Current simulation time

    // For rescheduling patients logic
    int lastArrivalTime; ///< Tracks last arrival to maintain sorted order for rescheduling

    // Special functionality
    int pCancel;        ///< Cancellation percentage
    int pResc;          ///< Reschedule percentage
    int pFreeFailure;   ///< Free failure percentage
    int pBusyFailure;   ///< Busy failure percentage

    // Statistics for output file
    int TotalTimeSteps;
    int TotalPatients;
    int N_patients;
    int R_patients;
    int TotalEarly;
    int TotalLate;
    int TotalLatePenalty;
    int TotalFreeFail_E;
    int TotalFreeFail_U;
    int TotalBusyFail_E;
    int TotalBusyFail_U;

    double AvgWaitTime_all, AvgWaitTime_N, AvgWaitTime_R;
    double AvgTreatmentTime_all, AvgTreatmentTime_N, AvgTreatmentTime_R;
    double perc_accepted_cancell;
    double perc_accepted_resch;
    double perc_early_patients;
    double perc_late_patients;
    double AvgLatePenalty;
    double perc_freefail_E;
    double perc_freefail_U;
    double perc_Busyfail_E;
    double perc_Busyfail_U;

    /**
     * @brief Skips waiting and checks for direct treatment eligibility.
     * @param required_treatment Pointer to the treatment.
     * @return Pointer to an available resource, or nullptr if not available.
     */
    Resource* ReadyResource(Treatment* required_treatment);

    /**
     * @brief Handles recovering patients and adjusts treatment order.
     * @param rec Pointer to the recovering patient.
     */
    void _handle_recovering_patient(Patient* rec);

    /**
     * @brief Manages rescheduling logic.
     * @return True if a patient was rescheduled, false otherwise.
     */
    bool _handle_rescheduling_patient();

    ResType _handle_busy_failure();    



    /**
     * @brief Calculates final statistics after simulation ends.
     */
    void CalcPatientStatistics();

    // Support for CheckAllPatientList();
    void AddToEarlyList(Patient* patient, int arr_time, int app_time);   ///< Insert patient into early list
    void AddToLateList(Patient* patient, int arr_time, int app_time);    ///< Insert patient into late list
    void AddToInterrupted(Patient* InterruptedPatient, int app_time);    ///< Inser patient into Interrupted list
public:
    /** @brief Default constructor. */
    Schedular();

    // Time setters
    void setCurrentTime(int currtime);          ///< Set current time
    void IncrementTime();                       ///< Increments simulation time

    // Set special functionality percentages
    void setpCancel(int p_cancel);              ///< Set cancellation percentage
    void setpResch(int p_resch);                ///< Set reschedule percentage
    void setpFreeFail(int pFF);                 ///< Set reschedule percentage
    void setpBusyFail(int pBF);                 ///< Set reschedule percentage

    // Initialization functions
    void AddToAllList(Patient* myPatient);      ///< Add patient to all list
    void AddResource(Resource* resource);       ///< Add resource to appropriate list
    void UpdateNumberResources();               ///< Updates the total number of each resource

    // Flow preparation
    void CheckAllPatientList();                 ///< Moves patients from ALL list to early/late lists
    void CheckInterrupted();                    ///< Moves patients from Interrupted list to treatment  (highest priority)
    void CheckMaintainanceList();               ///< called peridically to check for maintainance finish
    void EarlyToWait();                         ///< Transfers early patients to waiting lists
    void LateToWait();                          ///< Transfers late patients to waiting lists
    void WaitToTreatment();                     ///< Assigns patients from waiting to treatment
    void MoveToNextTreatment();                 ///< Advances all patients to next treatment phase

    // Waitlist management (used in Treatment::MoveToWait)
    bool addToEWait(Patient* patient);          ///< Add to E wait list
    bool addToUWait(Patient* patient);          ///< Add to U wait list
    bool addToXWait(Patient* patient);          ///< Add to X wait list
    /**
     * @return true if at least one ..-type device is available; false otherwise.
     * @note This function is used by Treatment::CanAssign().
     **/
    bool CanAssignE() const;                    /// Checks if a E-type device is available for assignment.
    bool CanAssignU() const;                    /// Checks if a U-type device is available for assignment.
    bool CanAssignX() const;                    /// Checks if a X-type device is available for assignment.


    // Treatment assignment
    void AddToTreatmentList(Patient* myPatient, Resource* myResource); ///< Direct add to treatment

    // Finalization
    bool IsPatientFinished(Patient* myPatient); ///< Check if a patient has completed all treatments

    // Current state getters (used in UI)
    int getCurrentTime() const;
    int getLastArrivalTime() const;
    int getEarlyPatientsCount() const;
    int getLatePatientsCount() const;
    int getEWaitCount() const;
    int getUWaitCount() const;
    int getXWaitCount() const;
    int getInTreatmentCount() const;
    int getInterruptedCount() const;
    bool isThereMaintainance() const;

    // Display functions
    void printResources() const;
    void printEarlyPatients() const;
    void printLatePatients() const;
    void printEWait() const;
    void printUWait() const;
    void printXWait() const;
    void printInTreatment() const;
    void printInterruptedList() const;
    void printMaintainance() const;

    // Special values getters
    int getpCancel() const;
    int getpResc() const;

    // Statistics getters
    int getTotalPatients() const;
    int getTotalResources() const;
    int getTotalTimeSteps() const;
    int getTotal_N_Patient() const;
    int getTotal_R_Patient() const;
    int getTotal_BusyFail_E() const;
    int getTotal_BusyFail_U() const;
    double get_AvgWait() const;
    double get_AvgWait_N() const;
    double get_AvgWait_R() const;
    double get_AvgTimeTreatment() const;
    double get_AvgTimeTreatment_N() const;
    double get_AvgTimeTreatment_R() const;
    double get_Perc_Accepted_Cancel() const;
    double get_Perc_Accepted_Resch() const;
    double get_Perc_Early_Patients() const;
    double get_Perc_Late_Patients() const;
    double get_Avg_Late_Penalty() const;
    double get_Perc_FreeFail_E() const;
    double get_Perc_FreeFail_U() const;

    /**
     * @brief Check whether all patients have finished.
     * @return True if all are done, false otherwise.
     **/
    bool SimulationEnded();

    // Exposing internal structures for UI access
    LinkedQueue<Patient*> getHistory() const;

    priQueue <Resource*> getE_MaintainanceList() const;
    priQueue <Resource*> getU_MaintainanceList() const;
    ArrayStack<Patient*> getFinishedPatients() const;     ///< Finished patients stack
    LinkedQueue<Patient*> getAllPatientList() const;       ///< All patient list

    /** @brief Destructor. **/
    ~Schedular();
};
