#ifndef AUDIO_H
#define AUDIO_H

#include "LPC17XX.h"
#include "../music/music.h"

enum AUDIO_E{
	A_NONE,
	A_CUDDLE,
	A_SNACK,
	A_MEAL,
	A_CLICK,
	A_END,
};

typedef struct {
	NOTE * notes;
	uint32_t length; // #elements
} AUDIO_t;


AUDIO_t* getAudio(uint8_t audioEnum);

#endif
