#ifndef __SD_CARD_H__
#define __SD_CARD_H__

#include "common/serialBuffer.h"
#include "peripherals/fatfs/ff.h"
#include <stdint.h>

struct SDCard
{
    // SerialBuffer must be the first item in this structure.
    // so that the pointer to the serialBuffer is the same as
    // the pointer for SDCard.
    struct SerialBuffer buf;

    FATFS Fatfs;
    FIL Fil;

    int shouldSendBuffer;

    uint32_t totalSpace;
    uint32_t freeSpace;
};

int SDCardInit(struct SDCard* card);
int SDCardUnlink(struct SDCard* card, const char* fileName);
int SDCardMount(struct SDCard* card);
int SDCardOpen(struct SDCard* card, const char* fileName);
int SDCardClose(struct SDCard* card);
int SDCardPrintf(struct SDCard* card, char* c, ...);

int SDCardSync(struct SDCard* card);
int SDCardWrite(struct SDCard* card, void* buf, int num);
void SDCardForceWriteBuffer(volatile struct SDCard*  card);

#endif
