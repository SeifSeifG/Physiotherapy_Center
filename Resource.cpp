#include "Resource.h"

// Constructor implementation
// Constructor implementation
Resource::Resource(ResType t) : id(0), status(FREE)
{
    type = t;
}

void Resource :: assignPatient()
{
    status = FULL;
}

void Resource::releasePatient()
{
    status = FREE;
}

// Returns the unique id of the resource
int Resource::getID() const
{
    return id;
}

// Returns type of resource
ResType Resource :: getType() const
{
    return type;
}

// Returns true if the resource is available
bool Resource::isAvailable() const
{
    return status;
}

// Virtual destructor implementation
Resource::~Resource() { }
