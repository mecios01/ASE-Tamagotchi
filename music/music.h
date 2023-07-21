#ifndef MUSIC_H
#define MUSIC_H

#define GETINTERVAL(x)  ((25000000/45)/x)
//Default: 1.65
#define SPEEDUP 1.6

#define TIMERSCALER 1

#define SECOND 0x17D7840 * TIMERSCALER


typedef char BOOL;
#define TRUE 1
#define FALSE 0

typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/64
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/32
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/16
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/8
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;

typedef enum frequencies
{
	
	a2b = GETINTERVAL(103),	// 103Hz	k=5351 a2b
	b2 = GETINTERVAL(123),	// 123Hz	k=4500 b2
	c3b = GETINTERVAL(127),	// 127Hz	k=4370 c3b
	c3 = GETINTERVAL(131),	// 131Hz	k=4240 c3
	d3 = GETINTERVAL(147),	// 147Hz	k=3779 d3
	e3 = GETINTERVAL(165),	// 165Hz	k=3367 e3
	f3 = GETINTERVAL(175),	// 175Hz	k=3175 f3
	g3 = GETINTERVAL(196),	// 196Hz	k=2834 g3
	a3b = GETINTERVAL(208), // 208Hz  k=2670 a4b
	a3 = GETINTERVAL(220),	// 220Hz	k=2525 a3
	b3 = GETINTERVAL(247),	// 247Hz	k=2249 b3
	c4 = GETINTERVAL(262),	// 262Hz	k=2120 c4
	d4 = GETINTERVAL(294),	// 294Hz	k=1890 d4
	e4 = GETINTERVAL(330),	// 330Hz	k=1684 e4
	f4 = GETINTERVAL(349),	// 349Hz	k=1592 f4
	g4b = GETINTERVAL(370), // 370Hz
	g4 = GETINTERVAL(392),	// 392Hz	k=1417 g4
	a4b = GETINTERVAL(415), // 415Hz
	a4 = GETINTERVAL(440),	// 440Hz	k=1263 a4
	b4 = GETINTERVAL(494),	// 494Hz	k=1125 b4
	c5 = GETINTERVAL(523), 	// 523Hz	k=1062 c5
	
	pause = 0		// DO NOT SOUND
} FREQUENCY;


typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
} NOTE;

void playNote(NOTE note);
BOOL isNotePlaying(void);

#endif
/* EOF */
