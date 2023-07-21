#ifndef MYFUNCTIONS_S
#define MYFUNCTIONS_S

#include "LPC17xx.h"
#include "images.h"

void resetGame(void);
void drawEmptyRectangle(uint16_t X_pos,uint16_t Y_pos, uint16_t width, uint16_t height, uint16_t color);
void drawAgeWidget(void);
void drawHappinessAndSatiety(void);
void drawSprite(uint8_t imageName,uint16_t x0,uint16_t y0);
void drawBaseSprite(void);
void drawActionButtons(void);
void updateAge(void);
void updateLevels(void);
void animateSprite(void);
void handleJoystick(void);
void handleAnimation(void);
void checkHealth(void);
void checkTouchPanel(void);
void playCurrentAudio(void);
void drawVolumeLevels(uint8_t perc);
void endScene(void);
#endif
