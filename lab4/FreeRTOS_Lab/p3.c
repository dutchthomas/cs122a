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

void transmit_data(unsigned char data, unsigned char line);

void transmit_data(unsigned char data, unsigned char line)
{
	int i;
	
	for(i = 7; i >= 0; --i)
	{
	    // Sets SRCLR to 1 to allow data to be set
	    // clears SRCLK in preparation of sending data
	    if(line == 0)
	    {
		    PORTC = 0x08;
	    }
	    else if(line == 1)
	    {
		    PORTC = 0x20;
	    }
		
		// set SER = next bit of data to be sent
		PORTC |= ((data >> i) & 0x01);
		
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register.
		PORTC |= 0x04;
	}
	
	// set RCLK = 1. Rising edge copies data from "shift" to "storage"
	if(line == 0)
	{
	    PORTC |= 0x02;
    }
    else if(line == 1)
    {
	    PORTC |= 0x10;
    }
	
	// clears all lines for new transmission
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
};

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

struct lightSet
{
    unsigned char b0, b1, p, i, run, state1, state2, line, len;
    char dir;
};

struct lightSet A, B;

void patternSelect(struct lightSet *lights)
{
        
        // Actions
        switch(lights->state1)
        {
            case INIT:
                lights->run = 0;
            break;
            
            case START:
                lights->p = 0;
                lights->dir = 0;
                lights->run = 1;
            break;
            
            case PRESS:
                if(lights->p == 0 && lights->dir == -1)
                {
                    lights->p = num -1;
                }
                else
                {
                    lights->p = (lights->p + lights->dir) % num;
                }
                
            break;
            
            case DIE:
            break;
            
            default:
            break;
        }
    
        // Transitions
        switch(lights->state1)
        {
            case INIT:
                if(lights->b0 && lights->b1)
                {
                    lights->state1 = START;
                }
            break;
            
            case START:
                if(!lights->b0 && !lights->b1)
                {
                    lights->state1 = WAIT;
                }
            break;
            
            case WAIT:
                if(lights->b0 && lights->b1)
                {
                    lights->state1 = DIE;
                }
                else if(lights->b0)
                {
                    lights->dir = 1;
                    lights->state1 = PRESS;
                }
                else if(lights->b1)
                {
                    lights->dir = -1;
                    lights->state1 = PRESS;
                }
                else
                {   
                    // Do nothing
                    lights->state1 = WAIT;
                }
            break;
            
            case PRESS:
                if(lights->b0 && lights->b1)
                {
                    lights->state1 = DIE;
                }
                else
                {
                    lights->state1 = RELEASE;
                }
            break;
            
            case RELEASE:
                if(lights->b0 && lights->b1)
                {
                    lights->state1 = DIE;
                }
                else if(!lights->b0 && !lights->b1)
                {
                    lights->state1 = WAIT;
                }
            break;
            
            case DIE:
                if(!lights->b0 && !lights->b1)
                {
                    lights->state1 = INIT;
                }
            break;
            
            default:
                lights->state1 = INIT;
            break;
        }
        
}

void patternDraw(struct lightSet *lights)
{

        lights->len = pattern[lights->p][0];
        
        // Actions
        switch(lights->state2)
        {
            case INIT:
                transmit_data(0, lights->line);
            break;
            
            case WAIT:
                transmit_data(0, lights->line);
            break;
            
            case PLAY:
                transmit_data(pattern[lights->p][lights->i+1], lights->line);
                lights->i = (lights->i + 1) % lights->len;
            break;
            
            default:
            break;
        }
        
        // Transitions
        switch(lights->state2)
        {
            case INIT:
                lights->state2 = WAIT;
            break;
            case WAIT:
                if(lights->run)
                {
                    lights->state2 = PLAY;
                }
            break;
            
            case PLAY:
                if(!lights->run)
                {
                    lights->state2 = DIE;
                }
            break;
            
            case DIE:
                lights->state2 = INIT;
            break;
            
            default:
                lights->state2 = INIT;
            break;
        }

}

// Input and selection code
void Task1()
{

    while(1)
    { 	
    
        // Input
        A.b0 = !(PINA & 0x01);
        A.b1 = !(PINA & 0x02);
        
        patternSelect(&A);
       
        vTaskDelay(10);
    }
}

// Pattern flasher
void Task2()
{

    while(1)
    { 	
        patternDraw(&A);
        vTaskDelay(100);
    }
}

// Input and selection code
void Task3()
{

    while(1)
    { 	
    
        // Input
        B.b0 = !(PINA & 0x04);
        B.b1 = !(PINA & 0x08);
        
        patternSelect(&B);
       
        vTaskDelay(10);
    }
}

// Pattern flasher
void Task4()
{

    while(1)
    { 	
        patternDraw(&B);
        vTaskDelay(100);
    }
}
 
int main(void) 
{ 
    DDRA = 0x00; PORTA=0xFF;
    DDRC = 0xFF;
    
    A.line = 0;
    B.line = 1;
  
    xTaskCreate(Task1, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    
    xTaskCreate(Task2, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    
    xTaskCreate(Task3, (signed portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    
    xTaskCreate(Task4, (signed portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
 
    vTaskStartScheduler();
    
    return 0; 
}