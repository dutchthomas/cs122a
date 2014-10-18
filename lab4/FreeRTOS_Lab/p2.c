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
START,
WAIT,
PRESS,
RELEASE,
DIE,
PLAY
}
task1_state, task2_state;

unsigned char b0, b1, p, i, len, run;
char dir;

/*

light show patterns
first value => number of patterns in show

*/

unsigned char pattern[3][10] =
{
{3, 0x92, 0x49, 0x24},
{7, 0x81, 0x42, 0x24, 0x18, 0x24, 0x42, 0x81},
{7, 0x80, 0x60, 0x90, 0x88, 0x84, 0x82, 0x81},
};

// Number of patterns
unsigned char num = 3;


// Input and selection code
void Task1()
{

    while(1)
    { 	
    
        // Input
        b0 = !(PINA & 0x01);
        b1 = !(PINA & 0x02);
    
        // Actions
        switch(task1_state)
        {
            case INIT:
                run = 0;
            break;
            
            case START:
                p = 0;
                dir = 0;
                run = 1;
            break;
            
            case PRESS:
                if(p == 0 && dir == -1)
                {
                    p = num -1;
                }
                else
                {
                    p = (p + dir) % num;
                }
                
            break;
            
            case DIE:
            break;
            
            default:
            break;
        }
    
        // Transitions
        switch(task1_state)
        {
            case INIT:
                if(b0 && b1)
                {
                    task1_state = START;
                }
            break;
            
            case START:
                if(!b0 && !b1)
                {
                    task1_state = WAIT;
                }
            break;
            
            case WAIT:
                if(b0 && b1)
                {
                    task1_state = DIE;
                }
                else if(b0)
                {
                    dir = 1;
                    task1_state = PRESS;
                }
                else if(b1)
                {
                    dir = -1;
                    task1_state = PRESS;
                }
                else
                {   
                    // Do nothing
                    task1_state = WAIT;
                }
            break;
            
            case PRESS:
                if(b0 && b1)
                {
                    task1_state = DIE;
                }
                else
                {
                    task1_state = RELEASE;
                }
            break;
            
            case RELEASE:
                if(b0 && b1)
                {
                    task1_state = DIE;
                }
                else if(!b0 && !b1)
                {
                    task1_state = WAIT;
                }
            break;
            
            case DIE:
                if(!b0 && !b1)
                {
                    task1_state = INIT;
                }
            break;
            
            default:
                task1_state = INIT;
            break;
        }
       
        vTaskDelay(10);
    }
}

// Pattern flasher
void Task2()
{

    while(1)
    { 	
        len = pattern[p][0];
        
        // Actions
        switch(task2_state)
        {
            case INIT:
                transmit_data(0);
            break;
            
            case WAIT:
                transmit_data(0);
            break;
            
            case PLAY:
                transmit_data(pattern[p][i+1]);
                i = (i + 1) % len;
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(task2_state)
        {
            case INIT:
                task2_state = WAIT;
            break;
            case WAIT:
                if(run)
                {
                    task2_state = PLAY;
                }
            break;
            
            case PLAY:
                if(!run)
                {
                    task2_state = DIE;
                }
            break;
            
            case DIE:
                task2_state = INIT;
            break;
            
            default:
            break;
        }
        
        
        vTaskDelay(100);
    }
}
 
int main(void) 
{ 
    DDRA = 0x00; PORTA=0xFF;
    DDRC = 0xFF;
   
    task1_state = INIT;
    task2_state = INIT;
  
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    
    xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}