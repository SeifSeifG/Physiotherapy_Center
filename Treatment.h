/**
 * @file Treatment.h
 * @brief Abstract base class representing generic medical treatments.
 *
 * Defines the Treatment class that manages treatment duration, resource assignment,
 * timing, and treatment type. This class serves as a base for specific treatment
 * types and includes interfaces for resource assignment and patient queue management.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include <iostream>
using namespace std;

class Resource;  // Forward declaration
class Schedular; // Forward declaration
class Patient;   // Forward declaration

/** @enum TreatmentType
    Represents the type of treatment a patient may receive. */
enum TreatmentType { E_THERAPY, U_THERAPY, X_THERAPY, DUMMY_THERAPY};

class Treatment
{
protected:
    int duration;               ///< Duration of the treatment
    Resource* Assigned_Resource;///< Pointer to the resource assigned to this treatment
    int assignmentTime;         ///< Time when the treatment was assigned
    int finishTime;             ///< Time when the treatment finishes
    TreatmentType type;                  ///< Type of the treatment (E, U, X)
    bool interrupted;

public:
    /** @brief Default constructor. Initializes members with default values. */
    Treatment();

    /**
     * @brief Parameterized constructor.
     * @param d Duration of the treatment
     * @param TreatType Type of the treatment
     */
    Treatment(int d, TreatmentType TreatType);

    /**
     * @brief Assigns a resource to the treatment.
     * @post Assigned_Resource is set and resource status is updated internally.
     * @param resource Pointer to the resource to be assigned
     */
    virtual void assignResource(Resource* resource);

    /**
     * @brief Frees the resource assigned to this treatment.
     * @post Assigned_Resource is set to nullptr and the resource is marked as available.
     * @return Pointer to freed resource
     */
    virtual Resource* FreeResource();

    /**
     * @brief Sets the time at which treatment was assigned.
     * @param t Time value to be set
     */
    virtual void setAssignmentTime(int t);

    /**
     * @brief Sets the duration of the treatment
     * @param t time to be set
     */
    virtual void setDuration(int t);

    /**
     * @brief Sets the time at which treatment will finish.
     * @param t Time value to be set
     */
    virtual void setFinishTime(int t);

    /**
    * @brief Sets the boolean interrupted to true.
    */
    virtual void setInterrupted();

    /** @brief Gets the duration of the treatment.
     *  @return Duration in time units
     */
    virtual int getDuration() const;

    /** @brief Gets the time at which the treatment was assigned.
     *  @return Assignment time
     */
    virtual int getAssignmentTime() const;

    /** @brief Gets the time at which the treatment will finish.
     *  @return Finish time
     */
    virtual int getFinishTime() const;

    /** @brief Gets the pointer to the assigned resource.
     *  @return Pointer to the Resource object
     */
    virtual Resource* getAssignedResource() const;

    /** @brief Gets the type of the treatment.
     *  @return Enum value of TreatmentType (E_THERAPY, U_THERAPY, X_THERAPY)
     */
    virtual TreatmentType getType() const;

    /** @brief Gets the type of the treatment.
     *  @return True if the treatment was previously interrupted, false otherwise
     */
    virtual bool isInterrupted() const;

    /**
     * @brief Determines whether the treatment can be assigned to a given resource.
     * @param resource Pointer to a candidate Resource object
     * @return True if assignable, false otherwise
     * @note This is a pure virtual function to be implemented by derived classes.
     */
    virtual bool CanAssign(Schedular& schedular) = 0;

    /**
     * @brief Moves a patient to the appropriate waiting list based on treatment type.
     * @param schedular Reference to the scheduler managing the facility
     * @param myPatient Pointer to the patient to be moved
     * @return True if the patient was successfully added to the waitlist
     * @note This is a pure virtual function to be implemented by derived classes.
     */
    virtual bool MoveToWait(Schedular& schedular, Patient* myPatient) = 0;

    /** @brief Virtual destructor to ensure proper cleanup in derived classes. */
    virtual ~Treatment();

    /**
     * @brief Overloaded stream output operator.
     * @param os Output stream
     * @param t Reference to a Treatment object
     * @return Reference to the output stream
     */
    friend ostream& operator<<(ostream& os, const Treatment& t);

    /**
     * @brief Abstract method to print treatment-specific details to output stream.
     * @param os Output stream
     */
    virtual void print(ostream& os) const = 0;
};
