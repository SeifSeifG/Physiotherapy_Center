/**
 * @file Schedular.h
 * @brief Defines the Schedular class responsible for managing the entire patient treatment simulation.
 *
 * The Schedular class orchestrates the flow of patients through a multi-resource treatment
 * scheduling system. It handles patient status transitions (early, late, waiting, interrupted, finished),
 * resource allocation and failures, treatment assignment and completion, as well as rescheduling
 * and cancellation events. The class maintains detailed statistics on patient flow, resource
 * usage, and failure rates, enabling comprehensive simulation and analysis of the scheduling process.
 *
 * Internally, it manages separate queues and priority queues for patients and resources,
 * including early and late patient lists, waiting lists for various treatment types,
 * devices undergoing maintenance, and currently active treatments. Utility functions
 * support the simulation time advancement and rescheduling logic.
 *
 * The class exposes a wide range of accessor and utility functions for external interaction,
 * including adding patients and resources, querying simulation status, and retrieving
 * detailed statistics for output.
 *
 * @authors Abdelrahman Samir & Ibrahim Mohsen
 * @date May 2025
 */
#pragma once
#define RANDOM_ARGUMENT ((unsigned int)time(0))

// Lists
#include "TreatmentList.h"
#include "ArrayStack.h"
#include "EarlyPlist.h"
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
 * @brief Main scheduling system responsible for managing patient flow, resource allocation,
 * treatment execution, rescheduling, cancellations, and simulation statistics.
 */
class Schedular
{
private:
    // ===================== Resource & Patient Structures =====================

    LinkedQueue<Patient*> history;                   ///< Queue for patient history (processed patients)
    LinkedQueue<Patient*> ALL_patients;              ///< Queue of all patients
    LinkedQueue<Resource*> E_Devices;                ///< Queue of E devices
    LinkedQueue<Resource*> U_Devices;                ///< Queue of U devices
    LinkedQueue<Resource*> X_Devices;                ///< Queue of X rooms

    int NumEdevices;                                 ///< Number of E devices
    int NumUdevices;                                 ///< Number of U devices
    int NumXdevices;                                 ///< Number of X rooms

    priQueue<Resource*> MaintenanceE;                ///< Maintenance queue for E devices
    priQueue<Resource*> MaintenanceU;                ///< Maintenance queue for U devices

    EarlyPlist EARLY_patients;                       ///< Priority list for early patients
    priQueue<Patient*> LATE_patients;                ///< Priority queue for late patients

    EU_WaitList Ewaiting;                            ///< Waiting list for E treatments
    EU_WaitList Uwaiting;                            ///< Waiting list for U treatments
    X_WaitList Xwaiting;                             ///< Waiting list for X treatments

    priQueue<Patient*> interruptedPatients;          ///< Priority queue for interrupted patients
    TreatmentList inTreatment;                       ///< Queue of patients currently undergoing treatment
    ArrayStack<Patient*> Finished_patients;          ///< Stack of completed patients

    // ===================== Simulation Time and Logic =====================

    int currentTime;                                 ///< Current simulation time
    int lastArrivalTime;                             ///< Last known arrival time (used for sorted input and rescheduling logic)

    // ===================== Configuration Parameters =====================

    int pCancel;                                     ///< Percentage probability of cancellation
    int pResc;                                       ///< Percentage probability of rescheduling
    int pFreeFailure;                                ///< Probability of free resource failure
    int pBusyFailure;                                ///< Probability of busy resource failure

    // ===================== Simulation Statistics =====================

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

    // ===================== Private Utility Functions =====================

    /**
     * @brief Checks if a resource is immediately available for treatment.
     * @param required_treatment The treatment to be matched with a free resource.
     * @return Pointer to a ready resource or nullptr if unavailable.
     */
    Resource* ReadyResource(Treatment* required_treatment);

    /**
     * @brief Adjusts treatment queue for recovering patients.
     * @param rec Pointer to the recovering patient.
     */
    void _handle_recovering_patient(Patient* rec);

    /**
     * @brief Triggers rescheduling process for eligible patients.
     * @return True if a rescheduling occurred.
     */
    bool _handle_rescheduling_patient();

    /**
     * @brief Handles failure of busy resources and processes any required logic.
     * @return Resource type that failed.
     */
    ResType _handle_busy_failure();

    /**
     * @brief Computes final simulation statistics.
     */
    void CalcPatientStatistics();

    /**
     * @brief Adds an early patient to the early list.
     * @param patient Pointer to patient.
     * @param arr_time Arrival time.
     * @param app_time Appointment time.
     */
    void AddToEarlyList(Patient* patient, int arr_time, int app_time);

    /**
     * @brief Adds a late patient to the late list.
     * @param patient Pointer to patient.
     * @param arr_time Arrival time.
     * @param app_time Appointment time.
     */
    void AddToLateList(Patient* patient, int arr_time, int app_time);

    /**
     * @brief Adds a patient to the interrupted list.
     * @param InterruptedPatient Pointer to interrupted patient.
     * @param app_time Their appointment time.
     */
    void AddToInterrupted(Patient* InterruptedPatient, int app_time);

public:
    /** @brief Default constructor. */
    Schedular();

    // ===================== Time and Parameter Setup =====================

    void setCurrentTime(int currtime);          ///< Set the current simulation time
    void IncrementTime();                       ///< Move simulation one timestep forward

    void setpCancel(int p_cancel);              ///< Set cancellation chance
    void setpResch(int p_resch);                ///< Set rescheduling chance
    void setpFreeFail(int pFF);                 ///< Set free device failure chance
    void setpBusyFail(int pBF);                 ///< Set busy device failure chance

    // ===================== Initialization Functions =====================

    void AddToAllList(Patient* myPatient);      ///< Add a patient to the master list
    void AddResource(Resource* resource);       ///< Register a resource into the appropriate list
    void UpdateNumberResources();               ///< Recalculate the total number of devices/rooms

    // ===================== Core Simulation Logic =====================

    void CheckAllPatientList();                 ///< Move patients from all list to early/late
    void CheckInterrupted();                    ///< Move patients from interrupted list to treatment
    void CheckMaintainanceList();               ///< Check if any devices completed maintenance
    void EarlyToWait();                         ///< Transfer early patients to waiting lists
    void LateToWait();                          ///< Transfer late patients to waiting lists
    void WaitToTreatment();                     ///< Assign waiting patients to free devices
    void MoveToNextTreatment();                 ///< Complete current treatment and process next

    // ===================== Waitlist Management =====================

    bool addToEWait(Patient* patient);          ///< Queue patient in E waiting list
    bool addToUWait(Patient* patient);          ///< Queue patient in U waiting list
    bool addToXWait(Patient* patient);          ///< Queue patient in X waiting list

    bool CanAssignE() const;                    ///< Check if an E device is available
    bool CanAssignU() const;                    ///< Check if a U device is available
    bool CanAssignX() const;                    ///< Check if a X room is available

    // ===================== Treatment & Completion =====================

    void AddToTreatmentList(Patient* myPatient, Resource* myResource); ///< Begin treatment immediately
    bool IsPatientFinished(Patient* myPatient);                        ///< Check if patient finished all treatments

    // ===================== Display & UI Functions =====================

    void printResources() const;                ///< Print currently available devices
    void printEarlyPatients() const;
    void printLatePatients() const;
    void printEWait() const;
    void printUWait() const;
    void printXWait() const;
    void printInTreatment() const;
    void printInterruptedList() const;
    void printMaintainance() const;

    // ===================== Accessors =====================

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

    int getpCancel() const;
    int getpResc() const;

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
     * @brief Checks if the simulation is finished.
     * @return True if no more active or waiting patients exist.
     */
    bool SimulationEnded();

    // ===================== Output File Access =====================

    LinkedQueue<Patient*> getHistory() const;                   ///< Get history of processed patients
    priQueue<Resource*> getE_MaintainanceList() const;          ///< Get E device maintenance queue
    priQueue<Resource*> getU_MaintainanceList() const;          ///< Get U device maintenance queue
    ArrayStack<Patient*> getFinishedPatients() const;           ///< Stack of completed patients
    LinkedQueue<Patient*> getAllPatientList() const;            ///< Original list of all patients

    /** @brief Destructor. */
    ~Schedular();
};
