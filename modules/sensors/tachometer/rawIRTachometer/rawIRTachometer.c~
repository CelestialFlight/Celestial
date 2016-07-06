#include "rawIRTachometer.h"
#include "common/common.h"

int RawIRTachometerInit(struct RawIRTachometer* tach)
{
    tach->RPM = 0;
    tach->_lastUpdateTime = SystemTime();

    return 0;
}

int RawIRTachometerUpdate(struct RawIRTachometer* tach, uint64_t curTime)
{
    uint64_t diff = curTime - tach->_lastUpdateTime;
    tach->_lastUpdateTime = curTime;

    tach->RPM = diff;

    return 0;
}

double RawIRTachometerGetRPM(struct RawIRTachometer* tach)
{
    return tach->RPM;
}
