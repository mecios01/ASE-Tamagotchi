#ifndef IMAGES_H
#define IMAGES_H
#include "LPC17xx.h"

enum IMAGES{
	I_STEADY,
	I_CYGAR,
	I_DRINKING,
	I_BOTTLE0,
	I_BOTTLE1,
	I_BOTTLE2,
	I_FIRE,
	I_FIRE1,
	I_SMOKE0,
	I_SMOKE1,
	I_SMOKE2,
	I_END,
	I_END_TEXT,
	I_SNACK,
	I_MEAL,
	I_HEART,
	I_CUDDLE,
	I_VOLUME_HIGH,
	I_VOLUME_MEDIUM,
	I_VOLUME_LOW,
	I_VOLUME_MUTED
};


const uint16_t* getImage(uint8_t name,uint16_t *width, uint16_t *height);
#endif
