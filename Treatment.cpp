#include "Treatment.h"
#include "Resource.h"

Treatment::Treatment()
    : duration(0), Assigned_Resource(nullptr), assignmentTime(-1), finishTime(-1) , type(DUMMY_THERAPY), interrupted(false){}


Treatment::Treatment(int d, TreatmentType TreatType)
    : duration((d > 0) ? d : 0), Assigned_Resource(nullptr), assignmentTime(-1), finishTime(-1), type(TreatType), interrupted(false) {}

void Treatment::assignResource(Resource* resource)
{
    if (!resource)
        return;

    resource->assignPatient();
    Assigned_Resource = resource;
}

Resource* Treatment::FreeResource()
{
    Assigned_Resource->releasePatient();
    Resource* Free = Assigned_Resource;
    Assigned_Resource = nullptr;
    return Free;
}

void Treatment::setAssignmentTime(int t)
{
    if ((t > finishTime && finishTime != -1) || t < 0)
        return;
    else
        assignmentTime = t;
}

void Treatment::setDuration(int t)
{
    duration = (t > 0) ? t : duration;
}

void Treatment::setFinishTime(int t)
{
    if (t < assignmentTime || t < 0)
        return;
    else
        finishTime = t;
}

void Treatment::setInterrupted()
{
    interrupted = true;
}

int Treatment::getDuration() const
{
    return duration;
}

int Treatment::getAssignmentTime() const
{
    return assignmentTime;
}

int Treatment::getFinishTime() const
{
    return finishTime;
}

Resource* Treatment::getAssignedResource() const
{
    return Assigned_Resource;
}

TreatmentType Treatment::getType() const
{
    return type;
}

bool Treatment::isInterrupted() const
{
    return interrupted;
}

// Virtual destructor
Treatment::~Treatment()
{
    // If any cleanup is needed for Assigned_Resource, handle it here.
}

