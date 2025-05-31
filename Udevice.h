/**
 * @file Udevice.h
 * @brief Defines the Udevice class, a type of Resource that can fail and require maintenance.
 *
 * The Udevice class extends the abstract Resource class to model a medical device
 * that may experience failures both when free and busy, requiring repair and maintenance.
 * It tracks the device's working state, maintenance time, cooldown period, and failure history.
 *
 * @author Abdelrahman Samir
 * @date March 2025
 */
#pragma once
#include "Resource.h"
#include <iostream>

class Udevice : public Resource
{
private:
    bool Working;                ///< Indicates if the device is working.
    int maintainanceTime;        ///< Remaining maintenance time.
    int MaintainanceCooldown;    ///< Cooldown time before the device can fail again.
    bool failedOnce;             ///< Tracks if the device has failed previously.
    int id;                      ///< Unique identifier for the device.

public:
    /**
     * @brief Default constructor. Initializes the Udevice and assigns a unique ID.
     */
    Udevice();

    /**
     * @brief Simulates a failure check when the device is free.
     *
     * @param pFF Probability (0-100) of failure occurring.
     * @return true if the device remains working, false if it fails.
     */
    bool FreeFail(int pFF);

    /**
     * @brief Forces the device to fail while busy.
     *
     * @return Always returns true indicating device failure.
     */
    bool BusyFail();

    /**
     * @brief Repairs the device, setting it to working state.
     */
    void Repair();

    /**
     * @brief Gets the current maintenance time remaining.
     *
     * @return Maintenance time as an integer.
     */
    int getMaintainanceTime() const;

    /**
     * @brief Sets the maintenance time.
     *
     * @param time New maintenance time to set.
     */
    void setMaintainanceTime(int time);

    /**
     * @brief Checks if the device is currently working.
     *
     * @return true if working, false otherwise.
     */
    bool isWorking() const;

    /**
     * @brief Checks if the device has failed before.
     * The first call returns false and sets the flag; subsequent calls return true.
     *
     * @return true if failed before, false if this is the first failure check.
     */
    bool FailedBefore();

    /**
     * @brief Prints the device details to the provided output stream.
     *
     * @param os Output stream to print to.
     */
    void print(std::ostream& os) const;
};
