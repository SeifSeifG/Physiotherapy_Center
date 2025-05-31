#include "GymRoom.h"
static int ID = 400; // Initialize static member

GymRoom::GymRoom() : Resource(_Xdevice), curr_cap(0), room_capacity(1)
{
    id = ++ID;
}

GymRoom::GymRoom(int capacity) : Resource(_Xdevice), curr_cap(0), room_capacity(capacity)
{
    id = ++ID;
}

int GymRoom::getAvailableSlots() const
{
    return (room_capacity - curr_cap);
}

void GymRoom::assignPatient()
{
    if (status == FREE)
    {
        curr_cap++;
        status = (curr_cap < room_capacity) ? FREE : FULL;
    }
}

void GymRoom::releasePatient()
{
    if (curr_cap > 0)
    {
        curr_cap--;
        status = FREE;
    }
}

void GymRoom::print(std::ostream& os) const
{
    os << "Room ID : " << id << "[" << curr_cap << ", " << room_capacity << "]";
}
