/**
 * C library for the piconzero hat for the raspberry pi 0.
 * Written by:
 * 	Alex Kneipp
 */
#define _DEFAULT_SOURCE
#include "piconzero.h"
#include <unistd.h>

static int pz_sendByte(PiconZero pz, uint8_t reg, int8_t data)
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

static int pz_sendWord(PiconZero pz, uint8_t reg, int16_t data)
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

static int8_t pz_getByte(PiconZero pz, int8_t reg)
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

static int16_t pz_getWord(PiconZero pz, int16_t reg)
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

PiconZero pz_create()
{
	PiconZero rval = malloc(sizeof(struct PiconZero_S));
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

int pz_init(PiconZero pz)
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

int pz_setMotor(PiconZero pz, int motor, int8_t value)
{
	if( (motor < 0 || motor > 1))
	{
		return PZ_INVALID_RANGE;
	}
	return pz_sendByte(pz, (uint8_t)motor, value);
}

int pz_cleanup(PiconZero pz)
{
	pz->initialized = 0;
	return PZ_EXIT_SUCCESS;
}

int pz_destroy(PiconZero pz)
{
	free(pz);
	return PZ_EXIT_SUCCESS;
}
