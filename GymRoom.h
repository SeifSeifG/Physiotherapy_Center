#pragma once
#include "Resource.h"

class GymRoom : public Resource
{
private:
    int curr_cap;
    int room_capacity;

public:
    // Default constructor with capacity 1
    GymRoom();
    // Parameterized constructor with custom capacity
    GymRoom(int capacity);

    // Get available slots in the gym room
    int getAvailableSlots() const;

    // Assign a patient to this room
    void assignPatient();

    // Release a patient from this room
    void releasePatient();

    // Optional: declare an overloaded operator<< if needed
    // friend std::ostream& operator<<(std::ostream& os, const Gym_Room& room);
    void print(std::ostream& os) const override;
};
