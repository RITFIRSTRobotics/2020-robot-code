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
#include "../i2cUtils.h"

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

#define PZ_EXIT_SUCCESS 0
#define PZ_EXCEEDED_RETRIES -1
#define PZ_INVALID_RANGE -2
#define PZ_UNSUPPORTED -3
#define PZ_COULD_NOT_CONNECT -4
#define PZ_UNINITIALIZED -5

typedef struct PiconZero_S
{
	i2cBus bus;
	uint16_t initialized;
	uint16_t revision;
}* PiconZero;


PiconZero pz_create();
int pz_init(PiconZero pz);
int pz_destroy();

#endif
