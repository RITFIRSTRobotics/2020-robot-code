#ifndef _PICONZERO_H
#define _PICONZERO_H

#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "core/i2c/i2cUtils.h"

#define PZ_I2CADDR 0x22
#define PZCMD_MOTORA 0
#define PZCMD_MOTORB 1
#define PZCMD_OUTCFG0 2
#define PZCMD_OUTPUT0 8
#define PZCMD_INCFG0 14
#define PZCMD_SETBRIGHT 18
#define PZCMD_UPDATENOW 19
#define PZCMD_RESET 20

#define PZ_NUM_RETRIES 10
#define PZ_MAX_OUTPUT_CHANNEL 5
#define PZ_MAX_INPUT_CHANNEL 3
#define PZ_MAX_OUTPUT_CONFIG 3
#define PZ_MAX_INPUT_CONFIG 2

#define PZ_EXIT_SUCCESS 0
#define PZ_EXCEEDED_RETRIES -1
#define PZ_INVALID_RANGE -2
#define PZ_UNSUPPORTED -3
#define PZ_COULD_NOT_CONNECT -4
#define PZ_UNINITIALIZED -5

typedef struct PiconZero_S
{
    /**
     * The i2cBus struct representing the bus the piconzero is on
     */
    i2cBus bus;
    /**
     * 1 if the piconzero has been intialized, 0 otherwise.
     */
    uint16_t initialized;
    /**
     * First byte is the firmware revision, second is the board type.
     */
    uint16_t revision;
}* PiconZero;


PiconZero pz_create();
int pz_init(PiconZero pz);

int pz_setMotor(PiconZero pz, int motor, int8_t value);
int pz_cleanup(PiconZero pz);
int pz_destroy(PiconZero pz);

#endif
