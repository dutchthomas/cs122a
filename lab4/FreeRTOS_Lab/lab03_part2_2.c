#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h"

unsigned char SetBit(unsigned char input, unsigned char pos, unsigned char val){
	return (input & (~(0x01 << pos))) | (val << pos);
}

// machine 1
enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led1_state, led2_state, led3_state;
void LEDS1_Init(){
	led1_state = INIT;
	
}

void LEDS1_Tick(){
	//Actions
	switch(led1_state){
		case INIT:
		PORTD = SetBit(PORTD, 0, 0);
		break;
		case L0:
		PORTD = SetBit(PORTD, 0, 0);
		break;
		case L1:
		PORTD = SetBit(PORTD, 0, 1);
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led1_state){
		case INIT:
			led1_state = L0;
		break;
		case L0:
			led1_state = L1;
		break;
		case L1:
			led1_state = L0;
		break;
		default:
			led1_state = INIT;
		break;
	}
}
// machine 2
void LEDS2_Init(){
	led2_state = INIT;
}

void LEDS2_Tick(){
	//Actions
	switch(led2_state){
		case INIT:
		PORTD = SetBit(PORTD, 2, 0);
		break;
		case L0:
		PORTD = SetBit(PORTD, 2, 0);
		break;
		case L1:
		PORTD = SetBit(PORTD, 2, 1);
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led2_state){
		case INIT:
		led2_state = L0;
		break;
		case L0:
		led2_state = L1;
		break;
		case L1:
		led2_state = L0;
		break;
		default:
		led2_state = INIT;
		break;
	}
}
// machine 3
void LEDS3_Init(){
	led3_state = INIT;
}

void LEDS3_Tick(){
	//Actions
	switch(led3_state){
		case INIT:
		PORTD = SetBit(PORTD, 4, 0);
		break;
		case L0:
		PORTD = SetBit(PORTD, 4, 0);
		break;
		case L1:
		PORTD = SetBit(PORTD, 4, 1);
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
	switch(led3_state){
		case INIT:
		led3_state = L0;
		break;
		case L0:
		led3_state = L1;
		break;
		case L1:
		led3_state = L0;
		break;
		default:
		led3_state = INIT;
		break;
	}
}

void Led1SecTask()
{
	LEDS1_Init();
   for(;;) 
   { 	
	LEDS1_Tick();
	vTaskDelay(500); 
   } 
}

void Led2SecTask()
{
	LEDS2_Init();
	for(;;)
	{
		LEDS2_Tick();
		vTaskDelay(1000);
	}
}

void Led3SecTask()
{
	LEDS3_Init();
	for(;;)
	{
		LEDS3_Tick();
		vTaskDelay(2500);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Led1SecTask, (signed portCHAR *)"Led1SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Led2SecTask, (signed portCHAR *)"Led2SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Led3SecTask, (signed portCHAR *)"Led3SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}