#pragma once
// Defines for resource status
#define FREE true
#define FULL false
#define MAINTAINANCE_DURATION 6
#define MAINTAINANCE_COOLDOWN 3
#include <iostream>
using namespace std;

enum ResType
{
    _Edevice, //0
    _Udevice, //1
    _Xdevice, //2
    _dummydevice = -1
};

class Resource
{
protected:
    // Unique id for each device
    int id;

    // Resource status: FREE (true) or FULL (false)
    bool status;

    //resource type : specific for each derived class
    ResType type;

public:
    // Constructor declaration
    Resource(ResType = _dummydevice);

    // Assign a patient to a device
    virtual void assignPatient();

    // release patients from the resource
    virtual void releasePatient();

    // Getter for resource ID
    virtual int getID() const;

    //Getter for resource type
    virtual ResType getType() const;

    // Checks if the resource is available
    virtual bool isAvailable() const;

    // Virtual destructor declaration
    virtual ~Resource();

    virtual void print(std::ostream& os) const = 0;
    friend ostream& operator<<(std::ostream& os, const Resource& r);
};
