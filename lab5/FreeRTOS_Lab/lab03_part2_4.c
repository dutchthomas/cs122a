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

//button input
unsigned char button, reverse;

// machine 1
enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led1_state, led2_state, led3_state;
void LEDS1_Init(){
	led1_state = INIT;
}

void LEDS2_Init(){
	led2_state = INIT;
}

void LEDS1_Tick(){
	//Actions
	switch(led1_state){
		case INIT:
		PORTD = 0x00;
		break;
		case L0:
		PORTD = 0x01;
		break;
		case L1:
		PORTD = PORTD << 1;
		break;
		case L2:
		PORTD = PORTD >> 1;
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
			if(reverse)
			{
				led1_state = L2;
				reverse = 0;
			}
			else if(PORTD < 128)
			{
				led1_state = L1;
			} else {
				led1_state = L2;	
			}
		break;
		case L2:
			if(reverse)
			{
				led1_state = L1;
				reverse = 0;
			}
			else if(PORTD > 1)
			{
				led1_state = L2;
				} else {
				led1_state = L1;
			}
		break;
		default:
			led1_state = INIT;
		break;
	}
}

void LEDS2_Tick(){
	//Actions
	switch(led2_state){
		case INIT:
			reverse = 0;
		break;
		case L0:
		break;
		case L1:
			reverse = 1;
		break;
		case L2:
		break;
		default:
		break;
	}
	//Transitions
	switch(led2_state){
		case INIT:
			led2_state = L0;
		break;
		case L0:
			if(button)
			{
				led2_state = L1;
			}

		break;
		case L1:
			led2_state = L2;
		break;
		case L2:
			if(!button)
			{
				led2_state = L0;
			}
		break;
		default:
		led2_state = INIT;
		break;
	}
}

void Led1SecTask()
{
	LEDS1_Init();
   for(;;) 
   { 	
	LEDS1_Tick();
	vTaskDelay(100); 
   } 
}

void Led2SecTask()
{
	LEDS2_Init();
	for(;;)
	{
		LEDS2_Tick();
		button = !(PINA & 0x01);
		PORTB = reverse;
		vTaskDelay(10);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Led1SecTask, (signed portCHAR *)"Led1SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
	xTaskCreate(Led2SecTask, (signed portCHAR *)"Led2SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}	
 
int main(void) 
{ 
   DDRA = 0x00; PORTA=0xFF;
   DDRD = 0xFF;
   DDRB = 0xFF;
   //Start Tasks  
   StartSecPulse(1);
    //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}