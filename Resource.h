/**
 * @file Resource.h
 * @brief Abstract base class for medical resources (devices/rooms).
 *
 * Defines the Resource class which manages the identification, status, and type
 * of medical resources such as devices or rooms.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once

#define FREE true
#define FULL false
#define MAINTAINANCE_DURATION 6
#define MAINTAINANCE_COOLDOWN 3

#include <iostream>
using namespace std;

/**
 * @enum ResType
 * @brief Represents the type of resource/device.
 */
enum ResType
{
    _Edevice,       ///< Electrotherapy device
    _Udevice,       ///< Ultrasound therapy device
    _Xdevice,       ///< Exercise/Gym room
    _dummydevice = -1  ///< Default/invalid type
};

class Resource
{
protected:
    int id;             ///< Unique resource ID
    bool status;        ///< Resource status (FREE or FULL)
    ResType type;       ///< Resource type

public:
    /**
     * @brief Constructor to initialize a resource with a given type.
     * @param t Type of the resource (default is _dummydevice).
     */
    Resource(ResType t = _dummydevice);

    /**
     * @brief Assigns a patient to the resource (makes it busy).
     */
    virtual void assignPatient();

    /**
     * @brief Releases the current patient (frees the resource).
     */
    virtual void releasePatient();

    /**
     * @brief Gets the unique ID of the resource.
     * @return Integer ID.
     */
    virtual int getID() const;

    /**
     * @brief Gets the type of the resource.
     * @return ResType enum representing the resource type.
     */
    virtual ResType getType() const;

    /**
     * @brief Checks if the resource is currently available.
     * @return True if free, false if in use.
     */
    virtual bool isAvailable() const;

    /**
     * @brief Virtual destructor for proper cleanup.
     */
    virtual ~Resource();

    /**
     * @brief Pure virtual function to print resource details to stream.
     * @param os Output stream.
     */
    virtual void print(ostream& os) const = 0;

    /**
     * @brief Overloaded stream insertion operator.
     * @param os Output stream.
     * @param r Reference to resource.
     * @return Output stream with resource info.
     */
    friend ostream& operator<<(ostream& os, const Resource& r);
};
