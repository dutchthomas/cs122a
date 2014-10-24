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
enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led1_state;
void LEDS1_Init(){
	led1_state = INIT;
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
		if(PORTD < 128)
		{
			led1_state = L1;
			} else {
			led1_state = L2;
		}
		break;
		case L2:
		if(PORTD > 1)
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

void Led1SecTask()
{
	LEDS1_Init();
	for(;;)
	{
		LEDS1_Tick();
		vTaskDelay(100);
	}
}


void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Led1SecTask, (signed portCHAR *)"Led1SecTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
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