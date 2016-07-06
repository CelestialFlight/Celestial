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

    // Fatfs information.
    FATFS Fatfs;
    FIL Fil;

    // A flag for if the serial buffer should be sent.
    int shouldSendBuffer;

    // Information gathered by the sd card mounting.
    uint32_t totalSpace;
    uint32_t freeSpace;
};

// Initializes the SD Card structure.
int SDCardInit(struct SDCard* card);
int SDCardUnlink(struct SDCard* card, const char* fileName);

// Attempts to mount the SD Card. Returns 0 if succesful, an error
// message from FatFS otherwise.
int SDCardMount(struct SDCard* card);

// Attempts to open a file on the SD Card.  Returns 0 if succesful.
int SDCardOpen(struct SDCard* card, const char* fileName);

// Closes file on a SD Card.
int SDCardClose(struct SDCard* card);

// Prints data to the currently opened file.
int SDCardPrintf(struct SDCard* card, char* c, ...);

// For applications where the card could be removed suddenly or power loss
// could occur, keeping critical sections as short as possible is important.
// When this is called after a SDCardWrite, it essentially tells the SD card
// to "halt" until another write occurs. This reduces critical sections to
// the time between the SDCardWrite call starts and the SDCardSync call ends.
int SDCardSync(struct SDCard* card);

// Writes a set of bytes to the sd card. Buf is the array of bytes to send, num
// is the number of bytes to send in that array.
int SDCardWrite(struct SDCard* card, void* buf, int num);

// This tells the SD Card to write whatever contents are currently buffered to
// the card.  This is only necessary if there is no more data going to be saved
// to the SD card and the program needs to confirm all the data had been saved.
void SDCardForceWriteBuffer(volatile struct SDCard*  card);

#endif
