#include "audio.h"
#include <stdlib.h>

//CUDDLE
const NOTE a_cuddle_notes[]={
//	{g3,time_semibiscroma},
//	{a2b,time_croma},
//	{b3,time_semibiscroma},
//	{f4,time_croma},
//	{c4,time_semibiscroma},
//	{f4,time_croma}
{e4,time_semiminima},
{e4,time_croma},
{pause,time_semibiscroma},
{e4,time_croma},
{a4,time_semiminima},
{a4b,time_semiminima},
{d4,time_semiminima},
{d4,time_croma},
{pause,time_semibiscroma},
{d4,time_croma},
{g4b,time_semiminima},
{e4,time_semiminima},
{pause,time_semibiscroma},
{e4,time_semiminima},
{pause,time_semibiscroma},
{e4,time_semiminima},
{pause,time_croma},
{a4,time_croma},
{pause,time_semibiscroma},
{a4,time_semiminima},
{d4,time_semiminima},
{pause,time_semibiscroma},
{d4,time_semiminima},
{pause,time_croma},
{e4,time_croma},
{pause,time_semibiscroma},
{e4,time_semiminima},
{pause,time_semibiscroma},
{e4,time_semiminima},
{pause,time_semibiscroma},
{e4,time_semiminima},
{pause,time_croma},
{a4,time_croma},
{pause,time_semibiscroma},
{a4,time_semiminima},
{d4,time_semiminima},
{pause,time_semibiscroma},
{d4,time_semiminima}
	
};
const AUDIO_t a_cuddle = {(NOTE *)a_cuddle_notes,38};


//MEAL
const NOTE a_meal_notes[]={
	{f3,time_semibiscroma},
	{c4,time_croma},
	{c5,time_semibiscroma},
	{e4,time_croma},
	{c3,time_semibiscroma},
	{g3,time_croma}
};
const AUDIO_t a_meal = {(NOTE *)a_meal_notes,6};

//SNACK 
const NOTE a_snack_notes[]={
	{g3,time_semibiscroma},
	{a3,time_croma},
	{g4,time_semibiscroma},
	{g3,time_croma},
	{a3,time_semibiscroma},
	{c5,time_croma}
};
const AUDIO_t a_snack = {(NOTE *)a_snack_notes,6};

//END
const NOTE a_end_notes[]={
	{f4,time_semibiscroma},
	{e4,time_croma},
	{d3,time_semibiscroma},
	{g4,time_croma},
	{d3,time_semibiscroma},
	{c5,time_croma}
};
const AUDIO_t a_end = {(NOTE *)a_end_notes,6};

//CLICK
const NOTE a_click_notes[]={
	{f4,time_semiminima},
};
const AUDIO_t a_click = {(NOTE *)a_click_notes,1};

AUDIO_t* getAudio(uint8_t audioEnum){
	switch(audioEnum){
		case A_CUDDLE:{
			return (AUDIO_t *) & a_cuddle;
		}
		case A_MEAL:{
			return (AUDIO_t *) & a_meal;
		}
		case A_SNACK:{
			return (AUDIO_t *) & a_snack;
		}
		case A_CLICK:{
			return (AUDIO_t *) & a_click;
		}
		case A_END:{
			return (AUDIO_t *) & a_end;
		}
	}
	return NULL;
}
