/**
 * @file GymRoom.h
 * @brief Defines the GymRoom class representing an exercise room resource for physical therapy.
 *
 * The GymRoom class inherits from Resource and models a room (X-device) that can
 * hold multiple patients up to a specified capacity. It manages patient assignments,
 * releases, and tracks the current occupancy of the room.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "Resource.h"

class GymRoom : public Resource
{
private:
    int curr_cap;        ///< Current number of patients assigned to the room
    int room_capacity;   ///< Maximum capacity of the gym room

public:
    /**
     * @brief Default constructor. Initializes with default capacity (1).
     */
    GymRoom();

    /**
     * @brief Parameterized constructor.
     * @param capacity Maximum number of patients the room can hold.
     */
    GymRoom(int capacity);

    /**
     * @brief Returns the number of available slots in the room.
     * @return Number of remaining available slots.
     */
    int getAvailableSlots() const;

    /**
     * @brief Assigns a patient to the room.
     * Increments the count and updates room status accordingly.
     */
    void assignPatient() override;

    /**
     * @brief Releases a patient from the room.
     * Decrements the count and marks the room as available if needed.
     */
    void releasePatient() override;

    /**
     * @brief Prints the room's current state to the output stream.
     * @param os Output stream.
     */
    void print(ostream& os) const override;
};
