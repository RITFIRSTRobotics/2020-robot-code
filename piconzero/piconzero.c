/**
 * C library for the piconzero hat for the raspberry pi 0.
 * Written by:
 * 	Alex Kneipp
 */
#define _DEFAULT_SOURCE

#include <unistd.h>
#include "piconzero.h"

/**
 * Send a byte to the Picon Zero over i2c
 *
 * @param pz the initialized Picon Zero structure to use to send the data
 * @param reg the register to write the data to
 * @param data the data to write to the register
 */
static int pz_sendByte(PiconZero_t* pz, uint8_t reg, int8_t data)
{
	if(!pz->initialized)
	{
		return PZ_UNINITIALIZED;
	}
	for(int i = 0; i < PZ_NUM_RETRIES; i++)
	{
		if(i2cBus_writeByte(pz->bus, PZ_I2CADDR, reg, data) == 0)
		{
			return PZ_EXIT_SUCCESS;
		}
	}
	return PZ_EXCEEDED_RETRIES;
}

/**
 * Send a word (16-bit) to the Picon Zero over i2c
 *
 * @param pz the initialized Picon Zero structure to use to send the data
 * @param reg the register to write the data to
 * @param data the data to write to the register
 */
static int pz_sendWord(PiconZero_t* pz, uint8_t reg, int16_t data)
{
	if(!pz->initialized)
	{
		return PZ_UNINITIALIZED;
	}
	for(int i = 0; i < PZ_NUM_RETRIES; i++)
	{
		if(i2cBus_writeWord(pz->bus, PZ_I2CADDR, reg, data) == 0)
		{
			return PZ_EXIT_SUCCESS;
		}
	}
	return PZ_EXCEEDED_RETRIES;
}

/**
 * Get a byte from the Picon Zero over i2c
 *
 * @param pz the initialized Picon Zero structure to get data from
 * @param reg the register to read data from
 * @error if an error occurs, errno is set with the error code
 */
static int8_t pz_getByte(PiconZero_t* pz, int8_t reg)
{
	if(!pz->initialized)
	{
		errno = PZ_UNINITIALIZED;
		return  -1;
	}
	int8_t data = 0;
	for(int i = 0; i < PZ_NUM_RETRIES; i++)
	{
		if( (data = i2cBus_readByte(pz->bus, PZ_I2CADDR, reg)) != -1)
		{
			errno = PZ_EXIT_SUCCESS;
			return data;
		}
	}
	errno = PZ_EXCEEDED_RETRIES;
	return -1;
}

/**
 * Get a word (16-bit) from the Picon Zero over i2c
 *
 * @param pz the initialized Picon Zero structure to get data from
 * @param reg the register to read data from
 * @error if an error occurs, errno is set with the error code
 */
static int16_t pz_getWord(PiconZero_t* pz, int16_t reg)
{
	if(!pz->initialized)
	{
		errno = PZ_UNINITIALIZED;
		return  -1;
	}
	int16_t data = 0;
	for(int i = 0; i < PZ_NUM_RETRIES; i++)
	{
		if( (data = i2cBus_readWord(pz->bus, PZ_I2CADDR, reg)) != -1)
		{
			errno = PZ_EXIT_SUCCESS;
			return data;
		}
	}
	errno = PZ_EXCEEDED_RETRIES;
	return -1;
}

/**
 * @inherit
 */
PiconZero_t* pz_create()
{
	PiconZero_t* rval = malloc(sizeof(PiconZero_t));
	char* filename="/dev/i2c-1";
	if( (rval->bus = i2cBus_create(filename)) == NULL )
	{
		//TODO log that we couldn't create a connection to the piconzero
		free(rval);
		return NULL;
	}
	rval -> initialized = 0;
	return rval;
}

/**
 * @inherit
 */
int pz_init(PiconZero_t* pz)
{
	errno = 0;
	if(i2cBus_initSlave(pz->bus, PZ_I2CADDR) != 0)
	{
		//TODO log that we couldn't get on the i2c bus for some reason
		return PZ_COULD_NOT_CONNECT;
	}
	pz_sendByte(pz, PZCMD_RESET, 0);
	//Wait for the command to complete
	usleep(10000);
	pz->revision = pz_getWord(pz, 0);
	pz->initialized = 1;
	return PZ_EXIT_SUCCESS;
}

/**
 * @inherit
 */
int pz_setMotor(PiconZero_t* pz, int motor, int8_t value)
{
	if( (motor < 0 || motor > 1))
	{
		return PZ_INVALID_RANGE;
	}
	return pz_sendByte(pz, (uint8_t)motor, value);
}

/**
 * @inherit
 */
int pz_readInput(PiconZero_t* pz, int channel, int16_t *buf)
{
	if(channel < 0 || channel > PZ_MAX_INPUT_CHANNEL)
	{
		return PZ_INVALID_RANGE;
	}
	errno = 0;
	int tmpBuf = pz_getWord(pz, channel + 1);
	if(errno != 0)
	{
		//TODO error handling.
		return errno;
	}
	*buf = tmpBuf;
	return PZ_EXIT_SUCCESS;
}

/**
 * @inherit
 */
int pz_setOutputConfig(PiconZero_t* pz, int channel, PZ_OutputConfig_t configValue)
{
	if(channel < 0 || channel > PZ_MAX_OUTPUT_CHANNEL)
	{
		return PZ_INVALID_RANGE;
	}
	if(configValue < 0 || configValue > PZ_MAX_OUTPUT_CONFIG)
	{
		return PZ_INVALID_RANGE;
	}
	errno = 0;
	pz_sendByte(pz, PZCMD_OUTCFG0 + channel, configValue);
	return errno;
}

/**
 * @inherit
 */
int pz_setInputConfig(PiconZero_t* pz, int channel, PZ_InputConfig_t configValue, int pullup)
{
	if(channel < 0 || channel > PZ_MAX_INPUT_CHANNEL)
	{
		return PZ_INVALID_RANGE;
	}
	if(configValue < 0 || configValue > PZ_MAX_INPUT_CONFIG)
	{
		return PZ_INVALID_RANGE;
	}
	if((pz->revision & 0xF0) >> 8 <= 6 )
	{
		return PZ_UNSUPPORTED;
	}
	if(configValue == 0 && pullup)
	{
		configValue = 128;
	}
	errno = 0;
	pz_sendByte(pz, PZCMD_INCFG0 + channel, configValue);
	return errno;
}

/**
 * @inherit
 */
int pz_cleanup(PiconZero_t* pz)
{
	pz->initialized = 0;
	return PZ_EXIT_SUCCESS;
}

/**
 * @inherit
 */
int pz_destroy(PiconZero_t* pz)
{
	free(pz);
	return PZ_EXIT_SUCCESS;
}
