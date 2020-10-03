#ifndef _PICONZERO_H
#define _PICONZERO_H

// standard includes
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// i2c utilities
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// local i2c utilities
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

// Define an enum for the various return codes of the Picon Zero
typedef enum {
    PZ_EXIT_SUCCESS = 0,
    PZ_EXCEEDED_RETRIES = -1,
    PZ_INVALID_RANGE = -2,
    PZ_UNSUPPORTED = -3,
    PZ_COULD_NOT_CONNECT = -4,
    PZ_UNINITIALIZED = -5
} PiconZeroState_t;

// Define an enum for the input configurations
typedef enum {
    PZ_IC_DIGITAL = 0,
    PZ_IC_ANALOG = 1,
    PZ_IC_DS18B20 = 2,
    PZ_IC_DUTY_CYCLE=4,
    PZ_IC_PULSE_WIDTH=5
} PZ_InputConfig_t;

// Define an enum for the output configurations
typedef enum {
    PZ_OC_DIGITAL = 0,
    PZ_OC_PWM = 1,
    PZ_OC_SERVO = 2,
    PZ_OC_WS2812B = 3
} PZ_OutputConfig_t;

// Define a structure to save data about the Picon Zero
typedef struct {
    i2cBus bus; // the i2c bus struct representing the bust the Picon Zero is on
    uint16_t initialized; // 1 if the board has been initialized, else 0
    uint16_t revision; // board info, first byte is firmware revision, second is board type
} PiconZero;

/**
 *
 * @return
 */
PiconZero* pz_create();
/**
 *
 * @param pz
 * @return
 */
int pz_init(PiconZero* pz);

/**
 *
 * @param pz
 * @param motor
 * @param value
 * @return
 */
int pz_setMotor(PiconZero* pz, int motor, int8_t value);
/**
 *
 * @param pz
 * @param channel
 * @param buf
 * @return
 */
int pz_readInput(PiconZero* pz, int channel, int16_t* buf);
/**
 *
 * @param pz
 * @param channel
 * @param configValue
 * @return
 */
int pz_setOutputConfig(PiconZero* pz, int channel, PZ_OutputConfig_t configValue);
/**
 *
 * @param pz
 * @param channel
 * @param configValue
 * @param pullup
 * @return
 */
int pz_setInputConfig(PiconZero* pz, int channel, PZ_InputConfig_t configValue, int pullup);

/**
 *
 * @param pz
 * @return
 */
int pz_cleanup(PiconZero* pz);
/**
 *
 * @param pz
 * @return
 */
int pz_destroy(PiconZero* pz);

#endif
