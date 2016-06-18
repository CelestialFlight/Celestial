#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

#define RADTODEG 57.2957795131
#define DEGTORAD 0.01745329251

// Returns the current system time in usec.
uint64_t SystemTime(void);

// Waits a certain number of usec.
void SystemWait(uint64_t usec);

// Waits until the system time reaches a given value.
void SystemWaitUntil(uint64_t waittime);


#endif
