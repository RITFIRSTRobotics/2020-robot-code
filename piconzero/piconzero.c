/**
 * C library for the piconzero hat for the raspberry pi 0.
 * Written by:
 * 	Alex Kneipp
 */
#define _DEFAULT_SOURCE
#include "piconzero.h"
#include <unistd.h>

static int pz_sendByte(PiconZero* pz, uint8_t reg, int8_t data)
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

static int pz_sendWord(PiconZero* pz, uint8_t reg, int16_t data)
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

static int8_t pz_getByte(PiconZero* pz, int8_t reg)
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

static int16_t pz_getWord(PiconZero* pz, int16_t reg)
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

PiconZero* pz_create()
{
	PiconZero* rval = malloc(sizeof(struct PiconZero*_S));
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

int pz_init(PiconZero* pz)
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

int pz_setMotor(PiconZero* pz, int motor, int8_t value)
{
	if( (motor < 0 || motor > 1))
	{
		return PZ_INVALID_RANGE;
	}
	return pz_sendByte(pz, (uint8_t)motor, value);
}

int pz_readInput(PiconZero* pz, int channel, i2cWord_t* buf)
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

int pz_setOutputConfig(PiconZero* pz, int channel, int8_t configValue)
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
	pz_sendByte(pz, PZCMD_OUTCFG0 + channel, configValue)
	return errno;
}

int pz_setInputConfig(PiconZero* pz, int channel, int configValue, int pullup)
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

int pz_cleanup(PiconZero* pz)
{
	pz->initialized = 0;
	return PZ_EXIT_SUCCESS;
}

int pz_destroy(PiconZero* pz)
{
	free(pz);
	return PZ_EXIT_SUCCESS;
}
