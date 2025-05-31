/**
 * @file Patient.h
 * @brief Defines the Patient class representing patients in the treatment scheduling system.
 *
 * The Patient class encapsulates all relevant data and operations related to a patient,
 * including their unique ID, type (NORMAL or RECOVERING), appointment and arrival times,
 * queue of required treatments, current status, and timing statistics such as waiting
 * and treatment durations. It supports adding treatments without duplication,
 * updating patient status, tracking cancellations and rescheduling, and managing
 * treatment completion.
 *
 * Patients are integral to the scheduling system, with methods for querying and
 * modifying their treatment queues, as well as utility functions for sorting and
 * output formatting. The class also manages state transitions and timing to
 * facilitate simulation of patient flow through treatment resources.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "LinkedQueue.h"
#include "Treatment.h"
#include "E_Treatment.h"
#include "U_Treatment.h"
#include "X_Treatment.h"
#include <iostream>
using namespace std;

/** @enum PatientType
 *  Represents the type of patient.
 */
enum PatientType { NORMAL, RECOVERING, DUMMY_PTYPE = -1 };

/** @enum PatientStatus
 *  Represents the current status of a patient in the system.
 */
enum PatientStatus { EARLY, LATE, WAIT, SERV, INTERRUPTED, FINISHED, IDLE = -1 };

/**
 * @class Patient
 * @brief Represents a patient in the treatment scheduling system.
 */
class Patient {
private:
	/** @brief Unique patient ID. */
	int PID;

	/** @brief Type of the patient (NORMAL or RECOVERING). */
	PatientType pType;

	/** @brief Appointment time. */
	int PT;

	/** @brief Arrival time. */
	int VT;

	/** @brief Queue of required treatments. */
	LinkedQueue<Treatment*> Required_Treatments;

	/** @brief Current status of the patient. */
	PatientStatus status;

	/** @brief Timestamp of the last state change (for tracking time). */
	int LastChangeTimeStamp;

	/** @brief Flag indicating if the patient cancelled. */
	bool cancelled;

	/** @brief Flag indicating if the patient rescheduled. */
	bool rescheduled;

	/** @brief Total time spent waiting. */
	int totalWaitingTime;

	/** @brief Total time spent in treatments. */
	int totalTreatmentTime;

	/** @brief Time the patient finished all treatments. */
	int finishTime;

	/**
	 * @brief Utility function to safely add a new treatment if not duplicated.
	 * @tparam T Type of the treatment.
	 * @param newTreatment Pointer to the new treatment.
	 * @return True if treatment was added, false if it was a duplicate.
	 */
	template<typename T>
	bool addTreatmentHelper(T* newTreatment)
	{
		LinkedQueue<Treatment*> tempQueue;
		Treatment* current;
		bool duplicateFound = false;

		while (Required_Treatments.dequeue(current))
		{
			if (dynamic_cast<T*>(current))
				duplicateFound = true;
			tempQueue.enqueue(current);
		}

		while (tempQueue.dequeue(current))
		{
			Required_Treatments.enqueue(current);
		}

		if (!duplicateFound)
		{
			Required_Treatments.enqueue(newTreatment);
			return true;
		}
		return false;
	}


public:
	/** @brief Default constructor. Initializes an empty patient. */
	Patient();

	/**
	 * @brief Constructor for creating a patient with type, appointment, and arrival time.
	 * @param type Patient type.
	 * @param pt Appointment time.
	 * @param vt Arrival time.
	 */
	Patient(int pt, int vt, PatientType type);

	/**
	 * @brief Constructor with specified ID.
	 * @param id Patient ID.
	 * @param type Patient type.
	 * @param pt Appointment time.
	 * @param vt Arrival time.
	 */
	Patient(int id, int pt, int vt, PatientType type);

	/**
	 * @brief Constructor with treatments queue.
	 * @param id Patient ID.
	 * @param type Patient type.
	 * @param pt Appointment time.
	 * @param vt Arrival time.
	 * @param req_treat Predefined queue of required treatments.
	 */
	Patient(int id, int pt, int vt, PatientType type, LinkedQueue<Treatment*> req_treat);

	/**
	 * @brief Adds a treatment to the patient if not already present.
	 * @param newTreatment Pointer to the new treatment.
	 * @return True if added successfully, false if a duplicate.
	 */
	bool addTreatment(Treatment* newTreatment);

	/** @brief Sets the current status of the patient. */
	void setStatus(PatientStatus newStatus);

	/** @brief Sets the finish time for the patient. */
	void setFinishTime(int time);

	/** @brief Updates the last change timestamp. */
	void TrackLastChange(int time);

	/** @brief Updates the patient's appointment time. */
	void setPT(int pt);

	/** @brief Sets the cancellation flag. */
	void setCancelled(bool);

	/** @brief Adds to the total waiting time. */
	void addWaitingTime(int timesteps);

	/** @brief Adds to the total treatment time. */
	void addTreatmentTime(int timesteps);

	/**
	 * @brief Gets the next treatment in the queue.
	 * @return Pointer to the next treatment, or nullptr if empty.
	 */
	Treatment* getNextTreatment() const;

	/**
	 * @brief Completes the current treatment.
	 * @param CurrentTime The time at which the treatment completes.
	 * @return True if a treatment was completed, false otherwise.
	 */
	bool completeCurrentTreatment(int CurrentTime);

	// -------------------- Getters --------------------

	/** @brief Returns the patient's unique ID. */
	int getPID() const;

	/** @brief Returns the type of the patient. */
	PatientType getType() const;

	/** @brief Returns the patient's appointment time. */
	int getAppointmentTime() const;

	/** @brief Returns the patient's arrival time. */
	int getArrivalTime() const;

	/** @brief Returns a copy of the patient's remaining treatment queue. */
	LinkedQueue<Treatment*> getRemainingTreatments() const;

	/** @brief Returns the number of remaining treatments. */
	int getNumber_RemainingTreatment() const;

	/** @brief Returns the patient's current status. */
	PatientStatus getStatus() const;

	/** @brief Returns the last time the patient changed state. */
	int getLastChange() const;

	/** @brief Returns the total waiting time. */
	int getTotalWaitingTime() const;

	/** @brief Returns the total treatment time. */
	int getTotalTreatmentTime() const;

	/** @brief Returns the finish time. */
	int getFinishTime() const;

	/** @brief Returns whether the patient has cancelled. */
	bool isCancelled() const;

	/** @brief Returns whether the patient has rescheduled. */
	bool isRescheduled() const;

	/** @brief Returns whether the patient has completed all treatments. */
	bool isFinished() const;

	// -------------------- Special Handling --------------------

	/** @brief Marks the patient as cancelled. */
	void markCancelled();

	/**
	 * @brief Marks the patient as rescheduled.
	 * @param newPT New appointment time.
	 * @param newVT New arrival time.
	 */
	void markRescheduled(int newPT, int newVT);

	/**
	 * @brief Resort the treatments queue for a recovering patient.
	 * @param TreatmentType for identifying which is placed first.
	 */
	void ResortTreatments(TreatmentType type);

	/** @brief Destructor. Handles cleanup. */
	~Patient();

	/**
	 * @brief Overloaded stream insertion operator to display patient details.
	 * @param os Output stream.
	 * @param myPatient Reference to the patient.
	 * @return Reference to the output stream.
	 */
	friend ostream& operator<<(ostream& os, Patient& myPatient);
};
