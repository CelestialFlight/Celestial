#ifndef __RAW_IR_TACHOMETER_H__
#define __RAW_IR_TACHOMETER_H__

#include <stdint.h>

// The Raw IR tachometer is a sensor that sits under the propeller
// and has the IR emitter and IR reciever pointed upwards towards
// the blade.  Everytime the blade passes the sensor, the IR reading
// goes high and returns a pulse back to the microcontroller.
//
// The sensor is physically constructed as follows:
//  - IR Emitter with one terminal at 5V and the other tied to ground
//    with a 100ohm resistor.
//  - IR Reciever with one terminal at 5V and the other tied to ground
//    with a 1K resistor.  The connection between the reciever and
//    resistor is the signal.  The signal is fed into a 0.1uF cap which
//    removes the DC componenet of the signal.  That signal is then fed
//    into a comparator/opamp where any signal from the IR reciever that
//    is greater then 100mV sets the comparator high, otherwise low.  The
//    signal from the comparator is then sent to the micontroller and can
//    be read in by interrupts.
struct RawIRTachometer
{
    // Calculated rotations per second.
    double RPM;

    // Last time in usec since last pulse.
    uint64_t _lastUpdateTime;
};

// Initiate tachometer hardware.
int RawIRTachometerInit(struct RawIRTachometer* tach);

// Update the RPM calculation when a new signal comes.
int RawIRTachometerUpdate(struct RawIRTachometer* tach);

// Returns the current RPM of the tachometer.
double RawIRTachometerGetRPM(struct RawIRTachometer* tach);

#endif
