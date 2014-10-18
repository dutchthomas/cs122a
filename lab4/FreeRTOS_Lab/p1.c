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

void transmit_data(unsigned char data);

void transmit_data(unsigned char data)
{
	int i;
	
	for(i = 7; i >= 0; --i)
	{
		PORTC = 0x08;
		PORTC |= ((data >> i) & 0x01);
		PORTC |= 0x04;
	}
	
	PORTC |= 0x02;
	PORTC = 0x00;
}

enum state_labels
{
INIT,
WAIT,
PRESS,
RELEASE
}
task1_state;

unsigned char b0, b1, leds, dir;

void Task1()
{

    while(1)
    { 	
    
        // Input
        b0 = !(PINA & 0x01);
        b1 = !(PINA & 0x02);
        
        //transmit_data(task1_state);
    
        // Actions
        switch(task1_state)
        {
            case INIT:
                leds = 0;
                dir = 0;
                transmit_data(leds);
            break;
            
            case PRESS:
                leds += dir;
                transmit_data(leds);
            break;
            
            default:
            break;
        }
    
        // Transitions
        switch(task1_state)
        {
            case INIT:
                task1_state = WAIT;
            break;
            
            case WAIT:
                if(b0)
                {
                    dir = 1;
                    task1_state = PRESS;
                }
                else if(b1)
                {
                    dir = -1;
                    task1_state = PRESS;
                }
            break;
            
            case PRESS:
                task1_state = RELEASE;
            break;
            
            case RELEASE:
                if(!b0 && !b1)
                {
                    task1_state = WAIT;
                }
            break;
            
            default:
                task1_state = INIT;
            break;
        }
       
        vTaskDelay(10);
    }
}
 
int main(void) 
{ 
    DDRA = 0x00; PORTA=0xFF;
    DDRC = 0xFF;
   
    task1_state = INIT;
  
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}