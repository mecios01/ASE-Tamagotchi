#include "LPC17xx.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "audio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "images.h"
#include "../RIT/rit.h"
#include "../timer/timer.h"
#include "myfunctions.h"

#define Warning_Red 	 0xC980
#define Warning_Pink   0xFCCC
#define Warning_Yellow 0xFE60
#define Warning_LGreen 0x9E66
#define Warning_Green  0x34C0


//enums
enum SPRITE_DIRECTIONS {
	CENTER=0,
	LEFT=1,
	RIGHT=2,
	TOP=3,
	ANY=4
};

enum JOY_STATUS{
	JOY_SEL=0,
	JOY_RX,
	JOY_LX,
	NONE,
	RESET, 
};

enum BATTERY_VALUES {
	FULL=100,M_HIGH=75,M_LOW=50,LOW=25,EMPTY=0,
};
enum COLOR_STATUS {
	S_FULL=Warning_Green,
	S_M_HIGH=Warning_LGreen,
	S_M_LOW=Warning_Yellow,
	S_LOW=Warning_Pink,
	S_EMPTY=Warning_Red,
};

enum BATTERY_TYPE{
	HAPPINESS=0,
	SATIETY=1
};

enum SPRITE_STATE{
	STEADY=0,
	STEADY_LEFT=1,
	STEADY_RIGHT=2,
	DRINKING=3,
	SMOKING=4,
	CUDDLE=5,
	//other states later
	END
};

//globals
extern uint16_t BgColor;
extern uint8_t hh,mm,ss;
extern int8_t happiness,satiety;
extern uint8_t currentAudio;
extern uint8_t pendingAudio;
extern uint8_t volume;

uint8_t spriteState = 0;
uint8_t selectedOption = 0; //center -> no options
uint8_t pendingUpdate[2]={0,0};

uint8_t getVolumeLevel(uint8_t percentage){
	if(percentage>=90){
		volume=100;
		return I_VOLUME_HIGH;
	}
	if(percentage>=75){
		volume=75;
		return I_VOLUME_HIGH;
	}
	if(percentage>=50){
		volume=50;
		return I_VOLUME_MEDIUM;
	}
	if(percentage>=25){
		volume=25;
		return I_VOLUME_LOW;
	}
	else{
		volume=0;
		return I_VOLUME_MUTED;
	}
}
void drawEmptyRectangle(uint16_t X_pos,uint16_t Y_pos, uint16_t width, uint16_t height, uint16_t color){
	uint16_t i,j;
	for(i=0;i<height;i++){
		if(i==0||i==(height-1)){
			j = i==(height-1)?Y_pos+height-1:Y_pos;
			LCD_DrawLine(X_pos,j,X_pos+width-1,j,color);
		}else{
			LCD_SetPoint(X_pos,Y_pos+i,color);
			LCD_SetPoint(X_pos+width-1,Y_pos+i,color);
		}
	}
}
void drawAgeWidget(){
	static char timeString[15];
	snprintf(timeString,15*(sizeof(char)),"Age: %02d:%02d:%02d",hh,mm,ss);
	GUI_Text(70,16,(uint8_t*)timeString,Black,BgColor);
}

void drawBattery(uint16_t X_pos,uint16_t Y_pos,uint16_t width,uint16_t height, uint16_t color){
	drawEmptyRectangle(X_pos,Y_pos,width,height,color);
	LCD_DrawRectangle(X_pos+width,Y_pos+4,2,height-8,color);
}
void drawBarLevels(uint8_t type,int8_t value){
	uint16_t x=0,y=0,i=0,color;
	uint8_t rectNum;
	
	switch(value){
		case FULL:{
			rectNum=4;
			color=S_FULL;
			break;
		}
		case M_HIGH:{
			rectNum=3;
			color=S_M_HIGH;
			break;
		}
		case M_LOW:{
			rectNum=2;
			color=S_M_LOW;
			break;
		}
		case LOW:{
			rectNum=1;
			color=S_LOW;
			break;
		}
		case EMPTY:{
			rectNum=0;
			color=S_EMPTY;
			break;
		}
		default:
			rectNum=0;
			color=S_EMPTY;
			break;
	}

	if(type==HAPPINESS){
		x=28;
		y=67;
	}else{
		x=148;
		y=67;
	}
	
	if(rectNum==0){
		if(type==HAPPINESS){
			drawBattery(25,65,60,30,color);
		}else{
			drawBattery(145,65,60,30,color);
		}
	}
	
	for(;i<4;i++){
		LCD_DrawRectangle(x,y,12,26,(i+1>rectNum)?BgColor:color);
		x+=14;
	}
}
void drawHappinessAndSatiety(){

    GUI_Text(20,40,(uint8_t*)"Happiness",Black,BgColor);
    GUI_Text(147,40,(uint8_t*)"Satiety",Black,BgColor);

    drawBattery(25,65,60,30,Black);
    drawBattery(145,65,60,30,Black);
    
    drawBarLevels(HAPPINESS,happiness);
    drawBarLevels(SATIETY,satiety);
}




void drawSpriteBase (uint8_t imageName,uint16_t x0,uint16_t y0,uint8_t direction,uint8_t skip){
	uint16_t fakeColor = 0xFFFF; //just a fake color not to be printed (565) does not have alpha
	uint16_t i,j,pos=0,color,height,width;
	const uint16_t* image = getImage(imageName,&width,&height);
	//LCD_DrawLine(0,96,240,96,Warning_Red); //border top
	
	if(direction==0){
		for(j=0;j<height;j++){
			for(i=0;i<width;i++){
				color=((image[pos]==fakeColor)?BgColor:image[pos]);
				pos++;
				//color=image[pos++];
				if(color!=BgColor){// && //color!=LCD_GetPixel(x0+i,y0+j)){
					LCD_SetPoint(x0+i,y0+j,color);
				}else if(!skip){
					LCD_SetPoint(x0+i,y0+j,color);
				}
			}
		}
	}
	else{
		for(j=0;j<height;j++){
			for(i=0;i<width;i++){
				color=((image[pos]==fakeColor)?BgColor:image[pos]);
				pos++;
				//color=image[pos++];
				if(color!=BgColor){// && //color!=LCD_GetPixel(x0+i,y0+j)){
					LCD_SetPoint(x0-i+width-1,y0+j,color);
				}else if(!skip){
					LCD_SetPoint(x0-i+width-1,y0+j,color);
				}
			}
		}
	}	
	//LCD_DrawLine(0,270,240,270,Warning_Red); //border bottom
}

void drawSpriteDir(uint8_t imageName,uint16_t x0,uint16_t y0,uint8_t direction){
	drawSpriteBase(imageName,x0,y0,direction,1); //mostly we want to skip same colors, including fake color
}
void drawSprite(uint8_t imageName,uint16_t x0,uint16_t y0){
	drawSpriteDir(imageName,x0,y0,0);
}
void drawBaseSprite(){
	drawSprite(I_STEADY,84,122);
}
void drawActionButtons(){
	drawEmptyRectangle(0,270,120,50,Black);
	drawEmptyRectangle(120,270,120,50,Black);
	drawSprite(I_MEAL,0,270);
	drawSprite(I_SNACK,120,270);
}
void drawResetButton(){
	LCD_DrawRectangle(0,270,240,50,BgColor); //erase all
	drawEmptyRectangle(0,270,240,50,Warning_Red); //draw new reset button
	GUI_Text(100,287,(uint8_t *)"RESET",Black,BgColor);
	selectedOption=RESET;
}

void drawVolumeLevels(uint8_t perc){
	static int16_t prec = -1;
	uint16_t x=10,y=10;
	uint16_t curr = getVolumeLevel(perc);
	if(curr!=prec || prec == -1){
		prec=curr;
		LCD_DrawRectangle(x,y,21,21,BgColor);
		drawSprite(curr,x,y);
	}
}
void updateAge(){
	if(ss==59){
		if(mm==59){
			if(hh==23){
				hh=0; //reset day (I assume none is going to play 24h straigth, if that happens idk)
			}else{
				hh++;
			}
			mm=0;
		}else{
			mm++;
		}
		ss=0;
	}else{
		ss++;
	}
	drawAgeWidget();
}
void checkHealth(){
	if(happiness==0||satiety==0){
		if(spriteState!=END){
			spriteState=END;
			// endScene();
		}
	}
}
void updateLevels(){
	static uint8_t times = 0;
	if(times==4){
		satiety-=25;
		happiness-=25;
		//''handle'' the case of 5s elapsing while eating/drinking
		if(pendingUpdate[0]==1){
			pendingUpdate[0]=0;//shout the 'interrrupt'
			happiness+=25;
		}
		if(pendingUpdate[1]==1){
			pendingUpdate[1]=0; //shout the 'interrupt'
			satiety+=25;
		}
		
		drawBarLevels(HAPPINESS,happiness);
		drawBarLevels(SATIETY,satiety);
		times=0;
		
	}else{
		times++;
	}
}

//steady state is the default. If an event changes the spriteState to another state then
//the other animations are triggered
void resetGame(){
	satiety=happiness=100;
	hh=mm=ss=0;
	disable_timer(0);
	disable_timer(3);
	currentAudio=A_NONE;
	spriteState=STEADY;
	
	LCD_DrawRectangle(0,122,240,198,BgColor);
	
	drawAgeWidget();
	drawHappinessAndSatiety();
	drawActionButtons();
	drawBaseSprite();
	animateSprite();
	
	enable_timer(1);
	enable_RIT();


}
void moveEyes(uint8_t direction,uint8_t prevState){
	const uint16_t x0=84,y0=122,
								ll=x0+14, lr=x0+43,
								 rl=x0+26,rr=x0+54,
								ytop=y0+49,ycenter=y0+53,
								 cl=x0+19,cr=x0+48;
	
	switch(direction){
		case RIGHT:{
			LCD_DrawRectangle(cl,ycenter,3,3,0xFFD5);
			LCD_DrawRectangle(cr,ycenter,3,3,0xFFD5);
			LCD_DrawRectangle(rl,ytop,3,3,Black);
			LCD_DrawRectangle(rr,ytop,3,3,Black);
			break;
		}
		case LEFT:{
			LCD_DrawRectangle(cl,ycenter,3,3,0xFFD5);
			LCD_DrawRectangle(cr,ycenter,3,3,0xFFD5);
			LCD_DrawRectangle(ll,ytop,3,3,Black);
			LCD_DrawRectangle(lr,ytop,3,3,Black);
			break;
		}
		case CENTER:{
			switch(prevState){	
				case(STEADY_LEFT):{
					LCD_DrawRectangle(ll,ytop,3,3,0xFFD5);
					LCD_DrawRectangle(lr,ytop,3,3,0xFFD5);
					break;
				}
				case(STEADY_RIGHT):{
					LCD_DrawRectangle(rl,ytop,3,3,0xFFD5);
					LCD_DrawRectangle(rr,ytop,3,3,0xFFD5);
					break;
				}
				case(ANY):{
					LCD_DrawRectangle(rl,ytop,3,3,0xFFD5);
					LCD_DrawRectangle(rr,ytop,3,3,0xFFD5);
					LCD_DrawRectangle(ll,ytop,3,3,0xFFD5);
					LCD_DrawRectangle(lr,ytop,3,3,0xFFD5);
				}
			}
			LCD_DrawRectangle(cl,ycenter,3,3,Black);
			LCD_DrawRectangle(cr,ycenter,3,3,Black);
			break;
		}
	}
}

void drinkAnimation(){
	static uint8_t times=0;
  switch(times){
		case 0:{
			pendingUpdate[0]=1; //avoids the death of the tamagotchi while eating 
			LCD_DrawRectangle(84,122,71,105,BgColor);
			drawSprite(I_DRINKING,80,128);
			drawSprite(I_BOTTLE0,15,160);
			times++;
			enable_timer(2);
			break;
		}
		case 1:{
			currentAudio=A_SNACK;
			playCurrentAudio();
			LCD_DrawRectangle(10,122,133,105,BgColor);
			drawSprite(I_DRINKING,70,122);
			drawSprite(I_BOTTLE1,10,165);
			times++;
			enable_timer(2);
			break;
		}	
		case 2:{
			LCD_DrawRectangle(10,122,127,105,BgColor);
			drawSprite(I_DRINKING,60,122);
			drawSprite(I_BOTTLE2,10,146);
			times++;
			enable_timer(2);
			break;
		}
		case 3:{
			LCD_DrawRectangle(10,122,127,105,BgColor);
			drawSprite(I_DRINKING,60,122);
			drawSprite(I_FIRE,42,186);
			times++;
			enable_timer(2);
			break;
		}
		case 4:{
			LCD_DrawRectangle(10,122,127,105,BgColor);
			drawSprite(I_DRINKING,70,122);
			drawSprite(I_FIRE1,39,186);
			times++;
			enable_timer(2);
			break;
		}
		case 5:{
			LCD_DrawRectangle(10,122,127,105,BgColor);
			drawSprite(I_DRINKING,80,122);
			drawSprite(I_FIRE,62,186);
			times++;
			enable_timer(2);
			break;
		}
		case 6:{
			LCD_DrawRectangle(10,122,133,105,BgColor);
			drawSprite(I_DRINKING,80,122);
			times++;
			enable_timer(2);
			break;
		}
		case 7:{
			disable_timer(2);
			LCD_DrawRectangle(10,122,133,105,BgColor);
			drawSprite(I_STEADY,84,122);
			moveEyes(CENTER,CENTER);
 			spriteState=STEADY;
			if(pendingUpdate[0]==1){//if no update has occurred update manually
				pendingUpdate[0]=0;
				if(happiness<FULL){ //if max is reached no update (No handle for overfull batteries)
					happiness+=25;
					drawBarLevels(HAPPINESS,happiness);
				}
			}
			times=0;
			break;
		}
	}
}
void smokeAnimation(){
	static uint8_t times=0;
	//uint16_t BgColor = Warning_Pink;
  switch(times){
		case 0:{
			pendingUpdate[1]=1;
			LCD_DrawRectangle(84,122,71,105,BgColor);
			drawSpriteDir(I_DRINKING,98,128,1);
			drawSpriteDir(I_CYGAR,175,204,1);
			times++;
			enable_timer(2);
			break;
		}
		case 1:{
			currentAudio=A_MEAL;
			playCurrentAudio();
			LCD_DrawRectangle(98,128,64,97,BgColor);
			drawSpriteDir(I_DRINKING,108,128,1);
			//drawSpriteDir(I_CYGAR,175,200,1);
			times++;
			enable_timer(2);
			break;
		}	
		case 2:{
			LCD_DrawRectangle(108,128,62,97,BgColor);
			drawSpriteDir(I_DRINKING,118,128,1);
			//drawSpriteDir(I_CYGAR,175,200,1);
			times++;
			enable_timer(2);
			break;
		}
		case 3:{
			LCD_DrawRectangle(118,128,61,97,BgColor);
			drawSpriteDir(I_DRINKING,128,128,1);
			drawSpriteDir(I_CYGAR,175,204,1);
			drawSprite(I_SMOKE0,210,183);
			times++;
			enable_timer(2);
			break;
		}
		case 4:{
			LCD_DrawRectangle(128,128,61,97,BgColor);
			drawSpriteDir(I_DRINKING,138,128,1);
			drawSpriteDir(I_CYGAR,175,204,1);
			drawSprite(I_SMOKE1,210,183);
			times++;
			enable_timer(2);
			break;
		}
		case 5:{
			LCD_DrawRectangle(128,128,61,97,BgColor);
			drawSpriteDir(I_DRINKING,138,128,1);
			drawSpriteDir(I_CYGAR,175,204,1);
			drawSprite(I_SMOKE2,210,183);
			times++;
			enable_timer(2);
			break;
		}
		case 6:{
			LCD_DrawRectangle(128,122,98,105,BgColor);
			drawSpriteDir(I_DRINKING,98,128,1);
			times++;
			enable_timer(2);
			break;
		}
		case 7:{
			LCD_DrawRectangle(98,122,62,105,BgColor);
			drawSpriteDir(I_DRINKING,88,128,1);
			times++;
			enable_timer(2);
			break;
		}
		case 8:{
			disable_timer(2);
			LCD_DrawRectangle(88,122,133,105,BgColor);
			drawSprite(I_STEADY,84,122);
			moveEyes(CENTER,CENTER);
 			spriteState=STEADY;
			if(pendingUpdate[1]==1){//if no update has occurred update manually
				pendingUpdate[1]=0;
				if(satiety<FULL){ //if max is reached no update (No handle for overfull batteries)
					satiety+=25;
					drawBarLevels(SATIETY,satiety);
				}
			}
			times=0;
			break;
		}
	}
}

void endAnimation(){
	static uint8_t times = 0;
	switch(times){
	
		case 0:{
			drawSprite(I_END,84,122);
			times++;
			enable_timer(2);
			break;
		}
		case 1:{
			currentAudio=A_END;
			playCurrentAudio();
			LCD_DrawRectangle(84,122,71,105,BgColor);
			drawSprite(I_END,124,122);
			times++;
			enable_timer(2);
			break;
		}
		case 2:{
			LCD_DrawRectangle(124,122,71,105,BgColor);
			drawSprite(I_END,144,122);
			times++;
			enable_timer(2);
			break;
		}
		case 3:{
			LCD_DrawRectangle(144,122,71,105,BgColor);
			times++;
			enable_timer(2);
			break;
		}
		case 4:{
			drawSpriteBase(I_END_TEXT,0,122,0,0);
			times=0;
			break;
		}
	}
	
}
void endScene(){
	//changes current program flow stopping timer1 (no more update happiness/satiety or age)
	//spriteState = END
	//joystick is still polling however only select option can be used (setted to the reset button)
	//stop timer 1
	disable_timer(1); //no more age/animation update
	disable_timer(0);
	disable_timer(3);
	//animation sequence
	drawResetButton();
	
	endAnimation(); //run once than triggers timer to keep going 
	
	//end animation sequence
}

void cuddleAnimation(){
static uint8_t times = 0;
	switch(times){
		case 0:{
			spriteState=CUDDLE;
			pendingUpdate[0]=1; //update happiness request
			drawSprite(I_HEART,84,122);
			times++;
			enable_timer(2);
			enable_timer(3); //starts audio playing (A_CUDDLE)
			break;
		}
		case 1:{
			drawSprite(I_HEART,110,122);
			currentAudio=A_CUDDLE;
			playCurrentAudio();
			times++;
			enable_timer(2);
			break;
		}
		case 2:{
			drawSprite(I_HEART,136,122);
			times++;
			enable_timer(2);
			break;
		}
		case 3:{
			drawSprite(I_HEART,136,148);
			drawSprite(I_CUDDLE,98,170);
			times++;
			enable_timer(2);
			break;
		}
		case 4:{
			drawSprite(I_HEART,136,174);
			times++;
			enable_timer(2);
			break;
		}
		case 5:{
			drawSprite(I_HEART,136,200);
			times++;
			enable_timer(2);
			break;		
		}
		case 6:{
			drawSprite(I_HEART,110,200);
			times++;
			enable_timer(2);
			break;		
		}
		case 7:{
			drawSprite(I_HEART,84,200);
			times++;
			enable_timer(2);
			break;		
		}
		case 8:{
			drawSprite(I_HEART,84,174);
			times++;
			enable_timer(2);
			break;		
		}
		case 9:{
			times++;
			drawSprite(I_HEART,84,148);
			enable_timer(2);
			break;		
		}
		case 10:{
			LCD_DrawRectangle(84,122,71,105,BgColor);
			drawSprite(I_STEADY,84,122);
			moveEyes(CENTER,CENTER);
			times=0;
			if(pendingUpdate[0]==1){//if no update has occurred update manually
				pendingUpdate[0]=0;
				if(happiness<FULL){ //if max is reached no update (No handle for overfull batteries)
					happiness+=25;
					drawBarLevels(HAPPINESS,happiness);
				}
			}
			spriteState=STEADY;
		}
	}
}

void handleAnimation(){
	if(spriteState==DRINKING){
		drinkAnimation();
	}
	if(spriteState==CUDDLE){
		cuddleAnimation();
	}
	else if (spriteState==SMOKING){
		smokeAnimation();
	}
	else if(spriteState==END){
		endAnimation();
	}
}

void drawSelectedOption(uint8_t selected){
	if(selected==JOY_LX){
		drawEmptyRectangle(120,270,120,50,Black);
		drawEmptyRectangle(0,270,120,50,Warning_Red);
	
	}
	else if(selected==JOY_RX){
		drawEmptyRectangle(0,270,120,50,Black);
		drawEmptyRectangle(120,270,120,50,Warning_Red);
	}
	else if(selected==RESET){
		drawEmptyRectangle(0,270,240,50,Warning_Red);
	}
	else if(selected==NONE){
		drawActionButtons();
	}
	
}

void handleJoystick(){
	//static uint8_t count=0;
	//quando è in stato DRINKING o SMOKING return
	int i;
	if((LPC_GPIO1->FIOPIN & ((1<<25)))==0){
			if(spriteState==DRINKING||spriteState==SMOKING)
				return;
			if(spriteState==END){
				selectedOption=RESET;
				resetGame();
				selectedOption=CENTER;
			}
			else if(selectedOption==JOY_LX){
				spriteState=DRINKING;
				currentAudio=A_CLICK;
				playCurrentAudio();
				for(i=0;i<SECOND>>2;i++);
				handleAnimation();
			}else if(selectedOption==JOY_RX){
				spriteState=SMOKING;
				currentAudio=A_CLICK;
				playCurrentAudio();
				for(i=0;i<SECOND>>2;i++);
				handleAnimation();
			}
	}else if((LPC_GPIO1->FIOPIN & ((1<<27)))==0){
		//count=0;
		if(spriteState!=END){
			selectedOption=JOY_LX;
			drawSelectedOption(selectedOption);
		}
	}else if((LPC_GPIO1->FIOPIN & ((1<<28)))==0){
		//count=0;
		if(spriteState!=END){
			selectedOption=JOY_RX;
			drawSelectedOption(selectedOption);
		}
	}else {
		//currently not handling other directions
	}
}

void checkTouchPanel(){
	Coordinate *Screen_Ptr , *Display_Ptr;
	Screen_Ptr = Read_Ads7846();
	Display_Ptr = &display;
	if ( Screen_Ptr == (void *)0 || spriteState>STEADY_RIGHT ) return;
	getDisplayPoint( Display_Ptr, Screen_Ptr, &matrix);
	
	if( Display_Ptr->x >= 84 && Display_Ptr->x < 155 &&
			Display_Ptr->y >= 122 && Display_Ptr->y < 227)
	{
		spriteState=CUDDLE;
		cuddleAnimation();
	}
	return;
}
void animateSprite(){
	static uint8_t nextState=0,prevState=0;
 	switch(spriteState){
		case STEADY:{
			checkHealth();
			if(spriteState!=END){
				moveEyes(CENTER,prevState);
				spriteState = ((nextState==0)?STEADY_LEFT:STEADY_RIGHT);
				prevState=STEADY;
				nextState=(~nextState);
			}
			break;
		}
		case STEADY_LEFT:{
			moveEyes(LEFT,prevState);
			spriteState=STEADY;
			prevState=STEADY_LEFT;
			break;
		}
		case STEADY_RIGHT:{
			moveEyes(RIGHT,prevState);
			spriteState=STEADY;
			prevState=STEADY_RIGHT;
			break;
		}
		case DRINKING:{
			break;
		}
		case SMOKING:{
			break;
		}
		case END:{
			break;
		}
		case CUDDLE:{
			break;
		}
		default:{
			break;
		}
	}
}

//
void playCurrentAudio(){
	static int currentNote = 0;
	//static int ticks = 0;
	static AUDIO_t * song = NULL;

		//singleton to get the song
	//song == NULL if first call or audio is finished
	if(song==NULL){ 
		song = getAudio(currentAudio);
		if(song==NULL) return;//should not happen unless smt wrong in the code
	}
	
	if(song!=NULL && currentNote == (song->length))
	{
		currentAudio=A_NONE;
		currentNote=0;
		song=NULL;
		//stop timer 3 (and prepare for next call)
		
		//just in case ;)
		disable_timer(0); 
		LPC_TIM0->IR=0;
		
		reset_timer(3);
		init_timer(3,0x0100);
		disable_timer(3);

		return;
	}

	
	//handle audio playing
	if(!isNotePlaying())
	{
		//++ticks;
		//if(ticks == 1)
		//{
		//	ticks = 0;
			playNote(song->notes[currentNote++]);
		//}
	}
	return;
}



